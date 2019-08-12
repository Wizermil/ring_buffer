#pragma once

#include <limits>
#include <type_traits>

#include <ring_buffer/details/bitop_unsigned_integer.hpp>
#include <ring_buffer/details/clz.hpp>
#include <ring_buffer/details/rotr.hpp>

namespace wiz::details::bit {

    template <typename T, std::enable_if_t<bitop_unsigned_integer<T>::value>* = nullptr>
    constexpr int countl_zero(T t) noexcept {
        static_assert(bitop_unsigned_integer<T>::value, "countl_zero requires unsigned");
        if (t == 0) {
            return std::numeric_limits<T>::digits;
        }

        if constexpr (sizeof(T) <= sizeof(unsigned int)) {
            return clz(static_cast<unsigned int>(t)) - (std::numeric_limits<unsigned int>::digits - std::numeric_limits<T>::digits);
        } else if constexpr (sizeof(T) <= sizeof(unsigned long)) {
            return clz(static_cast<unsigned long>(t)) - (std::numeric_limits<unsigned long>::digits - std::numeric_limits<T>::digits);
        } else if constexpr (sizeof(T) <= sizeof(unsigned long long)) {
            return clz(static_cast<unsigned long long>(t)) - (std::numeric_limits<unsigned long long>::digits - std::numeric_limits<T>::digits);
        } else {
            int ret = 0;
            int iter = 0;
            constexpr unsigned int const ulldigits = std::numeric_limits<unsigned long long>::digits;
            while (true) {
                t = rotr(t, ulldigits);
                if ((iter = countl_zero(static_cast<unsigned long long>(t))) != ulldigits) {
                    break;
                }
                ret += iter;
            }
            return ret + iter;
        }
    }
} // namespace wiz::details::bit
