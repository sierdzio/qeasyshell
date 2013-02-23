/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "qesprocess.h"

/*!
  Instantiates the QesProcess object with \a index and optional \a parent.
  */
QesProcess::QesProcess(int index, QObject *parent) :
    QProcess(parent), mIndex(index)
{
    mError = false;

    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(prepareReadStdOut()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(prepareReadStdErr()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(setError(QProcess::ProcessError)));
}

/*!
  Returns true if the process encountered an error.

  \sa errorString, setError
  */
bool QesProcess::isError() const
{
    return mError;
}

/*!
  Returns a string with nice description of error enum code.

  \sa isError, setError
 */
QString QesProcess::errorString() const
{
    QString result;

    if (!isError())
        return result;

    switch (error()) {
    case FailedToStart:
        result = "Could not find binary or insufficient permissions.";
        break;
    case Crashed:
        result = "Process crashed.";
        break;
    case Timedout:
        result = "Last waitFor*() call timed out.";
        break;
    case WriteError:
        result = "Could not write to process.";
        break;
    case ReadError:
        result = "Could not read from process.";
        break;
    case UnknownError:
        result = "QesProcess encountered an unknown error.";
        break;
    default:
        break;
    }

    return result;
}

/*!
  Writes \a data to the process (stdin). Requires \a processIndex for internal
  reasons.
  */
qint64 QesProcess::write(const QByteArray &data, int processIndex)
{
    Q_UNUSED(processIndex);
    return write(data.data());
}

/*!
  Sets internal error state to true. \a error parameter is ignored.

  \sa isError, errorString
  */
void QesProcess::setError(QProcess::ProcessError error)
{
    Q_UNUSED(error);

    mError = true;
}

/*!
  Emitts readyReadStandardOutput(QByteArray, int) signal with all the needed datas.
  */
void QesProcess::prepareReadStdOut()
{
    QByteArray toSend = readAllStandardOutput();
    emit readyReadStandardOutput(toSend, mIndex);
}

/*!
  Emitts readyReadStandardError(QByteArray, int) signal with all the needed datas.
  */
void QesProcess::prepareReadStdErr()
{
    QByteArray toSend = readAllStandardError();
    emit readyReadStandardError(toSend, mIndex);
}
