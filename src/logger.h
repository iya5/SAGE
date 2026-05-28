#ifndef SAGE_LOGGER_H
#define SAGE_LOGGER_H 

#include <stdint.h>

/* Commenting disables specific logs */
#define SAGE_LOG_FATAL_ENABLE  /* Highly recommended to keep this on! */
#define SAGE_LOG_ERROR_ENABLE  /* Recommended keep this on! */
#define SAGE_LOG_WARN_ENABLE
#define SAGE_LOG_INFO_ENABLE
#define SAGE_LOG_DEBUG_ENABLE
#define SAGE_LOG_TRACE_ENABLE

#ifdef SAGE_RELEASE
#   define LOG_DEBUG_ENABLE 0
#   define LOG_TRACE_ENABLE 0
#endif /* SAGE_RELEASE */

enum log_level {
    LOG_LEVEL_FATAL,  /* Errors that kills the program                    */
    LOG_LEVEL_ERROR,  /* Recoverable errors                               */
    LOG_LEVEL_WARN,   /* Sub-optimal behaviour                            */
    LOG_LEVEL_INFO,   /* General logging information                      */
    LOG_LEVEL_DEBUG,  /* Debug information not included in release builds */
    LOG_LEVEL_TRACE,  /* Verbose version of debugs                        */
};

void sage_log_output(enum log_level level, const char *fmt, ...);

#ifdef SAGE_LOG_FATAL_ENABLE
#   define SFATAL(fmt, ...) sage_log_output(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#else
#   define SFATAL(fmt, ...)
#endif /* SAGE_LOG_FATAL_ENABLE */

#ifdef SAGE_LOG_ERROR_ENABLE
#   define SERROR(fmt, ...) sage_log_output(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#else
#   define SERROR(fmt, ...)
#endif /* SAGE_LOG_ERROR_ENABLE */

#ifdef SAGE_LOG_WARN_ENABLE
#   define SWARN(fmt, ...) sage_log_output(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#else
#   define SWARN(fmt, ...)
#endif /* SAGE_LOG_WARN_ENABLE */

#ifdef SAGE_LOG_INFO_ENABLE
#   define SINFO(fmt, ...) sage_log_output(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#else
#   define SINFO(fmt, ...)
#endif /* SAGE_LOG_INFO_ENABLE */

#ifdef SAGE_LOG_DEBUG_ENABLE
#   define SDEBUG(fmt, ...) sage_log_output(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#   define SDEBUG(fmt, ...)
#endif /* SAGE_LOG_DEBUG_ENABLE */

#ifdef SAGE_LOG_TRACE_ENABLE
#   define STRACE(fmt, ...) sage_log_output(LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)
#else
#   define SERROR(fmt, ...)
#endif /* SAGE_LOG_TRACE_ENABLE */

#endif /* SAGE_LOGGER_H */
