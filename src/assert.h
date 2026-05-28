#ifndef SAGE_ASSERT_H
#define SAGE_ASSERT_H

#include <stdint.h>
#include <stdlib.h>

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
