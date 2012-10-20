#ifndef QESRESULT_H
#define QESRESULT_H

#include "QEasyShell_global.h"
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
    QesResult *pipe(QesCommand command);
    QesResult *chain(QesCommand command);

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

private:
    QStringList rawToStringList(const QByteArray &rawData);

    bool m_isValid;
    QByteArray m_rawOutput;
    QByteArray m_rawError;
};

#endif // QESRESULT_H
