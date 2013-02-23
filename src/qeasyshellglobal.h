/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QEASYSHELL_GLOBAL_H
#define QEASYSHELL_GLOBAL_H

#include <qglobal.h>

#if defined(QEASYSHELL_LIBRARY)
#  define QEASYSHELLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QEASYSHELLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QEASYSHELL_GLOBAL_H
