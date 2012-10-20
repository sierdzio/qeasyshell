#ifndef QESCOMMAND_H
#define QESCOMMAND_H

#include "QEasyShell_global.h"
#include "qesdefinitions.h"
#include "qesresult.h"

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>

class QEASYSHELLSHARED_EXPORT QesCommand : public QObject
{
    Q_OBJECT

public:
    explicit QesCommand(const QString &command,
                        const QStringList &arguments = QStringList(),
                        Qes::Shell shell = Qes::Bash,
                        QObject *parent = 0);

    QesResult *run(const QByteArray &input = QByteArray());
    QesResult *runDetached();

private:
    QString m_command;
    QStringList m_arguments;
    Qes::Shell m_shell;
};

#endif // QESCOMMAND_H
