/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QESSUBCOMMAND_H
#define QESSUBCOMMAND_H

#include <QtCore/QList>
#include <QtCore/QString>

#include "qesdefinitions.h"

/*!
  \brief A small private class used internally for storing managing command chains.

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
  \relates QesSubCommand

  Convenience typedef storing a list of QesSubCommands.
  */
typedef QList<QesSubCommand> CommandList;

#endif // QESSUBCOMMAND_H
