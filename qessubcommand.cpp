#include "qessubcommand.h"

QesSubCommand::QesSubCommand(const QString &command, Qes::Pipeline pipe) :
    m_command(command), m_pipeline(pipe)
{
}

QString QesSubCommand::toString()
{
    return QString(Qes::pipelineToString(m_pipeline) + m_command);
}

Qes::Pipeline QesSubCommand::pipeline() const
{
    return m_pipeline;
}

QString QesSubCommand::command() const
{
    return m_command;
}
