/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <QFile>
#include <QFileInfo>

#include "qescommand.h"

/*!
  Initialises the command object with it's first \a command string.

  If \a windowsCompatibility is turned on, QEasyShell will try to make commands
  work on Windows platform (it will add cmd.exe invocations automatically).
  */
QesCommand::QesCommand(const QString &command, bool windowsCompatibility,
                       QObject *parent) :
    QObject(parent), mWindowsCompatibility(windowsCompatibility)
{
    mFinished = false;
    mCommands.append(QesSubCommand(command));
    mResult = NULL;
    mCurrentCommandIndex = -1;
}

/*!
  Adds a specified \a command as a pipe after the previous one. Returns a pointer
  to the updated QesCommand object, to allow easy chaining.

  For example:
  \code{.cpp}
  QesCommand env("env");
  QesResult *result = env.pipe("grep USER")->chain("pwd")->pipe("wc")->run();
  \endcode

  \sa chain, run, operator|()
  */
QesCommand *QesCommand::pipe(const QString &command)
{
    mCommands.append(QesSubCommand(command, Qes::Pipe));
    return this;
}

/*!
  An overload taking another QesCommand object.
  */
QesCommand *QesCommand::pipe(QesCommand *command)
{
    CommandList temp = command->commandList();
    QesSubCommand tempsub = temp.takeAt(0);
    // This is required because the first subcommand is usually set to Qes::None
    tempsub.setPipeline(Qes::Pipe);
    mCommands.append(tempsub);
    mCommands.append(temp);
    return this;
}

/*!
  Handy overload for pipe method.

  \verbatim
    QesCommand env("env");
    QesResult *result = (env | ("grep USER")->run();
  \endverbatim

  \sa pipe
  */
QesCommand *QesCommand::operator |(QesCommand *command)
{
    return pipe(command);
}

/*!
  Handy overload for pipe method.

  \verbatim
    QesCommand env("env");
    QesResult *result = (env | ("grep USER")->run();
  \endverbatim

  \sa pipe
  */
QesCommand *QesCommand::operator |(const QString &command)
{
    return pipe(command);
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
    mCommands.append(QesSubCommand(command, Qes::Chain));
    return this;
}

/*!
  An overload taking another QesCommand object.
  */
QesCommand *QesCommand::chain(QesCommand *command)
{
    CommandList temp = command->commandList();
    QesSubCommand tempsub = temp.takeAt(0);
    // This is required because the first subcommand is usually set to Qes::None
    tempsub.setPipeline(Qes::Chain);
    mCommands.append(tempsub);
    mCommands.append(temp);
    return this;
}

/*!
  Adds a redirection command to the command chain. You can choose between
  Qes::Redirect and Qes::RedirectAppend.

  \sa operator>>()
  */
QesCommand *QesCommand::redirect(const QString &destination,
                                 Qes::Pipeline redirectionMode,
                                 QProcess::ProcessChannel channel)
{
    Q_UNUSED(channel);

    mCommands.append(QesSubCommand(destination, redirectionMode));
    return this;
}

/*!
  A nice overload for redirect().

  \sa redirect
  */
QesCommand *QesCommand::operator >>(const QString &destination)
{
    return redirect(destination, Qes::RedirectAppend, QProcess::StandardOutput);
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
    QesResult *result = new QesResult(this);
    mProcessList.clear();

    for(int i = 0; i < mCommands.length(); ++i) {
        int previousIndex = i - 1;
        Qes::Pipeline pipeline = mCommands.at(i).pipeline();

        // Skip process creation for redirection. It just needs to write
        // output.
        if ((previousIndex >= 0) && (mCommands.at(previousIndex).pipeline()
                                     & (Qes::Redirect | Qes::RedirectAppend))) {
            --previousIndex;
        }

        if (pipeline & (Qes::Redirect | Qes::RedirectAppend)) {
            mProcessList.append(new QesProcess(i, this));
            ++i;
            pipeline = mCommands.at(i).pipeline();
        }

        mProcessList.append(new QesProcess(i, this));

        QesProcess *current = mProcessList.at(i);
        QesProcess *previous = (i != 0)? mProcessList.at(previousIndex) : 0;

        if (pipeline == Qes::None || pipeline == Qes::Pipe) {
            if ((i < (mCommands.length() - 1) && mCommands.at(i + 1).pipeline() == Qes::Chain)
                    ||(i == (mCommands.length() - 1))) {
                connectOutputs(current, result);
            }

            if (i != 0) {
                if (mCommands.at(i - 1).pipeline() == Qes::Redirect) {
                    previous->setStandardOutputFile(mCommands.at(i - 1).command(), QIODevice::Truncate);
                    previous->setStandardErrorFile(mCommands.at(i - 1).command(), QIODevice::Truncate);
                } else if (mCommands.at(i - 1).pipeline() == Qes::RedirectAppend) {
                    previous->setStandardOutputFile(mCommands.at(i - 1).command(), QIODevice::Append);
                    previous->setStandardErrorFile(mCommands.at(i - 1).command(), QIODevice::Append);
                } else {

                    previous->setStandardOutputProcess(current);
                }

                previous->start(prepareCommand(mCommands.at(previousIndex).command(),
                                               mWindowsCompatibility));
                if ((i >= 2) && (mCommands.at(i - 1).pipeline() & (Qes::Redirect | Qes::RedirectAppend))) {
                    previous->closeWriteChannel();
                }

                if (i == 1) {
                    previous->write(input);
                    previous->closeWriteChannel();
                }
            }
        } else if (pipeline == Qes::Chain) {
            // In a chain, we must wait for pre-chain commands to finish
            if (i > 0) {
                previous->start(prepareCommand(mCommands.at(previousIndex).command(),
                                               mWindowsCompatibility));
            }

            if (i == 1) {
                previous->write(input);
                previous->closeWriteChannel();
            }

            for(int j = 0; j < i; ++j) {
                if (mProcessList.at(j)->state() != QProcess::NotRunning)
                    mProcessList.at(j)->waitForFinished();
            }

            connectOutputs(current, result);
        }
    }

    // This will do nothing if last command was chained
    mProcessList.last()->start(prepareCommand(mCommands.last().command(),
                                             mWindowsCompatibility));

    if ((mCommands.length() >= 3) && (mCommands.at(mCommands.length() - 2).pipeline()
                                       & (Qes::Redirect | Qes::RedirectAppend))) {
        mProcessList.last()->closeWriteChannel();
    }

    if (mCommands.length() == 1) {
        mProcessList.last()->write(input);
        mProcessList.last()->closeWriteChannel();
    }

    for(int i = 0; i < mCommands.length(); ++i) {
        if (mProcessList.at(i)->state() != QProcess::NotRunning) {
            mProcessList.at(i)->waitForFinished();
        }

        if (mProcessList.at(i)->isError()) {
            result->appendProgressError(QString("Process: " + mCommands.at(i).command()
                                                + " failed! Error: "
                                                + mProcessList.at(i)->errorString()));
        }
    }

    mResult = result;
    clearMembers();
    emit finished(result);
    return result;
}

/*!
  Same as run, but asynchronous - the method returns immediately and executes in
  the background. In order to get the result, you need to wait for signals it
  will emit once finished (finished()), and then get the result using result().

  \sa run, pipe, chain, finished, isFinished, result
  */
void QesCommand::runDetached(const QByteArray &input)
{
    Q_UNUSED(input);
    mProcessList.clear();
    mCurrentCommandIndex = 0;
    this->metaObject()->invokeMethod(this, "processNextStep",
                                     Q_ARG(int, 0),
                                     Q_ARG(QProcess::ExitStatus, QProcess::NormalExit),
                                     Q_ARG(QByteArray, input));
    return;
}

/*!
  Attempts to peacefully terminate all processes spawned by this QesCommand.
  It does not clear command list (you can rerun command chain), nor does it
  clean the QesResult object - you can still access it and get any data that was
  produced by running commands before they were terminated.

  \sa clear, result, killAll
  */
void QesCommand::terminateAll()
{
    foreach (QesProcess *p, mProcessList) {
        p->terminate();
    }

    clearMembers();
}

/*!
  Forcibly kills (immediately) all processes spawned by this QesCommand.
  It does not clear command list (you can rerun command chain), nor does it
  clean the QesResult object - you can still access it and get any data that was
  produced by running commands before they were killed.

  \sa clear, result, terminateAll
  */
void QesCommand::killAll()
{
    foreach (QesProcess *p, mProcessList) {
        p->kill();
    }

    clearMembers();
}

/*!
  Returns a result of the command. Is null if no command was run.

  To check if there were any errors, you can use the QesResult object's error() method.

  \sa isResultReady, isFinished
 */
QesResult *QesCommand::result() const
{
    return mResult;
}

/*!
  Returns true if command finished executing and the result is ready.

  \sa result, isFinished
 */
bool QesCommand::isResultReady() const
{
    return mFinished;
}

/*!
  Returns true if command finished executing and the result is ready.

  This is a simple synonym to isResultReady.

  \sa result, isResultReady
 */
bool QesCommand::isFinished() const
{
    return mFinished;
}

/*!
  Processes next blocking step of command chain execution (that is, QesCommand
  needs to wait for all pipes to finish before it can execute any chain commands
  ('&&')).
 */
void QesCommand::processNextStep(int pid, QProcess::ExitStatus pes, const QByteArray &input)
{
    if (mResult == NULL)
        mResult = new QesResult(this);

    if (pes == QProcess::CrashExit) {
        mResult->appendProgressError(QString("Process: " + mCommands.at(pid).command() + " failed! Error: "
                                              + mProcessList.at(pid)->errorString()));
        return;
    }

    for(int i = mCurrentCommandIndex; i < mCommands.length(); ++i) {
        bool takeABreak = false;
        int previousIndex = i - 1;
        mCurrentCommandIndex = i;
        Qes::Pipeline pipeline = mCommands.at(i).pipeline();

        // Skip process creation for redirection. It just needs to write
        // output.
        if ((previousIndex >= 0) && (mCommands.at(previousIndex).pipeline() & (Qes::Redirect | Qes::RedirectAppend))) {
            --previousIndex;
        }

        if (pipeline & (Qes::Redirect | Qes::RedirectAppend)) {
            mProcessList.append(new QesProcess(i, this));
            ++i;
            ++mCurrentCommandIndex;
            //--previousIndex;
            pipeline = mCommands.at(i).pipeline();
        }

        mProcessList.append(new QesProcess(i, this));

        QesProcess *current = mProcessList.at(i);
        QesProcess *previous = (i != 0)? mProcessList.at(previousIndex) : 0;

        if (pipeline == Qes::None || pipeline == Qes::Pipe) {
            // TODO: this code is important in chain, too! What if chain is the last command, eh?
            if ((i < (mCommands.length() - 1) && mCommands.at(i + 1).pipeline() == Qes::Chain)
                    ||(i == (mCommands.length() - 1))) {
                connectOutputs(current, mResult);
            }

            if (i != 0) {
                if (mCommands.at(i - 1).pipeline() == Qes::Redirect) {
                    previous->setStandardOutputFile(mCommands.at(i - 1).command(), QIODevice::Truncate);
                    previous->setStandardErrorFile(mCommands.at(i - 1).command(), QIODevice::Truncate);
                } else if (mCommands.at(i - 1).pipeline() == Qes::RedirectAppend) {
                    previous->setStandardOutputFile(mCommands.at(i - 1).command(), QIODevice::Append);
                    previous->setStandardErrorFile(mCommands.at(i - 1).command(), QIODevice::Append);
                } else {
                    previous->setStandardOutputProcess(current);
                }
                previous->start(prepareCommand(mCommands.at(previousIndex).command(),
                                               mWindowsCompatibility));

                if (i == 1) {
                    previous->write(input);
                    previous->closeWriteChannel();
                }
            }
        } else if (pipeline == Qes::Chain) {
            previous->start(prepareCommand(mCommands.at(previousIndex).command(),
                                           mWindowsCompatibility));

            if (i == 1) {
                previous->write(input);
                previous->closeWriteChannel();
            }

            connectOutputs(current, mResult);
            connect(previous, SIGNAL(finished(int, QProcess::ExitStatus)),
                    this, SLOT(processNextStep(int, QProcess::ExitStatus)));
            takeABreak = true;
            ++mCurrentCommandIndex;
        }

        if (i == (mCommands.length() - 1)) {
            current->start(prepareCommand(mCommands.at(i).command(),
                                          mWindowsCompatibility));
            connect(current, SIGNAL(finished(int)), this, SLOT(aboutToFinish()));
        }

        if (takeABreak)
            break;
    }
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
QString QesCommand::command() const
{
    QString result;

    foreach (const QesSubCommand &s, mCommands) {
        result += s.toString();
    }

    return result;
}

/*!
  Returns the internal list of commands. Not very useful for end users, but
  important in chaining subcommands.
  */
CommandList QesCommand::commandList() const
{
    return mCommands;
}

/*!
  Clear method lists. Method will bail out if processes are already running!
  */
void QesCommand::clear()
{
    foreach (QesProcess *p, mProcessList) {
        if (p->state() != QProcess::NotRunning) {
            return;
        }
    }

    clearMembers();
    mCommands.clear();
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

/*!
  Writes data to file specified in redirect command. Returns true on success,
  otherwise returns false.

  Requires to specify the output \a filename, \a data to write and write mode
  designated in \a pipe (Redirect or RedirectAppend).
  */
bool QesCommand::redirectToFile(const QString &filename, const QByteArray &data, Qes::Pipeline pipe)
{
    // need to add QProcess::Channel

    QFile file(filename);

    if (pipe == Qes::Redirect) {
        if (!file.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly))
            return false;
    } else if (pipe == Qes::RedirectAppend) {
        if (!file.open(QIODevice::Append | QIODevice::Text | QIODevice::WriteOnly))
            return false;
    }

    if (file.write(data) == -1) {
        return false;
    }

    file.close();
    return true;
}

/*!
  Tries to fix a call if it may not work.

  Especially needed on Windows, together with \a windowsMode set to true.
  */
QString QesCommand::prepareCommand(const QString &command, bool windowsMode)
{
    QString commandToRun(command);
    if (windowsMode) {
        // TODO: use PowerShell if available
        if (!commandToRun.contains("cmd.exe")) {
            commandToRun.prepend("cmd.exe /C ");
        }
    }

    return commandToRun;
}

/*!
  Clears member variables to conserve memory.
  */
void QesCommand::clearMembers()
{
    foreach (QesProcess *p, mProcessList) {
        p->deleteLater();
    }

    mProcessList.clear();
    mFinished = false;
    mCurrentCommandIndex = -1;
}

/*!
  Performs the actions needed to finalise detahed command execution.

  Emits finished() signals.
 */
void QesCommand::aboutToFinish()
{
    clearMembers();
    mFinished = true;
    emit finished();
    emit finished(mResult);
}
