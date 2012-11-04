#include "qescommand.h"

//#include <QtCore/QProcess>
#include <QtCore/QTextStream>
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
        if (pipeline == Qes::None || pipeline == Qes::Pipe) {
            processList.append(runSubcommand(i));

            if (i != 0) {
                QProcess *current = processList.at(i);
                QProcess *previous = processList.at(i - 1);

                connect(previous, SIGNAL(readyReadStandardOutput(int, const QByteArray &)),
                        current, SLOT(write(int, const QByteArray &)));
            }
        } else if (pipeline == Qes::Chain) {
            // TODO: make this part synchronous (all previous commands must finish)
        }
    }




    /*
    QProcess command;
    // TODO: add choice of working directory
    QTemporaryFile script;

    if (script.open()) {
        QTextStream scriptText(&script);
        bool perm = script.setPermissions(script.permissions()
                                          | QFile::ExeOwner | QFile::ExeUser
                                          | QFile::ExeGroup | QFile::ExeOther);
        scriptText << QString("#!/bin/" + Qes::shellToString(m_shell)) << " \n";
        scriptText << QString("echo I am Working! \"" + m_command + "\" \n");
        scriptText << QString("echo $(" + m_command + ")") << "\n";
        scriptText << "exit";
        script.close();

        command.start("/bin/sh", QStringList() << script.fileName());

#ifdef DEBUG
        qDebug(qPrintable(m_command + "\n" + QString::number(perm) + "\n" + script.fileName()
                          + "\n"), NULL);
#endif

        if (!command.waitForStarted()) {
            result->setValid(false);
            return result;
        }

        if (!input.isEmpty()) {
            command.write(input);
            command.closeWriteChannel();
        }

        if (!command.waitForFinished()) {
            result->setValid(false);
            return result;
        }

        result->setError(command.readAllStandardError());
        result->setRaw(command.readAllStandardOutput()); //readAll()
    } else {
        result->setValid(false);
    }
    */

    return result;
}

QesResult *QesCommand::runDetached()
{
    // TODO: implement!
    return 0;
}

//QString QesCommand::command()
//{
//    return m_command;
//}

Qes::Shell QesCommand::shell()
{
    return m_shell;
}

QesProcess *QesCommand::runSubcommand(int index)
{
    QesProcess *command = new QesProcess(index, this);
    // TODO: connect error out with QesResult
    command->start(m_commands.at(index).command());
    return command;
}
