#include "utils.hpp"
#include <cstdlib>
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
    std::abort();
}

void warningCore(std::string_view fmt, std::format_args args)
{
    std::string msg = std::vformat(fmt, args);

    const char *warning = IS_ATTY(/*stderr*/ 2) ? "\x1b[33m"
                                                  "warning"
                                                  "\x1b[0m"
                                                : "warning";
    std::println(stderr, "{}: {}", warning, msg);
}

void infoCore(std::string_view fmt, std::format_args args)
{
    std::string msg = std::vformat(fmt, args);

    const char *info = IS_ATTY(/*stderr*/ 2) ? "\x1b[36m"
                                               "info"
                                               "\x1b[0m"
                                             : "info";
    std::println(stderr, "{}: {}", info, msg);
}

uint8_t expectJsonPort(const json &j)
{
    uint8_t port = expectJsonKey<uint8_t>(j, "port");
    if (port < 1 || port > 21)
    {
        error("motor port must be in range [1, 21], got `{}`", port);
    }
    return port;
}

} // namespace bromine
