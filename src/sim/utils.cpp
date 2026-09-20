#include "utils.hpp"
#include <print>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#define IS_ATTY(fd) _isatty(fd)
#elif defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
#define IS_ATTY(fd) isatty(fd)
#else
#define IS_ATTY(fd) 0
#endif

namespace bromine
{
void errorCore(std::string_view fmt, std::format_args args)
{
    std::string msg = std::vformat(fmt, args);

    const char *error = IS_ATTY(/*stderr*/ 2) ? "\x1b[31m"
                                                "error"
                                                "\x1b[0m"
                                              : "error";
    std::println(stderr, "{}: {}", error, msg);
    abort();
}
} // namespace bromine
