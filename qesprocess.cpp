#include "qesprocess.h"

QesProcess::QesProcess(int index, QObject *parent) :
    QProcess(parent), m_index(index)
{
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(prepareReadData()));
}

qint64 QesProcess::write(int processIndex, const QByteArray &data)
{
    Q_UNUSED(processIndex);
    return write(data.data());
}

void QesProcess::prepareReadData()
{
    QByteArray toSend = read(bytesAvailable());
    emit readyReadStandardOutput(m_index, toSend);
}
