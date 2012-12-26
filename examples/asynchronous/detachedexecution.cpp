#include "detachedexecution.h"

#include <QDebug>
#include <QEasyShell>
#include <QCoreApplication>

DetachedExecution::DetachedExecution(QObject *parent) :
    QObject(parent)
{
}

void DetachedExecution::exec()
{
    qDebug() << "================";
    qDebug() << "QEasyShell example - asynchronous API usage.";
    QesCommand env("env");
    env.pipe("grep USER")->chain("pwd")->pipe("wc")->runDetached();
    qDebug() << "Quick result before run finishes:" << env.result()->stdOutString();
    qDebug() << "Command is:" << env.command();

    forever {
        if (!env.isFinished())
            QCoreApplication::instance()->processEvents();
        else
            break;
    }

    qDebug() << "=======OUT======";
    qDebug() << env.result()->stdOutString();

    qDebug() << "=======ERR======";
    qDebug() << env.result()->stdErrString();

    qDebug() << "==PROGRESS ERR==";
    qDebug() << env.result()->progressError();

    qDebug() << "================";
    qDebug() << "End.";
    QCoreApplication::quit();
}
