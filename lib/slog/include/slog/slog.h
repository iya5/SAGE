/* header for slog (Sage Logger)

This file is part of slog

slog is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

slog is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
slog; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */

#ifndef __SLOG_H__
#define __SLOG_H__

#include <stdio.h>

enum SLOG_LEVEL {
    PASS,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

void slog_log(FILE *output, 
              enum SLOG_LEVEL level, 
              const char *file,
              int line,
              const char *fmt, ...);

#define LOG_PASS(fmt, ...)  slog_log(stdout, PASS, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) slog_log(stdout, DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  slog_log(stdout, INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  slog_log(stdout, WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) slog_log(stderr, ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) slog_log(stderr, FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif /* __SLOG_H__ */
