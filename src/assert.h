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

#ifndef SAGE_ASSERT_H
#define SAGE_ASSERT_H

#include <stdint.h>

/* Comment out if disabling assertions */
#define SAGE_ENABLE_ASSERTIONS

#ifdef SAGE_ENABLE_ASSERTIONS

void report_assertion_failure(const char *expression,
                              const char *message,
                              const char *file,
                              int32_t line);

#define SASSERT(expr)                                                   \
    {                                                                   \
        if (expr) {                                                     \
        } else {                                                        \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
            exit(1);                                                    \
        }                                                               \
    }
#define SASSERT_MSG(expr, msg)                                          \
    {                                                                   \
        if (expr) {                                                     \
        } else {                                                        \
            report_assertion_failure(#expr, msg, __FILE__, __LINE__);   \
            exit(1);                                                    \
        }                                                               \
    }
#else
#define SASSERT(expr)
#define SASSERT_MSG(expr, msg)
#endif /* SAGE_ENABLE_ASSERTIONS */

#endif /* SAGE_ASSERT_H */
