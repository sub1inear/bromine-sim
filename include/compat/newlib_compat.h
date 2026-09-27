#pragma once
// NOTE: This file must be C-compatible

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>

static inline double infinity(void) { return HUGE_VAL; }

static inline float infinityf(void) { return HUGE_VALF; }

// glibc declares these already.
#ifndef __GLIBC__
static inline int isnanf(float x) { return isnan(x); }

static inline int isinff(float x) { return isinf(x); }

static inline int finitef(float x) { return isfinite(x); }
#endif
