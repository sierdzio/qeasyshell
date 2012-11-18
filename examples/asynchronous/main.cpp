/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <QDebug>
#include <QEasyShell>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    qDebug() << "================";
    qDebug() << "QEasyShell example - asynchronous API usage.";
    QesCommand env("env");
    env.pipe("grep USER")->chain("pwd")->pipe("wc")->runDetached();
    qDebug() << "Quick result before run finishes:" << env.result()->toString();
    qDebug() << "Command is:" << env.command();

    qDebug() << "=======OUT======";
    qDebug() << env.result()->toString();

    qDebug() << "=======ERR======";
    qDebug() << env.result()->errorString();

    qDebug() << "================";
    qDebug() << "End.";

    return 0;
}
