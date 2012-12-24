/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QESPROCESS_H
#define QESPROCESS_H

#include <QtCore/QByteArray>
#include <QtCore/QProcess>

/*!
  \brief Expands QProcess with some methods important in shell communication.

  A thin wrapper for QProcess, adds some functionality missing in base class that
  is required by QES.
  */
class QesProcess : public QProcess
{
    Q_OBJECT

public:
    explicit QesProcess(int index, QObject *parent = 0);
    bool isError();
    QString errorString();
    using QProcess::write;
    
signals:
    /*!
      Similar to base signal of QProcess of that same name, but this one passes
      the \a data and \a processIndex, too, for convenience.
      */
    void readyReadStandardOutput(const QByteArray &data, int processIndex);

    /*!
      Similar to base signal of QProcess of that same name, but this one passes
      the \a data and \a processIndex, too, for convenience.
      */
    void readyReadStandardError(const QByteArray &data, int processIndex);
    
public slots:
    qint64 write(const QByteArray &data, int processIndex);
    void setError(QProcess::ProcessError error);

protected slots:
    void prepareReadStdOut();
    void prepareReadStdErr();
    
private:
    int m_index;
    bool m_error;
};

/*!
  \relates QesProcess

  Convenience typedef for a list of QesProcess objects.
  */
typedef QList<QesProcess *> ProcessList;

#endif // QESPROCESS_H
