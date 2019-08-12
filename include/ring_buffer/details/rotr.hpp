#pragma once

#include <limits>
#include <type_traits>

#include <ring_buffer/details/bitop_unsigned_integer.hpp>

namespace wiz::details::bit {

    template <typename T, std::enable_if_t<bitop_unsigned_integer<T>::value>* = nullptr>
    constexpr T rotr(T t, unsigned int cnt) noexcept {
        static_assert(bitop_unsigned_integer<T>::value, "rotr requires unsigned");
        constexpr unsigned int const dig = std::numeric_limits<T>::digits;
        if ((cnt % dig) == 0) {
            return t;
        }
        return (t >> (cnt % dig)) | (t << (dig - (cnt % dig)));
    }

} // namespace wiz::details::bit
