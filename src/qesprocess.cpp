#include "qesprocess.h"

QesProcess::QesProcess(int index, QObject *parent) :
    QProcess(parent), m_index(index)
{
    m_error = false;

    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(prepareReadStdOut()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(prepareReadStdErr()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(setError(QProcess::ProcessError)));
}

bool QesProcess::isError()
{
    return m_error;
}

qint64 QesProcess::write(const QByteArray &data, int processIndex)
{
    Q_UNUSED(processIndex);
    return write(data.data());
}

void QesProcess::setError(QProcess::ProcessError error)
{
    Q_UNUSED(error);

    m_error = true;
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
