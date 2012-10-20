#ifndef QESCOMMAND_H
#define QESCOMMAND_H

#include "QEasyShell_global.h"
#include "qesdefinitions.h"
#include "qesresult.h"

#include <QtCore/QObject>

class QEASYSHELLSHARED_EXPORT QesCommand : public QObject
{
    Q_OBJECT

public:
    explicit QesCommand(const QString &command, Qes::Shell shell = Qes::Bash, QObject *parent = 0);

    QesResult run();
};

#endif // QESCOMMAND_H
