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
    mRawStdOut = QByteArray(); // a null QByteArray
}

/*!
  Sets the error to \a rawError. Discards any previous error data!

  \sa appendStdErr
  */
void QesResult::setStdErr(const QByteArray &rawError)
{
    mRawStdErr = rawError;
}

/*!
  Returns the raw error output.
  */
QByteArray QesResult::stdErrRaw() const
{
    return mRawStdErr;
}

/*!
  Returns error data as a string.
  */
QString QesResult::stdErrString() const
{
    return QString(mRawStdErr);
}

/*!
  Returns the error data as a list of error lines.
  */
QStringList QesResult::stdErrStringList() const
{
    return rawToStringList(mRawStdErr);
}

/*!
  Sets the raw result (from standard output) to \a rawData. Discards any previously
  stored raw data!

  \sa appendStdOut
  */
void QesResult::setStdOut(const QByteArray &rawData)
{
    mRawStdOut = rawData;
}

/*!
  Returns raw output data.
  */
QByteArray QesResult::stdOutRaw() const
{
    return mRawStdOut;
}

/*!
  Returns output data as a string.
  */
QString QesResult::stdOutString() const
{
    return QString(mRawStdOut);
}

/*!
  Returns the output data as a list of consecutive lines.
  */
QStringList QesResult::stdOutStringList() const
{
    return rawToStringList(mRawStdOut);
}

/*!
  Sets the progress error to \a message. This clears all previous values.

  \sa progressError, appendProgressError
 */
void QesResult::setProgressError(const QString &message)
{
    mProgressError = message;
}

/*!
  Returns the progress error. Progress error is not stdErr. It is used to report
  problems in running the command itself (like "Command not found", "Permissions
  denied", etc.)

  \sa stdErrRaw, setProgressError, appendProgressError
 */
QString QesResult::progressError() const
{
    return mProgressError;
}

/*!
  Sets result's object validity to \a validity.
  */
void QesResult::setValid(bool validity)
{
    mIsValid = validity;
    if (validity == false)
        mRawStdOut = QByteArray(); // a null QByteArray
}

/*!
  Returns true if the result object is valid.
  */
bool QesResult::isValid() const
{
    return mIsValid;
}

/*!
  Appends given data (\a stdOut) to the raw data.

  \sa setRaw
  */
void QesResult::appendStdOut(const QByteArray &stdOut)
{
    mRawStdOut.append(stdOut);
}

/*!
  Appends given data (\a stdErr) to the error data.

  \sa setError
  */
void QesResult::appendStdErr(const QByteArray &stdErr)
{
    mRawStdErr.append(stdErr);
}

/*!
  Appends \a message to progress error variable. It does not add any separation
  to the previous message (no space, no new line).
 */
void QesResult::appendProgressError(const QString &message, bool addNewLine)
{
    mProgressError.append(message + QString((addNewLine)? "\n" : ""));
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
