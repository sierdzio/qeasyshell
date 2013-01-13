/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <QDebug>
#include <QEasyShell>

/*!
  \defgroup exampleCombining Combining Commands Example
  @{
  This example shows how different command objects can be combined to form a chain.
  \snippet combiningCommands.cpp ccExample
  */

//! [ccExample]
int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    qDebug() << "================";
    qDebug() << "QEasyShell example - combining commands.";
    QesCommand env("env");
    QesCommand *pwdWc = new QesCommand("pwd");
    pwdWc->pipe("wc");
    QesResult *result = env.pipe("grep USER")->chain(pwdWc)->run();
    qDebug() << "Command is:" << env.command();

    qDebug() << "=======OUT======";
    qDebug() << result->stdOutString();

    qDebug() << "=======ERR======";
    qDebug() << result->stdErrString();

    qDebug() << "==PROGRESS ERR==";
    qDebug() << result->progressError();

    qDebug() << "================";
    qDebug() << "End.";

    return 0;
}
//! [ccExample]
//! @}
