#include <QtCore/QProcess>

#include "qescommand.h"

QesCommand::QesCommand(const QString &command,
                       Qes::Shell shell,
                       QObject *parent) :
    QObject(parent), m_shell(shell)
{
    m_commands.append(QesSubCommand(command));
}

QesCommand *QesCommand::pipe(const QString &command)
{
    m_commands.append(QesSubCommand(command, Qes::Pipe));
    return this;
}

QesCommand *QesCommand::chain(const QString &command)
{
    m_commands.append(QesSubCommand(command, Qes::Chain));
    return this;
}

QesResult *QesCommand::run(const QByteArray &input)
{
    Q_UNUSED(input);

    QesResult *result = new QesResult(this);
    ProcessList processList;

    for(int i = 0; i < m_commands.length(); ++i) {
        Qes::Pipeline pipeline = m_commands.at(i).pipeline();
        QesProcess *command = new QesProcess(i, this);
        processList.append(command);

        QesProcess *current = processList.at(i);
        QesProcess *previous = processList.at(i - 1);

        if (pipeline == Qes::None || pipeline == Qes::Pipe) {
            if (i != 0) {
                if ((i < (m_commands.length() - 1) && m_commands.at(i + 1).pipeline() == Qes::Chain)
                        ||(i == (m_commands.length() - 1))) {
                    connectOutputs(current, result);
                }

                previous->setStandardOutputProcess(current);
                previous->start(m_commands.at(i - 1).command());
            }
        } else if (pipeline == Qes::Chain) {
            // In a chain, we must wait for pre-chain commands to finish
            previous->start(m_commands.at(i - 1).command());
            for(int j = 0; j < i; ++j) {
                processList.at(j)->waitForFinished();
            }

            connectOutputs(current, result);
        }
    }

    // This will do nothing if last command was chained
    processList.last()->start(m_commands.last().command());

    for(int i = 0; i < m_commands.length(); ++i) {
        processList.at(i)->waitForFinished();
    }

    return result;
}

QesResult *QesCommand::runDetached()
{
    // TODO: implement!
    return 0;
}

QString QesCommand::command()
{
    QString result;

    foreach (const QesSubCommand &s, m_commands) {
        result += s.toString();
    }

    return result;
}

Qes::Shell QesCommand::shell()
{
    return m_shell;
}

void QesCommand::connectOutputs(QesProcess *process, QesResult *result)
{
    connect(process, SIGNAL(readyReadStandardOutput(const QByteArray &, int)),
            result, SLOT(appendStdOut(const QByteArray &)));
    connect(process, SIGNAL(readyReadStandardError(const QByteArray &, int)),
            result, SLOT(appendStdErr(const QByteArray &)));
}
