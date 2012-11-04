#ifndef QESSUBCOMMAND_H
#define QESSUBCOMMAND_H

#include <QtCore/QList>
#include <QtCore/QString>

#include "qesdefinitions.h"

/*!
  Class used internally by QesCommand to manage sub commands in pipes and chains.
  */
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

/*!
  Convenience typedef storing a list of QesSubCommands.
  */
typedef QList<QesSubCommand> CommandList;

#endif // QESSUBCOMMAND_H
