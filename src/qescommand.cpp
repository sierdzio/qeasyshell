/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "qescommand.h"

/*!
  Initialises the command object with it's first \a command string.
  */
QesCommand::QesCommand(const QString &command,
                       QObject *parent) :
    QObject(parent)
{
    m_finished = false;
    m_commands.append(QesSubCommand(command));
    m_result = NULL;
    m_currentCommandIndex = -1;
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
  An overload taking another QesCommand object.
  */
QesCommand *QesCommand::pipe(QesCommand *command)
{
    CommandList temp = command->commandList();
    QesSubCommand tempsub = temp.takeAt(0);
    // This is required because the first subcommand is usually set to Qes::None
    tempsub.setPipeline(Qes::Pipe);
    m_commands.append(tempsub);
    m_commands.append(temp);
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
  An overload taking another QesCommand object.
  */
QesCommand *QesCommand::chain(QesCommand *command)
{
    CommandList temp = command->commandList();
    QesSubCommand tempsub = temp.takeAt(0);
    // This is required because the first subcommand is usually set to Qes::None
    tempsub.setPipeline(Qes::Chain);
    m_commands.append(tempsub);
    m_commands.append(temp);
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
        processList.append(new QesProcess(i, this));

        QesProcess *current = processList.at(i);
        QesProcess *previous = (i != 0)? processList.at(i - 1) : 0;

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
            if (i > 0)
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

    m_result = result;
    emit finished(result);
    return result;
}

/*!
  Same as run, but asynchronous - the method returns immediately and executes in
  the background. In order to get the result, you need to wait for signals it
  will emit once finished.

  \sa run, pipe, chain
  */
void QesCommand::runDetached(const QByteArray &input)
{
    Q_UNUSED(input);
    m_processList.clear();
    m_currentCommandIndex = 0;
    this->metaObject()->invokeMethod(this, "processNextStep",
                                     Q_ARG(int, 0));
//    processNextStep(0, QProcess::NormalExit);
    return;
}

QesResult *QesCommand::result()
{
    return m_result;
}

bool QesCommand::isResultReady()
{
    return m_finished;
}

bool QesCommand::isFinished()
{
    return m_finished;
}

void QesCommand::processNextStep(int pid, QProcess::ExitStatus pes)
{
    Q_UNUSED(pid);

    if (m_result == NULL)
        m_result = new QesResult(this);

    if (pes == QProcess::CrashExit) {
        // TODO: ERROR!
        m_result->setError(QString("Process PID: " + QString::number(pid) + "failed!").toLocal8Bit());
        return;
    }

    for(int i = m_currentCommandIndex; i < m_commands.length(); ++i) {
        bool takeABreak = false;
        m_currentCommandIndex = i;
        Qes::Pipeline pipeline = m_commands.at(i).pipeline();
        m_processList.append(new QesProcess(i, this));

        QesProcess *current = m_processList.at(i);
        QesProcess *previous = (i != 0)? m_processList.at(i - 1) : 0;

        if (pipeline == Qes::None || pipeline == Qes::Pipe) {
            if (i != 0) {
                // TODO: this code is important in chain, too! What if chain is the last command, eh?
                if ((i < (m_commands.length() - 1) && m_commands.at(i + 1).pipeline() == Qes::Chain)
                        ||(i == (m_commands.length() - 1))) {
                    connectOutputs(current, m_result);                    
                }

                previous->setStandardOutputProcess(current);
                previous->start(m_commands.at(i - 1).command());
            }
        } else if (pipeline == Qes::Chain) {
            previous->start(m_commands.at(i - 1).command());
            connectOutputs(current, m_result);
            connect(previous, SIGNAL(finished(int, QProcess::ExitStatus)),
                    this, SLOT(processNextStep(int, QProcess::ExitStatus)));
            takeABreak = true;
        }

        if (i == (m_commands.length() - 1)) {
            current->start(m_commands.at(i).command());
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
QString QesCommand::command()
{
    QString result;

    foreach (const QesSubCommand &s, m_commands) {
        result += s.toString();
    }

    return result;
}

/*!
  Returns the internal list of commands. Not very useful for end users, but
  important in chaining subcommands.
  */
CommandList QesCommand::commandList()
{
    return m_commands;
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

void QesCommand::aboutToFinish()
{
//    foreach (QesProcess *process, m_processList) {
//        process->deleteLater();
//    }
//    m_processList.clear();

    m_finished = true;
    emit finished();
    emit finished(m_result);
}
