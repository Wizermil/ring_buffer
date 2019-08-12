#pragma once

namespace wiz::details::bit {

    constexpr int clz(unsigned x) noexcept { return __builtin_clz(x); }

    constexpr int clz(unsigned long x) noexcept { return __builtin_clzl(x); }

    constexpr int clz(unsigned long long x) noexcept { return __builtin_clzll(x); }

} // namespace wiz::details::bit
