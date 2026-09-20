#include "common/posix.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
int dprintf(int fd, const char* fmt, ...) __attribute__((format(printf, 2, 3)))
{
    va_list args;
    va_start(args, fmt);
    FILE *f = NULL;
    if (fd == 0) {
        f = stdout;
    } else {
        f = stderr;
    }
    if (fd == 3) {
        fprintf(stderr, "kerr: ");
    }

    int retval = vfprintf(stderr, fmt, args);
    va_end(args);

    return retval;
}
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int gettimeofday(struct timeval* tp, void* tzp) {
    // Note: The tzp parameter (timezone) is ignored as it is obsolete in POSIX

    if (tp != NULL) {
        FILETIME ft;
        ULARGE_INTEGER uli;

        // Get the current system time in 100-nanosecond intervals
        GetSystemTimeAsFileTime(&ft);

        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;

        // Convert from 1601-01-01 to 1970-01-01 epoch (in 100-nanosecond intervals)
        // 116444736000000000 = seconds between epochs * 10,000,000
        uint64_t unix_time = uli.QuadPart - 116444736000000000ULL;

        // Convert to seconds and microseconds
        tp->tv_sec = (long)(unix_time / 10000000ULL);
        tp->tv_usec = (long)((unix_time % 10000000ULL) / 10ULL);
    }

    return 0;
}
#endif

// newlib's integer-only printf. The host C library has no such variant, so
// forward to the full printf; accepting float conversions is harmless here.
int iprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int retval = vprintf(fmt, args);
    va_end(args);

    return retval;
}

struct _reent _REENT_S;
struct _reent *_REENT = &_REENT_S;
