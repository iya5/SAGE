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

#include "../include/slog/slog.h"

#define TIME_FMT_BUFFER_SIZE 80
#define RED

/*
 * Stores time formatted in yy-mm-dd hh:mm:ss in a char buffer.
 * On success, get_time() returns 0, and on failure, returns 1
 */
static int32_t get_time(char time_fmt[TIME_FMT_BUFFER_SIZE]);

void slog_log(void)
{
    char time_fmt[TIME_FMT_BUFFER_SIZE];
    int32_t res = get_time(time_fmt);

    if (res != 0) {
        fprintf(stderr, "Failed to get time\n");
        return;
    }

    printf("[ %s ] | %s |\n");
}

static int32_t get_time(char time_fmt[TIME_FMT_BUFFER_SIZE])
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
