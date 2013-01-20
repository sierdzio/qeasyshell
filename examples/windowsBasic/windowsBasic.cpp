/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <QDebug>
#include <QEasyShell>

/*!
  \defgroup exampleWindowsBasic Basic Windows Example
  @{
  This example shows usage of QEasyShell on MS Windows.
  \snippet windowsBasic.cpp WindowsBasicExample
  */
//! [WindowsBasicExample]
int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    qDebug() << "================";
    qDebug() << "QEasyShell example - basic windows.";
    QesCommand command("dir", true);
    QesResult *result = command.chain("echo %CD%")->run();
    qDebug() << "Command is:" << command.command();

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
//! [WindowsBasicExample]

//! @}
