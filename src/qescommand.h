/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QESCOMMAND_H
#define QESCOMMAND_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QStringList>

#include "qeasyshellglobal.h"
#include "qesdefinitions.h"
#include "qesresult.h"
#include "qessubcommand.h"
#include "qesprocess.h"

/*!
  \brief Base class used to manage command chaining and execution.

  This class is used to manage shell command executions, and creating command
  chains (or pipelines). Easily the most importand class to look at in the whole
  project.

  You can run commands synchronously using run(). This method will not return
  until all chained commands finish and the result is ready. That makes it easy to
  use, but may cause the UI to become unresponsive if commands take a lot of time
  to execute.

  In that case, you can use runDetached() which will return immediately and execute
  all commands in the background. Once the result is ready, it will emit finished()
  and the output will be ready under result().

  Result object is returned as a pointer, and QEasyShell will never destroy it.
  You need to delete it when you see fit.

  Command list is not cleared after execution - you can rerun QesCommand at will.
  To clear command list, use QesCommand::clear() method.

  See QES examples to see it in action, here is just a tiny example of synchronous
  run:
  \verbatim
  QesResult *r= QesCommand("env").pipe("grep USER")->chain("pwd")->pipe("wc")->run();
  printf(qPrintable(r->toString()), NULL);
  \endverbatim

  QesCommand also has a Windows compatibility mode. You can turn it on in
  the constructor. If that flag is set to true, QES will try to make certain
  commands work on Windows by adding cmd.exe invocation. This feature is quite experimental
  at the moment.
  */
class QEASYSHELLSHARED_EXPORT QesCommand : public QObject
{
    Q_OBJECT

public:
    explicit QesCommand(const QString &command,
                        bool windowsCompatibility = false,
                        QObject *parent = 0);

    QesCommand *pipe(const QString &command);
    QesCommand *pipe(QesCommand *command);
    QesCommand *operator |(QesCommand *command);
    QesCommand *operator |(const QString &command);

    QesCommand *chain(const QString &command);
    QesCommand *chain(QesCommand *command);

    QesCommand *redirect(const QString &destination,
                         Qes::Pipeline redirectionMode = Qes::Redirect,
                         QProcess::ProcessChannel channel = QProcess::StandardOutput);
    QesCommand *operator >>(const QString &destination);


    QesResult *run(const QByteArray &input = QByteArray());
    void runDetached(const QByteArray &input = QByteArray());
    void terminateAll();
    void killAll();

    QesResult *result() const;
    bool isResultReady() const;
    bool isFinished() const;

    QString command() const;
    CommandList commandList() const;
    void clear();

signals:
    /*!
      Unused.
     */
    void finishedStep(QesResult *result);

    /*!
      Emitted when the command has stopped running. Passes a pointer to result
      object.

      \sa result
     */
    void finished(QesResult *);

    /*!
      Emitted when the command has stopped running.

      \sa result
     */
    void finished();

protected slots:
    void processNextStep(int pid, QProcess::ExitStatus pes = QProcess::NormalExit,
                         const QByteArray &input = QByteArray());
    void aboutToFinish();

private:
    void connectOutputs(QesProcess *process, QesResult *result);
    bool redirectToFile(const QString &filename, const QByteArray &data,
                        Qes::Pipeline pipe = Qes::Redirect);
    QString prepareCommand(const QString &command, bool windowsMode = false);
    void clearMembers();

    bool mFinished;
    bool mWindowsCompatibility;
    int mCurrentCommandIndex;
    CommandList mCommands;
    ProcessList mProcessList;
    QesResult *mResult;
};

#endif // QESCOMMAND_H
