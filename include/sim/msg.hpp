#pragma once
#include <format>
#include <string>
#include <string_view>

namespace bromine
{
// Implementation, not meant to be called directly.
[[noreturn]]
void errorCore(std::string_view fmt, std::format_args args);

template <typename... A>
[[noreturn]]
void error(std::format_string<A...> fmt, A &&...args)
{
    errorCore(fmt.get(), std::make_format_args(args...));
}

void warningCore(std::string_view fmt, std::format_args args);

template <typename... A> void warning(std::format_string<A...> fmt, A &&...args)
{
    warningCore(fmt.get(), std::make_format_args(args...));
}

void infoCore(std::string_view fmt, std::format_args args);

template <typename... A> void info(std::format_string<A...> fmt, A &&...args)
{
    infoCore(fmt.get(), std::make_format_args(args...));
}
} // namespace bromine
