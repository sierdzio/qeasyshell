#ifndef QESSUBCOMMAND_H
#define QESSUBCOMMAND_H

#include <QtCore/QList>
#include <QtCore/QString>
#include "qesdefinitions.h"

class QesSubCommand
{
public:
    QesSubCommand(const QString &command, Qes::Pipeline pipe = Qes::None);
    QString toString() const;

    Qes::Pipeline pipeline() const;
    QString command() const;

private:
    QString m_command;
    Qes::Pipeline m_pipeline;
};

typedef QList<QesSubCommand> CommandList;

#endif // QESSUBCOMMAND_H
