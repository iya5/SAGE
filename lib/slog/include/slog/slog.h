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

enum SLOG_LEVEL {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

void slog_log(enum SLOG_LEVEL level);

#endif /* __SLOG_H__ */
