#ifndef QESRESULT_H
#define QESRESULT_H

#include "QEasyShell_global.h"
#include "qesdefinitions.h"
#include "qescommand.h"

#include <QtCore/QObject>

class QEASYSHELLSHARED_EXPORT QesResult : public QObject
{
    Q_OBJECT
public:
    explicit QesResult(QObject *parent = 0);
    QesResult pipe(QesCommand command);
    QesResult chain(QesCommand command);

    QByteArray toRaw();
    QString toString();
    QStringList toStringList();
};

#endif // QESRESULT_H
