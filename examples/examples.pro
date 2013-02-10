TEMPLATE = subdirs

noexamples {
  SUBDIRS +=
} else {
  SUBDIRS += \
    asynchronous \
    basic \
    combiningCommands \
    progressErrorTest \
    inputData \
    naturalSyntax \
    redirection \
    windowsBasic
}
