/* internal header for slog (Sage Logger)

This file is part of slog

slog is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

slog is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
slog; see the file LICENSE. If not, see <https://www.gnu.org/licenses/>.    */


#ifndef __SLOG_INTERNAL_H__
#define __SLOG_INTERNAL_H__

// normal colors
#define GREEN "\e[0;32m"
#define BLUE "\e[0;34m"

// bold colours
#define RED_B "\e[1;31m"
#define GREEN_B "\e[1;32m"
#define YELLOW_B "\e[1;33m"
#define BLUE_B "\e[1;34m"
#define WHITE_B "\e[1;37m"

// background colours
#define RED_BG  "\e[41m"

#define RESET "\e[0m"

// labels for log levels
#define PASS_LABEL  "LOG_PASS"
#define DEBUG_LABEL "LOG_DEBUG"
#define INFO_LABEL  "LOG_INFO"
#define WARN_LABEL  "LOG_WARN"
#define ERROR_LABEL "LOG_ERROR"
#define FATAL_LABEL "LOG_FATAL"

// labels for log levels with syntax color
#define PASS_LABEL_FMT  GREEN_B PASS_LABEL RESET
#define DEBUG_LABEL_FMT BLUE_B DEBUG_LABEL RESET
#define INFO_LABEL_FMT  WHITE_B INFO_LABEL RESET
#define WARN_LABEL_FMT  YELLOW_B WARN_LABEL RESET
#define ERROR_LABEL_FMT RED_B ERROR_LABEL RESET
#define FATAL_LABEL_FMT RED_BG WHITE_B FATAL_LABEL RESET

#define LOG_PREHEADER_FMT GREEN "%s" RESET " | %s\t| " BLUE "%s" RESET ":" BLUE "%d" RESET " - "

#endif /* __SLOG_INTERNAL_H__ */
