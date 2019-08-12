#pragma once

#include <cassert>
#include <limits>
#include <type_traits>

#include <ring_buffer/details/bitop_unsigned_integer.hpp>
#include <ring_buffer/details/countl_zero.hpp>

namespace wiz::details::bit {

    template <typename T, std::enable_if_t<bitop_unsigned_integer<T>::value>* = nullptr>
    constexpr T ceil2(T t) noexcept {
        if (t < 2) {
            return 1;
        }
        unsigned const n = static_cast<unsigned>(std::numeric_limits<T>::digits - countl_zero(static_cast<T>(t - 1u)));
        assert(n != std::numeric_limits<T>::digits); // "Bad input to ceil2"

        if constexpr (sizeof(T) >= sizeof(unsigned)) {
            return T{1} << n;
        } else {
            constexpr unsigned const extra = static_cast<unsigned>(std::numeric_limits<unsigned>::digits - std::numeric_limits<T>::digits);
            unsigned const retVal = 1u << (n + extra);
            return static_cast<T>(retVal >> extra);
        }
    }

} // namespace wiz::details::bit
