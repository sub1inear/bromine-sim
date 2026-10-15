#pragma once

namespace bromine
{

enum class Categories : uint64_t
{
    None = 0x0,
    Player = 0x1,
    Object = 0x2,
    Wall = 0x4,
    Goal = 0x8,
    All = 0xFFFFFFFFFFFFFFFF,
};

constexpr Categories operator|(Categories lhs, Categories rhs)
{
    using T = std::underlying_type_t<Categories>;
    return static_cast<Categories>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

constexpr Categories operator&(Categories lhs, Categories rhs)
{
    using T = std::underlying_type_t<Categories>;
    return static_cast<Categories>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

} // namespace bromine