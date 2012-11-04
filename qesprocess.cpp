#include "qesprocess.h"

QesProcess::QesProcess(int index, QObject *parent) :
    QProcess(parent), m_index(index)
{
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(prepareReadStdOut()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(prepareReadStdErr()));
}

qint64 QesProcess::write(const QByteArray &data, int processIndex)
{
    Q_UNUSED(processIndex);
    return write(data.data());
}

void QesProcess::prepareReadStdOut()
{
    QByteArray toSend = readAllStandardOutput();
    emit readyReadStandardOutput(toSend, m_index);
}

void QesProcess::prepareReadStdErr()
{
    QByteArray toSend = readAllStandardError();
    emit readyReadStandardError(toSend, m_index);
}
