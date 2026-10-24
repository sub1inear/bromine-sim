#include "common/posix.h"

#ifdef _WIN32
#include <stdio.h>
#include <stdarg.h>

int dprintf(int fd, const char* fmt, ...) __attribute__((format(printf, 2, 3)))
{
    if (fd == 3) {
        return 0; // Ignore KDBG_FILENO writes for now.
    }

    va_list args;
    va_start(args, fmt);
    FILE *f = NULL;
    if (fd == 0) {
        f = stdout;
    } else {
        f = stderr;
    }

    int retval = vfprintf(stderr, fmt, args);
    va_end(args);

    return retval;
}
#endif
