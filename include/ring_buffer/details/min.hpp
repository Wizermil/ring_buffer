#pragma once

#include <ring_buffer/details/config.hpp>

namespace wiz::details::algorithm {

    template <typename T>
    RING_BUFFER_HIDE_FROM_ABI constexpr T const& min(T const& l, T const& r) noexcept {
        return l < r ? l : r;
    }

} // namespace wiz::details::algorithm
