#ifndef QESRESULT_H
#define QESRESULT_H

#include "qeasyshellglobal.h"
#include "qesdefinitions.h"

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>

class QesCommand;

class QEASYSHELLSHARED_EXPORT QesResult : public QObject
{
    Q_OBJECT

public:
    explicit QesResult(QObject *parent = 0);

    void setError(const QByteArray &rawError);
    QByteArray errorRaw();
    QString errorString();
    QStringList errorStringList();

    void setRaw(const QByteArray &rawData);
    QByteArray toRaw();
    QString toString();
    QStringList toStringList();

    void setValid(bool validity);
    bool isValid();

public slots:
    void appendStdOut(const QByteArray &stdOut);
    void appendStdErr(const QByteArray &stdErr);

private:
    QStringList rawToStringList(const QByteArray &rawData);

    bool m_isValid;
    QByteArray m_rawOutput;
    QByteArray m_rawError;
};

#endif // QESRESULT_H
