#ifndef QESPROCESS_H
#define QESPROCESS_H

#include <QtCore/QByteArray>
#include <QtCore/QProcess>

class QesProcess : public QProcess
{
    Q_OBJECT

public:
    explicit QesProcess(int index, QObject *parent = 0);
    bool isError();
    using QProcess::write;
    
signals:
    void readyReadStandardOutput(const QByteArray &data, int processIndex);
    void readyReadStandardError(const QByteArray &data, int processIndex);
    
public slots:
    qint64 write(const QByteArray &data, int processIndex);
    void setError(QProcess::ProcessError error);

protected slots:
    void prepareReadStdOut();
    void prepareReadStdErr();
    
private:
    int m_index;
    bool m_error;
};

typedef QList<QesProcess *> ProcessList;

#endif // QESPROCESS_H
