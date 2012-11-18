#include "detachedexecution.h"

#include <QDebug>
#include <QEasyShell>

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
    qDebug() << "Quick result before run finishes:" << env.result()->toString();
    qDebug() << "Command is:" << env.command();

//    forever {
//        if (!env.isFinished())
//            app.processEvents();
//        else
//            break;
//    }

    qDebug() << "=======OUT======";
    qDebug() << env.result()->toString();

    qDebug() << "=======ERR======";
    qDebug() << env.result()->errorString();

    qDebug() << "================";
    qDebug() << "End.";
}
