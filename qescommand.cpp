#include "qescommand.h"

#include <QtCore/QProcess>
//#include <QtCore/QTextStream>
//#include <QtCore/QIODevice>
//#include <QtCore/QFile>
//#include <QtCore/QTemporaryFile>
//#include <QtCore/QDir>

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

        if (pipeline == Qes::None || pipeline == Qes::Pipe) {
            if (i != 0) {
                QesProcess *current = processList.at(i);
                QesProcess *previous = processList.at(i - 1);

//                connect(previous, SIGNAL(readyReadStandardOutput(const QByteArray &, int)),
//                        current, SLOT(write(const QByteArray &, int)));

                if ((i < (m_commands.length() - 1) && m_commands.at(i + 1).pipeline() == Qes::Chain)
                        ||(i == (m_commands.length() - 1))) {
                    connect(current, SIGNAL(readyReadStandardOutput(const QByteArray &, int)),
                            result, SLOT(appendStdOut(const QByteArray &)));
                    connect(current, SIGNAL(readyReadStandardError(const QByteArray &, int)),
                            result, SLOT(appendStdErr(const QByteArray &)));
                }

                previous->setStandardOutputProcess(current);
                previous->start(m_commands.at(i - 1).command());
            }
        } else if (pipeline == Qes::Chain) {
            // TODO: make this part synchronous (all previous commands must finish)
        }
    }

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
