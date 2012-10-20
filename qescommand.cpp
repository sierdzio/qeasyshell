#include "qescommand.h"

#include <QtCore/QProcess>

QesCommand::QesCommand(const QString &command,
                       const QStringList &arguments = QStringList(),
                       Qes::Shell shell,
                       QObject *parent) :
    QObject(parent), m_command(command), m_arguments(arguments), m_shell(shell)
{
}

QesResult QesCommand::run(const QByteArray &input)
{
    QesResult result;
    QProcess command;
    command.start(Qes::commandForShell(m_shell) + " -c \""
                  + m_command + m_arguments.join(" ") + "\"",
                  m_arguments);

    if (!command.waitForStarted()) {
        result.setValid(false);
        return result;
    }

    if (!input.isEmpty()) {
        command.write(input);
        command.closeWriteChannel();
    }

    if (!command.waitForFinished()) {
        result.setValid(false);
        return result;
    }

    result.setRaw(command.getAll());
    return result;
}

QesResult *QesCommand::runDetached()
{
    // TODO: implement!
    return 0;
}
