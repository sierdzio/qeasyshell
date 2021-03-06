/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QESRESULT_H
#define QESRESULT_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QStringList>

#include "qeasyshellglobal.h"
#include "qesdefinitions.h"

class QesCommand;

/*!
  \brief Returned as result of QesCommand execution, provides access to data
         regarding the outcome of the QesCommand operation..

  Holds all the results of running the QesCommand, including errors if any
  were reported.

  3 main channels are available: stdOut (which holds standard console output of
  all processes that were run by QesCommand), stdErr (standard error data reported
  by processes), and progressError (provides metadata errors - failure to run
  any of the QesCommands in chain, process crash, etc).
  */
class QEASYSHELLSHARED_EXPORT QesResult : public QObject
{
    Q_OBJECT

public:
    explicit QesResult(QObject *parent = 0);

    void setStdErr(const QByteArray &rawError);
    QByteArray stdErrRaw() const;
    QString stdErrString() const;
    QStringList stdErrStringList() const;

    void setStdOut(const QByteArray &rawData);
    QByteArray stdOutRaw() const;
    QString stdOutString() const;
    QStringList stdOutStringList() const;

    void setProgressError(const QString &message);
    QString progressError() const;

    void setValid(bool validity);
    bool isValid() const;

public slots:
    void appendStdOut(const QByteArray &stdOutRaw);
    void appendStdErr(const QByteArray &stdErr);
    void appendProgressError(const QString &message, bool addNewLine = true);

private:
    QStringList rawToStringList(const QByteArray &rawData) const;

    bool mIsValid;
    QByteArray mRawStdOut;
    QByteArray mRawStdErr;
    QString mProgressError;
};

#endif // QESRESULT_H
