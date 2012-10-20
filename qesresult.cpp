#include "qesresult.h"
#include "qescommand.h"

QesResult::QesResult(QObject *parent) :
    QObject(parent)
{
    m_rawOutput = QByteArray(); // a null QByteArray
}

QesResult QesResult::pipe(const QesCommand &command)
{
    QesResult result;

    if (m_rawOutput.isNull()) {
        result.setValid(false);
        return result;
    }

    return command.run(m_rawOutput);
}

QesResult QesResult::chain(const QesCommand &command)
{
    return command.run();
}

void QesResult::setRaw(QByteArray rawData)
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
    QStringList result;
    QList<QByteArray> temp = m_rawOutput.split('\n');

    foreach (const QByteArray &ba, temp) {
        result.append(QString(ba));
    }

    return result;
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
