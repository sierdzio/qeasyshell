/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include <QCoreApplication>
#include "detachedexecution.h"

/*!
  \defgroup exampleAsynchronous Asynchronous Example
  @{
  This example shows how to use QEasyShell asynchronous API.
  \snippet asynchronous.cpp AsynchronousExample

  Header:
  \snippet detachedexecution.h AsyncClassHeaderExample

  Source:
  \snippet detachedexecution.cpp AsyncClassSourceExample
  */

//! [AsynchronousExample]
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    DetachedExecution object;
    object.metaObject()->invokeMethod(&object, "exec", Qt::QueuedConnection);

    return app.exec();
}
//! [AsynchronousExample]
//! @}
