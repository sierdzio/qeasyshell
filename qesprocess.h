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
    void readyReadStandardOutput(int processIndex, const QByteArray &data);
    
public slots:
    qint64 write(int processIndex, const QByteArray &data);

protected slots:
    void prepareReadData();
    
private:
    int m_index;
};

typedef QList<QesProcess *> ProcessList;

#endif // QESPROCESS_H
