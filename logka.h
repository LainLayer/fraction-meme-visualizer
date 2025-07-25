#ifndef LOGKA_H
#define LOGKA_H

// assume user included stdio.h

#define DEBUG_LABEL "[\033[36mDEBUG\033[0m]: " /* Cyan   */
#define WARN_LABEL  "[\033[33m WARN\033[0m]: " /* Yellow */
#define ERROR_LABEL "[\033[31mERROR\033[0m]: " /* Red    */
#define INFO_LABEL  "[\033[34m INFO\033[0m]: " /* Blue   */
#define OK_LABEL    "[\033[32m   OK\033[0m]: " /* Green  */

#ifdef SILENT
    #define debugf(format, ...) ((void)0)
    #define  warnf(format, ...) ((void)0)
    #define errorf(format, ...) ((void)0)
    #define  infof(format, ...) ((void)0)
    #define    okf(format, ...) ((void)0)

    #define debug(message) ((void)0)
    #define  warn(message) ((void)0)
    #define error(message) ((void)0)
    #define  info(message) ((void)0)
    #define    ok(message) ((void)0)
#else
    #define debugf(format, ...) fprintf(stdout, DEBUG_LABEL format "\n", __VA_ARGS__)
    #define  warnf(format, ...) fprintf(stdout, WARN_LABEL  format "\n", __VA_ARGS__)
    #define errorf(format, ...) fprintf(stdout, ERROR_LABEL format "\n", __VA_ARGS__)
    #define  infof(format, ...) fprintf(stdout, INFO_LABEL  format "\n", __VA_ARGS__)
    #define    okf(format, ...) fprintf(stdout, OK_LABEL    format "\n", __VA_ARGS__)

    #define debug(message) fprintf(stdout, DEBUG_LABEL message "\n")
    #define  warn(message) fprintf(stdout, WARN_LABEL  message "\n")
    #define error(message) fprintf(stdout, ERROR_LABEL message "\n")
    #define  info(message) fprintf(stdout, INFO_LABEL  message "\n")
    #define    ok(message) fprintf(stdout, OK_LABEL    message "\n")
#endif

#ifdef RELEASE
    #undef  debug
    #define debug(format, ...) ((void)0)
#endif

#endif // LOGKA_H
