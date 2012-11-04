#ifndef QESPROCESS_H
#define QESPROCESS_H

#include <QtCore/QByteArray>
#include <QtCore/QProcess>

class QesProcess : public QProcess
{
    Q_OBJECT

public:
    explicit QesProcess(int index, QObject *parent = 0);
    using QProcess::write;
    
signals:
    void readyReadStandardOutput(const QByteArray &data, int processIndex);
    void readyReadStandardError(const QByteArray &data, int processIndex);
    
public slots:
    qint64 write(const QByteArray &data, int processIndex);

protected slots:
    void prepareReadStdOut();
    void prepareReadStdErr();
    
private:
    int m_index;
};

typedef QList<QesProcess *> ProcessList;

#endif // QESPROCESS_H
