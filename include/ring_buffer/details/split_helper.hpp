#pragma once

extern "C" {
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
}

#include <utility>
#include <type_traits>

namespace wiz::details::algorithm {

    // copy many values

    template <typename It>
    static inline void copy(It* __restrict__ first, It* __restrict__ last, It* __restrict__ result) noexcept {
        for (; first != last; ++first, ++result) {
            new (result) It{*first};
        }
    }

    template <typename It>
    static inline void copy_assign(It* __restrict__ first, It* __restrict__ last, It* __restrict__ result) noexcept {
        if constexpr (std::is_trivially_copy_assignable_v<It>) {
            size_t const n = static_cast<size_t>(last - first);
            if (n > 0) {
                __builtin_memcpy(result, first, n * sizeof(It));
            }
        } else {
            for (; first != last; ++first, ++result) {
                *result = *first;
            }
        }
    }

    template <typename It>
    static inline void move(It* __restrict__ first, It* __restrict__ last, It* __restrict__ result) noexcept {
        for (; first != last; ++first, ++result) {
            new (result) It{std::move(*first)};
        }
    }

    template <typename It, typename Fn>
    static inline void basic_split(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap, It* __restrict__ out,
                                   It* __restrict__ out_beg, size_t out_cap, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap || static_cast<size_t>(last - first) <= out_cap);
        It* __restrict__ const out_cap_ptr{out_beg + out_cap}, *__restrict__ const in_cap_ptr{in_beg + in_cap};

        uint8_t split{};
        if (out > out_cap_ptr) {
            split = 12;
        } else if (out + static_cast<size_t>(last - first) > out_cap_ptr) {
            split = 4;
        }
        if (first > in_cap_ptr) {
            split |= 3;
        } else if (last > in_cap_ptr) {
            split |= 1;
        }

        switch (split) {
            case 0: {
                // in : [first, ..., last   , ..., in_cap ]
                // out: [out  , ..., out + n, ..., out_cap]
                f(first, last, out);
                break;
            }
            case 1: {
                // in : [first, ..., in_cap , ..., last   ]
                // out: [out  , ..., out + n, ..., out_cap]
                f(first, in_cap_ptr, out);
                f(in_beg, last - in_cap, out + static_cast<size_t>(in_cap_ptr - first));
                break;
            }
            case 3: {
                // in : [in_cap, ..., first  , ..., last   ]
                // out: [out   , ..., out + n, ..., out_cap]
                f(first - in_cap, last - in_cap, out);
                break;
            }
            case 4: {
                // in : [first, ..., last   , ..., in_cap ]
                // out: [out  , ..., out_cap, ..., out + n]
                size_t const out_wrap{static_cast<size_t>(out_cap_ptr - out)};
                f(first, first + out_wrap, out);
                f(first + out_wrap, last, out_beg);
                break;
            }
            case 5: {
                // in : [first, ..., in_cap , ..., last   ]
                // out: [out  , ..., out_cap, ..., out + n]
                size_t const in_wrap{static_cast<size_t>(in_cap_ptr - first)}, out_wrap{static_cast<size_t>(out_cap_ptr - out)};
                if (in_wrap <= out_wrap) {
                    f(first, first + in_wrap, out);
                    f(in_beg, in_beg + out_wrap - in_wrap, out + in_wrap);
                    f(in_beg + out_wrap - in_wrap, last - in_wrap, out_beg);
                } else {
                    f(first, first + out_wrap, out);
                    f(first + out_wrap, first + in_wrap, out_beg);
                    f(in_beg, last - in_wrap, out_beg + in_wrap - out_wrap);
                }
                break;
            }
            case 7: {
                // in : [in_cap, ..., first  , ..., last   ]
                // out: [out   , ..., out_cap, ..., out + n]
                size_t const out_wrap{static_cast<size_t>(out_cap_ptr - out)};
                f(first - in_cap, first + out_wrap - in_cap, out);
                f(first + out_wrap - in_cap, last - in_cap, out_beg);
                break;
            }
            case 12: {
                // in : [first  , ..., last, ..., in_cap ]
                // out: [out_cap, ..., out , ..., out + n]
                f(first, last, out - out_cap);
                break;
            }
            case 13: {
                // in : [first  , ..., in_cap , ..., last   ]
                // out: [out_cap, ..., out    , ..., out + n]
                f(first, in_cap_ptr, out - out_cap);
                f(in_beg, last - in_cap, out + (in_cap_ptr - first) - out_cap);
                break;
            }
            case 15: {
                // in : [in_cap , ..., first, ..., last   ]
                // out: [out_cap, ..., out  , ..., out + n]
                f(first - in_cap, last - in_cap, out - out_cap);
                break;
            }
            default:
                assert(false);
        }
    }

    template <typename It>
    static inline void split_copy(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap, It* __restrict__ out,
                                  It* __restrict__ out_beg, size_t out_cap) {
        basic_split(first, last, in_beg, in_cap, out, out_beg, out_cap, copy<It>);
    }

    template <typename It>
    static inline void split_copy_assign(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap, It* __restrict__ out,
                                         It* __restrict__ out_beg, size_t out_cap) {
        basic_split(first, last, in_beg, in_cap, out, out_beg, out_cap, copy_assign<It>);
    }

    template <typename It>
    static inline void split_move(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap, It* __restrict__ out,
                                         It* __restrict__ out_beg, size_t out_cap) {
        basic_split(first, last, in_beg, in_cap, out, out_beg, out_cap, move<It>);
    }

    // copy one value

    template <typename It>
    static inline void copy_assign_value(It* __restrict__ first, It* __restrict__ last, It const& value) noexcept {
        for (; first != last; ++first) {
            if constexpr (std::is_trivially_copy_assignable_v<It>) {
                __builtin_memcpy(first, &value, sizeof(It));
            } else {
                *first = value;
            }
        }
    }

    template <typename It>
    static inline void copy_value(It* __restrict__ first, It* __restrict__ last, It const& value) noexcept {
        for (; first != last; ++first) {
            new (first) It{value};
        }
    }

    template <typename It, typename Fn>
    static inline void basic_split_value(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap, It const& value, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap);
        It* __restrict__ const in_cap_ptr{in_beg + in_cap};
        uint8_t split{};
        if (first > in_cap_ptr) {
            split = 3;
        } else if (last > in_cap_ptr) {
            split = 1;
        }

        switch (split) {
            case 0: {
                // in : [first, ..., last   , ..., in_cap ]
                f(first, last, value);
                break;
            }
            case 1: {
                // in : [first, ..., in_cap , ..., last   ]
                f(first, in_cap_ptr, value);
                f(in_beg, last - in_cap, value);
                break;
            }
            case 3: {
                // in : [in_cap, ..., first  , ..., last   ]
                f(first - in_cap, last - in_cap, value);
                break;
            default:
                assert(false);
            }
        }
    }

    template <typename It>
    static inline void split_copy_assign_value(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap, It const& value) noexcept {
        basic_split_value(first, last, in_beg, in_cap, value, copy_assign_value<It>);
    }

    template <typename It>
    static inline void split_copy_value(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap, It const& value) noexcept {
        basic_split_value(first, last, in_beg, in_cap, value, copy_value<It>);
    }

    // placement new + destroy

    template <typename It>
    static inline void construct_placement(It* __restrict__ first, It* __restrict__ last) noexcept {
        for (; first != last; ++first) {
            new (first) It{};
        }
    }

    template <typename It>
    static inline void destruct_placement(It* __restrict__ first, It* __restrict__ last) noexcept {
        for (; first != last; ++first) {
            first->~It();
        }
    }

    template <typename It, typename Fn>
    static inline void basic_split_placement(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap);
        It* __restrict__ const in_cap_ptr{in_beg + in_cap};
        uint8_t split{};
        if (first > in_cap_ptr) {
            split = 3;
        } else if (last > in_cap_ptr) {
            split = 1;
        }

        switch (split) {
            case 0: {
                // in : [first, ..., last   , ..., in_cap ]
                f(first, last);
                break;
            }
            case 1: {
                // in : [first, ..., in_cap , ..., last   ]
                f(first, in_cap_ptr);
                f(in_beg, last - in_cap);
                break;
            }
            case 3: {
                // in : [in_cap, ..., first  , ..., last   ]
                f(first - in_cap, last - in_cap);
                break;
            default:
                assert(false);
            }
        }
    }

    template <typename It>
    static inline void split_construct(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap) noexcept {
        basic_split_placement(first, last, in_beg, in_cap, construct_placement<It>);
    }

    template <typename It>
    static inline void split_destruct(It* __restrict__ first, It* __restrict__ last, It* __restrict__ in_beg, size_t in_cap) noexcept {
        basic_split_placement(first, last, in_beg, in_cap, destruct_placement<It>);
    }

} // namespace wiz::details::algorithm
