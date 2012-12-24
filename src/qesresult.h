/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QESRESULT_H
#define QESRESULT_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "qeasyshellglobal.h"
#include "qesdefinitions.h"

class QesCommand;

/*!
  \brief Returned as result of QesCommand execution, provides access to stdout
  and stderr.

  Holds all the results of running the QesCommand, including errors if any
  were reported.
  */
class QEASYSHELLSHARED_EXPORT QesResult : public QObject
{
    Q_OBJECT

public:
    explicit QesResult(QObject *parent = 0);

    void setStdErr(const QByteArray &rawError);
    QByteArray stdErrRaw();
    QString stdErrString();
    QStringList stdErrStringList();

    void setStdOut(const QByteArray &rawData);
    QByteArray stdOutRaw();
    QString stdOutString();
    QStringList stdOutStringList();

    void setValid(bool validity);
    bool isValid();

public slots:
    void appendStdOut(const QByteArray &stdOutRaw);
    void appendStdErr(const QByteArray &stdErr);

private:
    QStringList rawToStringList(const QByteArray &rawData);

    bool m_isValid;
    QByteArray m_rawStdOut;
    QByteArray m_rawStdErr;
};

#endif // QESRESULT_H
