#include "qesprocess.h"

/*!
  Instantiates the QesProcess object with \a index and optional \a parent.
  */
QesProcess::QesProcess(int index, QObject *parent) :
    QProcess(parent), m_index(index)
{
    m_error = false;

    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(prepareReadStdOut()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(prepareReadStdErr()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(setError(QProcess::ProcessError)));
}

/*!
  Returns true if the process encountered an error.
  */
bool QesProcess::isError()
{
    return m_error;
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
  */
void QesProcess::setError(QProcess::ProcessError error)
{
    Q_UNUSED(error);

    m_error = true;
}

/*!
  Emitts readyReadStandardOutput(QByteArray, int) signal with all the needed datas.
  */
void QesProcess::prepareReadStdOut()
{
    QByteArray toSend = readAllStandardOutput();
    emit readyReadStandardOutput(toSend, m_index);
}

/*!
  Emitts readyReadStandardError(QByteArray, int) signal with all the needed datas.
  */
void QesProcess::prepareReadStdErr()
{
    QByteArray toSend = readAllStandardError();
    emit readyReadStandardError(toSend, m_index);
}
