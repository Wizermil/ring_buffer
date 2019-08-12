#pragma once

namespace wiz::details::algorithm {

    template <typename T>
    static constexpr T const& max(T const& l, T const& r) noexcept {
        return l > r ? l : r;
    }

} // namespace wiz::details::algorithm
