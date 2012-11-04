#include "qesresult.h"
#include "qescommand.h"

QesResult::QesResult(QObject *parent) :
    QObject(parent)
{
    m_rawOutput = QByteArray(); // a null QByteArray
}

void QesResult::setError(const QByteArray &rawError)
{
    m_rawError = rawError;
}

QByteArray QesResult::errorRaw()
{
    return m_rawError;
}

QString QesResult::errorString()
{
    return QString(m_rawError);
}

QStringList QesResult::errorStringList()
{
    return rawToStringList(m_rawError);
}

void QesResult::setRaw(const QByteArray &rawData)
{
    m_rawOutput = rawData;
}

QByteArray QesResult::toRaw()
{
    return m_rawOutput;
}

QString QesResult::toString()
{
    return QString(m_rawOutput);
}

QStringList QesResult::toStringList()
{
    return rawToStringList(m_rawOutput);
}

void QesResult::setValid(bool validity)
{
    m_isValid = validity;
    if (validity == false)
        m_rawOutput = QByteArray(); // a null QByteArray
}

bool QesResult::isValid()
{
    return m_isValid;
}

void QesResult::appendStdOut(const QByteArray &stdOut)
{
    m_rawOutput.append(stdOut);
}

void QesResult::appendStdErr(const QByteArray &stdErr)
{
    m_rawError.append(stdErr);
}

QStringList QesResult::rawToStringList(const QByteArray &rawData)
{
    QStringList result;
    QList<QByteArray> temp = rawData.split('\n');

    foreach (const QByteArray &ba, temp) {
        result.append(QString(ba));
    }

    return result;
}
