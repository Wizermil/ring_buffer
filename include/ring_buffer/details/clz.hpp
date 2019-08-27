#pragma once

#include <ring_buffer/details/config.hpp>

namespace wiz::details::bit {

    RING_BUFFER_HIDE_FROM_ABI constexpr int clz(unsigned x) noexcept { return __builtin_clz(x); }

    RING_BUFFER_HIDE_FROM_ABI constexpr int clz(unsigned long x) noexcept { return __builtin_clzl(x); }

    RING_BUFFER_HIDE_FROM_ABI constexpr int clz(unsigned long long x) noexcept { return __builtin_clzll(x); }

} // namespace wiz::details::bit
