/**
 * unwind.c - Unwind functions specialized for PROS
 *
 * Unwinding is necessary in PROS because tasks containing C++ stack frames may
 * be arbitrarily stopped, requiring us to call all the destructors of the task
 * to be killed.
 *
 * \copyright (c) 2017-2024, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void report_fatal_error(uint32_t _sp, const char* error_name) {
	fprintf(stderr, "FATAL ERROR: %s\n", error_name);
	abort();
}
