/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <QDebug>
#include <QEasyShell>

/*!
  \defgroup exampleInputData Input Data Example
  @{
  Shows how external data can be provided to prime first command in chain.
  \snippet inputData.cpp inputExample
  */

//! [inputExample]
int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    qDebug() << "================";
    qDebug() << "QEasyShell example - input data.";
    QesCommand env("wc");
    QesResult *result = env.run("All of your base are belong to us.");
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
//! [inputExample]
//! @}
