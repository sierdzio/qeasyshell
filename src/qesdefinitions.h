/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QESDEFINITIONS_H
#define QESDEFINITIONS_H

#include <QtCore/QString>

#include "qeasyshellglobal.h"

struct QEASYSHELLSHARED_EXPORT Qes {
    /*!
      Holds the shell of choice. Defaults to OS default shell.

      NOTE: this is not being used at all right now.
      */
    enum Shell {
        OS   = 0,
        Sh   = 1,
        Bash = 2,
        Tcsh = 3,
        Zsh  = 4
    };

    /*!
      Specifies the piping mechanism to use. Defaults to none (for a single command).
      */
    enum Pipeline {
        None  = 0,
        Chain = 1,
        Pipe  = 2
    };

    /*!
      Transforms the value of Shell enum into a nice string.
      */
    static QString shellToString(Shell shell) {
        if (shell == OS)
            return "";
        else if (shell == Sh)
            return "sh";
        else if (shell == Bash)
            return "bash";
        else if (shell == Tcsh)
            return "tcsh";
        else if (shell == Zsh)
            return "zsh";
        else
            return "sh"; // default.
    }

    /*!
      Transforms the value of Pipeline enum into a nice string.
      */
    static QString pipelineToString(Pipeline pl) {
        if (pl == Chain)
            return " && ";
        else if (pl == Pipe)
            return " | ";

        return QString();
    }
};

#endif // QESDEFINITIONS_H
