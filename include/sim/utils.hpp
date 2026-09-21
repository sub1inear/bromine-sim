#pragma once
#include "msg.hpp"
#include <nlohmann/json.hpp>

namespace bromine
{

using json = nlohmann::json;

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

} // namespace bromine
