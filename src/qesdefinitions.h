/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef QESDEFINITIONS_H
#define QESDEFINITIONS_H

#include <QString>
#include "qeasyshellglobal.h"

/*!
  \brief Global Qes convenience structure, holds useful enums and static methods.
  */
struct QEASYSHELLSHARED_EXPORT Qes {
    /*!
      Holds the shell of choice. Defaults to OS default shell.

      NOTE: this is not being used at all right now.
      */
    enum Shell {
        OS         = 0, //! Use QES default (bash/cmd.exe)
        Sh         = 1, //! Use /usr/bin/sh
        Bash       = 2, //! Use /usr/bin/bash
        Tcsh       = 3, //! Use /usr/bin/tcsh
        Zsh        = 4, //! Use /usr/bin/zsh
        PowerShell = 5  //! Use PowerShell (Windows only)
    };

    /*!
      Specifies the piping mechanism to use. Defaults to none (for a single command).
      */
    enum Pipeline {
        None           = 0, //! Used for the first command in chain, instructs QES to
                            //! ignore previous entries.

        Chain          = 1, //! Equivalent of "&&" in shell. Does not pass data between previous
                            //! and current commands.

        Pipe           = 2, //! Equivalent of "|" in shell. Routes output of previous
                            //! command as input of current one.

        Redirect       = 4, //! Used at the end of a chain redirects output of the command
                            //! to a file (if it already exists, it will be removed).
                            //! Equivalent to '>' in shell.

        RedirectAppend = 8  //! Used at the end of a chain redirects output of the command
                            //! to a file (if it already exists, data will be appended).
                            //! Equivalent to ">>" in shell.
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
        else if (shell == PowerShell)
            return "PowerShell";
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
        else if (pl == Redirect)
            return " > ";
        else if (pl == RedirectAppend)
            return " >> ";

        return QString();
    }
};

#endif // QESDEFINITIONS_H
