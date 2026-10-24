#pragma once

#include <stdint.h>

#ifdef _WIN32
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

#ifndef F_OK
#define F_OK 0
#endif

int dprintf(int fd, const char* fmt, ...) __attribute__((format(printf, 2, 3)));
#else
#include <unistd.h>
#endif
