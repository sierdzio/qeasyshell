#ifndef QESDEFINITIONS_H
#define QESDEFINITIONS_H

#include <QtCore/QString>

#include "qeasyshellglobal.h"

struct QEASYSHELLSHARED_EXPORT Qes {
    enum Shell {
        OS   = 0,
        Sh   = 1,
        Bash = 2,
        Tcsh = 3,
        Zsh  = 4
    };

    enum Pipeline {
        None  = 0,
        Chain = 1,
        Pipe  = 2
    };

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

    static QString pipelineToString(Pipeline pl) {
        if (pl == Chain)
            return " && ";
        else if (pl == Pipe)
            return " | ";

        return QString();
    }
};


#endif // QESDEFINITIONS_H
