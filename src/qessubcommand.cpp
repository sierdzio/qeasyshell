/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "qessubcommand.h"

/*!
  Initialises the object with \a command to run, and \a pipe to use.
  */
QesSubCommand::QesSubCommand(const QString &command, Qes::Pipeline pipe) :
    m_command(command), m_pipeline(pipe)
{
}

/*!
  Returns a string representing how this command would be invoked if shell was used.
  */
QString QesSubCommand::toString() const
{
    return QString(Qes::pipelineToString(m_pipeline) + m_command);
}

/*!
  Returns the pipeline in use for this subcommand.
  */
Qes::Pipeline QesSubCommand::pipeline() const
{
    return m_pipeline;
}

/*!
  Returns the command string.
  */
QString QesSubCommand::command() const
{
    return m_command;
}
