#ifndef QESRESULT_H
#define QESRESULT_H

#include "QEasyShell_global.h"
#include "qesdefinitions.h"
//#include "qescommand.h"

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
    QesResult pipe(const QesCommand &command);
    QesResult chain(const QesCommand &command);

    void setRaw(QByteArray rawData);
    QByteArray toRaw();
    QString toString();
    QStringList toStringList();

    void setValid(bool validity);
    bool isValid();

private:
    bool m_isValid;
    QByteArray m_rawOutput;
};

#endif // QESRESULT_H
