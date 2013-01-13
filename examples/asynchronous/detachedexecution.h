#ifndef DETACHEDEXECUTION_H
#define DETACHEDEXECUTION_H

#include <QObject>

/*!
  \ingroup exampleAsynchronous
  @{
  Class needed because even loop is required for this example.
  */

//! [AsyncClassHeaderExample]
class DetachedExecution : public QObject
{
    Q_OBJECT
public:
    explicit DetachedExecution(QObject *parent = 0);
    Q_INVOKABLE void exec();
};
//! [AsyncClassHeaderExample]
//! @}

#endif // DETACHEDEXECUTION_H
