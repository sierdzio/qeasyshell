/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <QIODevice>
#include <QFile>
#include <QDebug>
#include <QEasyShell>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    qDebug() << "================";
    qDebug() << "QEasyShell example - redirection.";
    QesCommand truncate("env");
    QesResult *resultTruncate = truncate.pipe("grep USER")->redirect("result.txt")->pipe("wc")->run();

    QesCommand append("env");
    QesResult *resultAppend = append.pipe("grep USER")->redirect("result.txt", Qes::RedirectAppend)->pipe("wc")->run();
    qDebug() << "Truncate command is:" << truncate.command();
    qDebug() << "Append command is:" << append.command();

    qDebug() << "=======OUT======";
    qDebug() << "Truncate:";
    qDebug() << resultTruncate->stdOutString();
    qDebug() << "Append:";
    qDebug() << resultAppend->stdOutString();

    qDebug() << "=======ERR======";
    qDebug() << "Truncate:";
    qDebug() << resultTruncate->stdErrString();
    qDebug() << "Append:";
    qDebug() << resultAppend->stdErrString();

    qDebug() << "==PROGRESS ERR==";
    qDebug() << "Truncate:";
    qDebug() << resultTruncate->progressError();
    qDebug() << "Append:";
    qDebug() << resultAppend->progressError();

    qDebug() << "================";
    qDebug() << "=Resulting file=";
    QFile file("result.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    qDebug() << file.readAll();
    file.close();
    qDebug() << "End.";

    return 0;
}
