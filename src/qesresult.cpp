#include "qesresult.h"
#include "qescommand.h"

/*!
  Initialises the result with an null QByteArray.

  \a parent parameter is optional.
  */
QesResult::QesResult(QObject *parent) :
    QObject(parent)
{
    m_rawOutput = QByteArray(); // a null QByteArray
}

/*!
  Sets the error to \a rawError. Discards any previous error data!

  \sa appendStdErr
  */
void QesResult::setError(const QByteArray &rawError)
{
    m_rawError = rawError;
}

/*!
  Returns the raw error output.
  */
QByteArray QesResult::errorRaw()
{
    return m_rawError;
}

/*!
  Returns error data as a string.
  */
QString QesResult::errorString()
{
    return QString(m_rawError);
}

/*!
  Returns the error data as a list of error lines.
  */
QStringList QesResult::errorStringList()
{
    return rawToStringList(m_rawError);
}

/*!
  Sets the raw result (from standard output) to \a rawData. Discards any previously
  stored raw data!

  \sa appendStdOut
  */
void QesResult::setRaw(const QByteArray &rawData)
{
    m_rawOutput = rawData;
}

/*!
  Returns raw output data.
  */
QByteArray QesResult::toRaw()
{
    return m_rawOutput;
}

/*!
  Returns output data as a string.
  */
QString QesResult::toString()
{
    return QString(m_rawOutput);
}

/*!
  Returns the output data as a list of consecutive lines.
  */
QStringList QesResult::toStringList()
{
    return rawToStringList(m_rawOutput);
}

/*!
  Sets result's object validity to \a validity.
  */
void QesResult::setValid(bool validity)
{
    m_isValid = validity;
    if (validity == false)
        m_rawOutput = QByteArray(); // a null QByteArray
}

/*!
  Returns true if the result object is valid.
  */
bool QesResult::isValid()
{
    return m_isValid;
}

/*!
  Appends given data (\a stdOut) to the raw data.

  \sa setRaw
  */
void QesResult::appendStdOut(const QByteArray &stdOut)
{
    m_rawOutput.append(stdOut);
}

/*!
  Appends given data (\a stdErr) to the error data.

  \sa setError
  */
void QesResult::appendStdErr(const QByteArray &stdErr)
{
    m_rawError.append(stdErr);
}

/*!
  Converts a stream of raw data into a string list (where each entry represents
  one line of the output).
  */
QStringList QesResult::rawToStringList(const QByteArray &rawData)
{
    QStringList result;
    QList<QByteArray> temp = rawData.split('\n');

    foreach (const QByteArray &ba, temp) {
        result.append(QString(ba));
    }

    return result;
}
