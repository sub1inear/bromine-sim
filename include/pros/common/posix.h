#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#undef _fstat
#undef _stat
#undef _isatty
#endif

#include <stdint.h>

#ifdef _WIN32
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

typedef int mode_t;
typedef long long ssize_t;
typedef uint32_t useconds_t;
typedef long suseconds_t;

int dprintf(int fd, const char* fmt, ...) __attribute__((format(printf, 2, 3)));
#define O_ACCMODE 0x0003
#define O_NONBLOCK 0x0800
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

struct timeval {
    time_t      tv_sec;  /* Seconds */
    suseconds_t tv_usec; /* Microseconds [0, 999999] */
};

struct timespec {
    time_t tv_sec;  /* Seconds */
    long   tv_nsec; /* Nanoseconds [0, 999999999] */
};


#ifndef F_OK
#define F_OK 0
#endif

#ifndef TIMEVAL_TO_TIMESPEC
#define TIMEVAL_TO_TIMESPEC(tv, ts)       \
    do {                                   \
        (ts)->tv_sec = (tv)->tv_sec;       \
        (ts)->tv_nsec = (tv)->tv_usec * 1000;\
    } while (0)
#endif

typedef int32_t clockid_t;

#else
#include <unistd.h>
#include <sys/time.h>
#endif


int gettimeofday(struct timeval* tp, void* tzp);

struct _reent { int _errno; int dummy; };

int iprintf(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

#undef _REENT
extern struct _reent* _REENT;
