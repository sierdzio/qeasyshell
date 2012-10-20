#ifndef QESDEFINITIONS_H
#define QESDEFINITIONS_H

#include <QtCore/QString>

#include "qeasyshellglobal.h"

struct QEASYSHELLSHARED_EXPORT Qes {
    enum Shell {
        Sh   = 0,
        Bash = 1,
        Tcsh = 2,
        Zsh  = 3
    };

    static QString commandForShell(Shell shell) {
        if (shell == Sh)
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
};


#endif // QESDEFINITIONS_H
