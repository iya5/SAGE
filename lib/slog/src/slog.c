/* source code for slog (Sage Logger)

This file is part of slog

slog is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

slog is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
slog; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#include <time.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "../include/slog/slog.h"
#include "internal.h"

#define TIME_FMT_BUFFER_SIZE 64
#define LEVEL_FMT_BUFFER_SIZE 32

#define LOG_LEVEL_FORMAT(fmt, level) { memcpy(fmt, level, sizeof(level)); }

static int32_t timef_get(char time_fmt[TIME_FMT_BUFFER_SIZE]);
static void level_format(char level_fmt[LEVEL_FMT_BUFFER_SIZE], enum SLOG_LEVEL level);

void slog_log(FILE *output, 
              enum SLOG_LEVEL level, 
              const char *file,
              int line,
              const char *fmt, ...)
{
    char time_buffer[TIME_FMT_BUFFER_SIZE] = {0};
    char level_buffer[LEVEL_FMT_BUFFER_SIZE] = {0};
    va_list args;

    timef_get(time_buffer);
    level_format(level_buffer, level);

    va_start(args, fmt);
    fprintf(output, LOG_PREHEADER_FMT, time_buffer, level_buffer, file, line);
    vfprintf(output, fmt, args);
    fprintf(output, "\n");
    va_end(args);
}

static int32_t timef_get(char time_fmt[TIME_FMT_BUFFER_SIZE])
{
    time_t raw_time = (time_t) -1;
    struct tm *info = NULL;
    time(&raw_time);

    if (raw_time == (time_t) -1)
        return 1;

    info = localtime(&raw_time);
    strftime(time_fmt, 80, "%Y-%m-%d %H:%M:%S", info);

    return 0;
}

static void level_format(char level_fmt[LEVEL_FMT_BUFFER_SIZE], enum SLOG_LEVEL level)
{
    switch (level) {
        case PASS:  LOG_LEVEL_FORMAT(level_fmt, PASS_LABEL_FMT); break;
        case DEBUG: LOG_LEVEL_FORMAT(level_fmt, DEBUG_LABEL_FMT); break;
        case INFO:  LOG_LEVEL_FORMAT(level_fmt, INFO_LABEL_FMT); break;
        case WARN:  LOG_LEVEL_FORMAT(level_fmt, WARN_LABEL_FMT); break;
        case ERROR: LOG_LEVEL_FORMAT(level_fmt, ERROR_LABEL_FMT); break;
        case FATAL: LOG_LEVEL_FORMAT(level_fmt, FATAL_LABEL_FMT); break;
    }
}
