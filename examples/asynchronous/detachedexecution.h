#ifndef DETACHEDEXECUTION_H
#define DETACHEDEXECUTION_H

#include <QObject>

class DetachedExecution : public QObject
{
    Q_OBJECT
public:
    explicit DetachedExecution(QObject *parent = 0);
    void exec();
};

#endif // DETACHEDEXECUTION_H
