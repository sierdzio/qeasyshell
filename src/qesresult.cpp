#include "qesresult.h"
/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "qescommand.h"

/*!
  Initialises the result with an null QByteArray.

  \a parent parameter is optional.
  */
QesResult::QesResult(QObject *parent) :
    QObject(parent)
{
    m_rawStdOut = QByteArray(); // a null QByteArray
}

/*!
  Sets the error to \a rawError. Discards any previous error data!

  \sa appendStdErr
  */
void QesResult::setStdErr(const QByteArray &rawError)
{
    m_rawStdErr = rawError;
}

/*!
  Returns the raw error output.
  */
QByteArray QesResult::stdErrRaw() const
{
    return m_rawStdErr;
}

/*!
  Returns error data as a string.
  */
QString QesResult::stdErrString() const
{
    return QString(m_rawStdErr);
}

/*!
  Returns the error data as a list of error lines.
  */
QStringList QesResult::stdErrStringList() const
{
    return rawToStringList(m_rawStdErr);
}

/*!
  Sets the raw result (from standard output) to \a rawData. Discards any previously
  stored raw data!

  \sa appendStdOut
  */
void QesResult::setStdOut(const QByteArray &rawData)
{
    m_rawStdOut = rawData;
}

/*!
  Returns raw output data.
  */
QByteArray QesResult::stdOutRaw() const
{
    return m_rawStdOut;
}

/*!
  Returns output data as a string.
  */
QString QesResult::stdOutString() const
{
    return QString(m_rawStdOut);
}

/*!
  Returns the output data as a list of consecutive lines.
  */
QStringList QesResult::stdOutStringList() const
{
    return rawToStringList(m_rawStdOut);
}

/*!
  Sets the progress error to \a message. This clears all previous values.

  \sa progressError, appendProgressError
 */
void QesResult::setProgressError(const QString &message)
{
    m_progressError = message;
}

/*!
  Returns the progress error. Progress error is not stdErr. It is used to report
  problems in running the command itself (like "Command not found", "Permissions
  denied", etc.)

  \sa stdErrRaw, setProgressError, appendProgressError
 */
QString QesResult::progressError() const
{
    return m_progressError;
}

/*!
  Sets result's object validity to \a validity.
  */
void QesResult::setValid(bool validity)
{
    m_isValid = validity;
    if (validity == false)
        m_rawStdOut = QByteArray(); // a null QByteArray
}

/*!
  Returns true if the result object is valid.
  */
bool QesResult::isValid() const
{
    return m_isValid;
}

/*!
  Appends given data (\a stdOut) to the raw data.

  \sa setRaw
  */
void QesResult::appendStdOut(const QByteArray &stdOut)
{
    m_rawStdOut.append(stdOut);
}

/*!
  Appends given data (\a stdErr) to the error data.

  \sa setError
  */
void QesResult::appendStdErr(const QByteArray &stdErr)
{
    m_rawStdErr.append(stdErr);
}

/*!
  Appends \a message to progress error variable. It does not add any separation
  to the previous message (no space, no new line).
 */
void QesResult::appendProgressError(const QString &message)
{
    m_progressError.append(message);
}

/*!
  Converts a stream of raw data into a string list (where each entry represents
  one line of the output).
  */
QStringList QesResult::rawToStringList(const QByteArray &rawData) const
{
    QStringList result;
    QList<QByteArray> temp = rawData.split('\n');

    foreach (const QByteArray &ba, temp) {
        result.append(QString(ba));
    }

    return result;
}
