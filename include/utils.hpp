#pragma once
#include <format>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace cobalt
{

using json = nlohmann::json;

// Implementation, not meant to be called directly.
[[noreturn]]
void errorCore(std::string_view fmt, std::format_args args);

template <typename... A>
[[noreturn]]
void error(std::format_string<A...> fmt, A &&...args)
{
    errorCore(fmt.get(), std::make_format_args(args...));
}

constexpr float inchesToMeters(float inches) { return inches * 0.0254f; }

constexpr float ccwToCw(float ccw) { return -ccw; }

constexpr float poundsToKilograms(float pounds) { return pounds * 0.453592f; }

template <typename T> T expectJsonKey(const json &j, std::string_view key)
{
    auto it = j.find(key);
    if (it == j.end())
    {
        error("expected key `{}`", key);
    }

    if (it->is_null())
    {
        error("please fill out key `{}`", key);
    }

    try
    {
        return it->get<T>();
    }
    catch (const json::type_error &e)
    {
        error("incorrect type for key `{}`", key);
    }
}

} // namespace cobalt
