#ifndef QESCOMMAND_H
#define QESCOMMAND_H

#include "qeasyshellglobal.h"
#include "qesdefinitions.h"
#include "qesresult.h"
#include "qessubcommand.h"
#include "qesprocess.h"

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>

class QEASYSHELLSHARED_EXPORT QesCommand : public QObject
{
    Q_OBJECT

public:
    explicit QesCommand(const QString &command,
                        Qes::Shell shell = Qes::Bash,
                        QObject *parent = 0);

    QesCommand *pipe(const QString &command);
    QesCommand *chain(const QString &command);

    QesResult *run(const QByteArray &input = QByteArray());
    QesResult *runDetached();

    QString command();
    Qes::Shell shell();

private:
    Qes::Shell m_shell;

    CommandList m_commands;
};

#endif // QESCOMMAND_H
