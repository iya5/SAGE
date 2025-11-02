/* SAGE: Sage Ain't A Game Engine. An OpenGL 3D Renderer.

This file is part of Sage

Sage is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Sage is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
Sage; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "assert.h"
#include "logger.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BG_RED  "\x1b[41m"

/* based on strftime */
#define LOG_TIME_FMT "%H:%M:%S"
#define LOG_TIME_FMT_BUFFER_SIZE 32

static int32_t timef_get(char time_fmt[LOG_TIME_FMT_BUFFER_SIZE]);

int32_t logger_initialize()
{
    /* TODO: cleanup loggers */
    return 0;
}

void logger_shutdown()
{
    /* TODO: cleanup loggers */
}

void sage_log_output(enum log_level level, const char *fmt, ...)
{
    const char *log_level_strings[6] = {
        ANSI_COLOR_BG_RED"[FATAL]"ANSI_COLOR_RESET,
        ANSI_COLOR_RED"[ERROR]"ANSI_COLOR_RESET,
        ANSI_COLOR_YELLOW"[WARN]"ANSI_COLOR_RESET,
        ANSI_COLOR_BLUE"[INFO]"ANSI_COLOR_RESET,
        ANSI_COLOR_BLUE"[DEBUG]"ANSI_COLOR_RESET,
        "[TRACE]"
    };
    // int32_t is_error = level < 2;

    /* Log Entry a limit of 1024 characters */
    char out_message[1024] = {0};

    /* final buffer for prepending the level */
    char prepended_buffer[1064] = {0};

    char time_buffer[LOG_TIME_FMT_BUFFER_SIZE] = {0};

    timef_get(time_buffer);

    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(out_message, 1024, fmt, arg_ptr);
    va_end(arg_ptr);
    snprintf(prepended_buffer, 
             1064, 
             "%s %s %s",
             time_buffer,
             log_level_strings[level], 
             out_message);

    fprintf(stdout, "%s\n", prepended_buffer);
}

static int32_t timef_get(char time_fmt[LOG_TIME_FMT_BUFFER_SIZE])
{
    time_t raw_time = (time_t) -1;
    struct tm *info = NULL;
    time(&raw_time);

    if (raw_time == (time_t) -1)
        return 1;

    info = localtime(&raw_time);
    strftime(time_fmt, LOG_TIME_FMT_BUFFER_SIZE, LOG_TIME_FMT, info);

    return 0;
}

void report_assertion_failure(const char *expression,
                              const char *message,
                              const char *file,
                              int32_t line)
{
    sage_log_output(LOG_LEVEL_FATAL,
                    "Assertion Failure: '%s' message: %s, in file: %d:%s'",
                    expression,
                    message,
                    line,
                    file);
}
