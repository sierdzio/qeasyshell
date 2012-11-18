/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QESCOMMAND_H
#define QESCOMMAND_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>

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
  */
class QEASYSHELLSHARED_EXPORT QesCommand : public QObject
{
    Q_OBJECT

public:
    explicit QesCommand(const QString &command,
                        QObject *parent = 0);

    QesCommand *pipe(const QString &command);
    QesCommand *pipe(QesCommand *command);
    QesCommand *chain(const QString &command);
    QesCommand *chain(QesCommand *command);

    QesResult *run(const QByteArray &input = QByteArray());
    void runDetached(const QByteArray &input = QByteArray());
    QesResult *result();

    QString command();
    CommandList commandList();

signals:
    // TODO: evaluate and start using!
    void finishedStep(QesResult *result);
    void finished(QesResult *);
    void finished();

protected slots:
    void processNextStep(int pid, QProcess::ExitStatus pes);

private:
    void connectOutputs(QesProcess *process, QesResult *result);
    void aboutToFinish();

    int m_currentCommandIndex;
    CommandList m_commands;
    ProcessList m_processList;
    QesResult *m_result;
};

#endif // QESCOMMAND_H
