#include <QDebug>
#include <QEasyShell>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    
    qDebug() << "================";
    qDebug() << "QEasyShell test.";
    QesCommand env("env");
    QesResult *result = env.pipe("grep USER")->chain("pwd")->pipe("wc")->run();
    qDebug() << "Command is:" << env.command();

    qDebug() << "=======OUT======";
    qDebug() << result->toString();

    qDebug() << "=======ERR======";
    qDebug() << result->errorString();

    qDebug() << "================";
    qDebug() << "End.";

    return 0;
}
