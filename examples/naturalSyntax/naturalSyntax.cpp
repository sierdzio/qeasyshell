/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <QDebug>
#include <QEasyShell>

/*!
  \defgroup exampleNaturalSyntax Natural Syntax Example
  @{
  Shows how natural (bash-like) syntax can be used in QEasyShell. It has some
  limitations, but can be used in many situations.
  \snippet naturalSyntax.cpp naturalExample
  */

//! [naturalExample]
int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    
    qDebug() << "================";
    qDebug() << "QEasyShell example - natural syntax.";
    QesCommand env("env");
    QesResult *result = (env | ("grep USER"))->chain("pwd")->pipe("wc")->run();
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
//! [naturalExample]
//! @}
