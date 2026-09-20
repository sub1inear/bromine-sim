/**
 * \file system/build_info.h
 *
 * Build stamp recorded by CMake at build time.
 *
 * Defined in the generated pros_build_info.c. On a real brain these symbols are
 * injected by the PROS CLI at link time and declared weak so a build without the
 * CLI still links; here CMake always provides them, so they are ordinary strong
 * definitions.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/** Human readable local build time, "YYYY-MM-DD HH:MM:SS". */
extern char const* const _PROS_COMPILE_TIMESTAMP;

/** Source directory the build came from, tail-truncated to fit the banner. */
extern char const* const _PROS_COMPILE_DIRECTORY;

/** Build time as seconds since the Unix epoch. */
extern const int _PROS_COMPILE_TIMESTAMP_INT;

#ifdef __cplusplus
}
#endif
