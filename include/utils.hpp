#pragma once
#include <format>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace cobalt
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

template <typename T>
T expectJsonKey(nlohmann::json &json, std::string_view prefix,
                std::string_view key)
{
    auto it = json.find(key);
    if (it == json.end() || it->is_null())
    {
        if (prefix.empty())
        {
            error("expected key `{}`", key);
        }

        error("expected key `{}.{}`", prefix, key);
    }

    try
    {
        return it->get<T>();
    }
    catch (const nlohmann::json::type_error &e)
    {
        error("incorrect type for key `{}.{}`", prefix, key);
    }
}

} // namespace cobalt
