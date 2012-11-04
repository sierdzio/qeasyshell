#include "qescommand.h"

/*!
  Initialises the command object with it's first \a command string.
  */
QesCommand::QesCommand(const QString &command,
                       QObject *parent) :
    QObject(parent)
{
    m_commands.append(QesSubCommand(command));
}

/*!
  Adds a specified \a command as a pipe after the previous one. Returns a pointer
  to the updated QesCommand object, to allow easy chaining.

  For example:
  \code{.cpp}
  QesCommand env("env");
  QesResult *result = env.pipe("grep USER")->chain("pwd")->pipe("wc")->run();
  \endcode

  \sa chain, run
  */
QesCommand *QesCommand::pipe(const QString &command)
{
    m_commands.append(QesSubCommand(command, Qes::Pipe));
    return this;
}

/*!
  Adds a specified \a command as a chain after the previous one. Returns a pointer
  to the updated QesCommand object, to allow easy chaining.

  For example:
  \code{.cpp}
  QesCommand env("env");
  QesResult *result = env.pipe("grep USER")->chain("pwd")->pipe("wc")->run();
  \endcode

  \sa pipe, run
  */
QesCommand *QesCommand::chain(const QString &command)
{
    m_commands.append(QesSubCommand(command, Qes::Chain));
    return this;
}

/*!
  Runs the command (or the whole chain if more commands were run), optionally
  passing the \a input data to the standard input of the first command in
  the chain.

  This command it synchronous - it will wait for the whole chain to finish
  executing before exiting. As such, it should be used with caution, as it might
  cause the application to freeze (you can use runDetached() to prevent this).

  \sa runDetached, pipe, chain
  */
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

/*!
  Same as run, but asynchronous - the method returns immediately and executes in
  the background. In order to get the result, you need to wait for signals it
  will emit once finished.

  \sa run, pipe, chain
  */
QesResult *QesCommand::runDetached()
{
    // TODO: implement!
    return 0;
}

/*!
  Returns the whole command chain nicely formatted (just as it would look like
  when used in a standard shell).

  Example:
  \code
  QesCommand env("env");
  QesResult *result = env.pipe("grep USER")->chain("pwd")->pipe("wc")->run();
  qDebug() << "Command is:" << env.command();
  \endcode

  Results in:
  \verbatim
  Command is: "env | grep USER && pwd | wc"
  \endverbatim
  */
QString QesCommand::command()
{
    QString result;

    foreach (const QesSubCommand &s, m_commands) {
        result += s.toString();
    }

    return result;
}

/*!
  Private method, used to connect QesProcess instanes with QesResult.
  */
void QesCommand::connectOutputs(QesProcess *process, QesResult *result)
{
    connect(process, SIGNAL(readyReadStandardOutput(const QByteArray &, int)),
            result, SLOT(appendStdOut(const QByteArray &)));
    connect(process, SIGNAL(readyReadStandardError(const QByteArray &, int)),
            result, SLOT(appendStdErr(const QByteArray &)));
}
