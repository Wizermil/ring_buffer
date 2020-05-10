#pragma once

extern "C" {
#include <assert.h>
#include <stddef.h>
}

#include <new>
#include <type_traits>
#include <utility>

#include <ring_buffer/details/config.hpp>

namespace wiz::details::algorithm {

    // copy many values

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void copy_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt* __restrict__ result) noexcept {
        for (; first != last; ++first, ++result) {
            new (result) OutputIt{*first};
        }
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void copy_assign(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt* __restrict__ result) noexcept {
        for (; first != last; ++first, ++result) {
            *result = *first;
        }
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void move_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt* __restrict__ result) noexcept {
        for (; first != last; ++first, ++result) {
            new (result) OutputIt{std::move(*first)};
        }
    }

    template <typename InputIt, typename OutputIt, typename Fn>
    RING_BUFFER_HIDE_FROM_ABI constexpr void basic_split_in(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg,
                                                            size_t in_cap, OutputIt* __restrict__ out, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap);
        InputIt* const __restrict__ in_cap_ptr{in_beg + in_cap};
        if (RING_BUFFER_UNLIKELY(first > in_cap_ptr)) {
            // in : [in_cap, ..., first, ..., last]
            f(first - in_cap, last - in_cap, out);
        } else if (RING_BUFFER_UNLIKELY(last > in_cap_ptr)) {
            // in : [first, ..., in_cap, ..., last]
            f(first, in_cap_ptr, out);
            f(in_beg, last - in_cap, out + static_cast<size_t>(in_cap_ptr - first));
        } else {
            // in : [first, ..., last, ..., in_cap]
            f(first, last, out);
        }
    }

    template <typename InputIt, typename OutputIt, typename Fn>
    RING_BUFFER_HIDE_FROM_ABI constexpr void basic_split_out(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt* __restrict__ out,
                                                             OutputIt* __restrict__ out_beg, size_t out_cap, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= out_cap);
        OutputIt* const __restrict__ out_cap_ptr{out_beg + out_cap};
        if (RING_BUFFER_UNLIKELY(out > out_cap_ptr)) {
            // out: [out_cap, ..., out, ..., out + n]
            f(first, last, out - out_cap);
        } else if (RING_BUFFER_UNLIKELY(out + static_cast<size_t>(last - first) > out_cap_ptr)) {
            // out: [out, ..., out_cap, ..., out + n]
            size_t const out_wrap{static_cast<size_t>(out_cap_ptr - out)};
            f(first, first + out_wrap, out);
            f(first + out_wrap, last, out_beg);
        } else {
            // out: [out, ..., out + n, ..., out_cap]
            f(first, last, out);
        }
    }

    template <typename InputIt, typename OutputIt, typename Fn>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    basic_split_inout(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap, OutputIt* __restrict__ out,
                      OutputIt* __restrict__ out_beg, size_t out_cap, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap || static_cast<size_t>(last - first) <= out_cap);
        OutputIt* const __restrict__ out_cap_ptr{out_beg + out_cap};
        InputIt* const __restrict__ in_cap_ptr{in_beg + in_cap};

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
                    f(in_beg + out_wrap - in_wrap, last - in_cap, out_beg);
                } else {
                    f(first, first + out_wrap, out);
                    f(first + out_wrap, first + in_wrap, out_beg);
                    f(in_beg, last - in_cap, out_beg + in_wrap - out_wrap);
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

    // split_copy_placement

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void split_copy_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg,
                                                                  size_t in_cap, OutputIt* __restrict__ out, OutputIt* __restrict__ out_beg, size_t out_cap) {
        basic_split_inout(first, last, in_beg, in_cap, out, out_beg, out_cap, copy_placement<InputIt, OutputIt>);
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    split_copy_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap, OutputIt* __restrict__ out) {
        basic_split_in(first, last, in_beg, in_cap, out, copy_placement<InputIt, OutputIt>);
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    split_copy_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt* __restrict__ out, OutputIt* __restrict__ out_beg, size_t out_cap) {
        basic_split_out(first, last, out, out_beg, out_cap, copy_placement<InputIt, OutputIt>);
    }

    // split_copy_assign

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void split_copy_assign(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg,
                                                               size_t in_cap, OutputIt* __restrict__ out, OutputIt* __restrict__ out_beg, size_t out_cap) {
        basic_split_inout(first, last, in_beg, in_cap, out, out_beg, out_cap, copy_assign<InputIt, OutputIt>);
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    split_copy_assign(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt* __restrict__ out, OutputIt* __restrict__ out_beg, size_t out_cap) {
        basic_split_out(first, last, out, out_beg, out_cap, copy_assign<InputIt, OutputIt>);
    }

    // split_move_placement

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void split_move_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg,
                                                                  size_t in_cap, OutputIt* __restrict__ out, OutputIt* __restrict__ out_beg, size_t out_cap) {
        basic_split_inout(first, last, in_beg, in_cap, out, out_beg, out_cap, move_placement<InputIt, OutputIt>);
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    split_move_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap, OutputIt* __restrict__ out) {
        basic_split_in(first, last, in_beg, in_cap, out, move_placement<InputIt, OutputIt>);
    }

    // copy one value

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void copy_assign_value(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt const& value) noexcept {
        for (; first != last; ++first) {
            *first = value;
        }
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void copy_placement_value(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt const& value) noexcept {
        for (; first != last; ++first) {
            new (first) OutputIt{value};
        }
    }

    template <typename InputIt, typename OutputIt, typename Fn>
    RING_BUFFER_HIDE_FROM_ABI constexpr void basic_split_value_in(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg,
                                                                  size_t in_cap, OutputIt const& value, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap);
        InputIt* const __restrict__ in_cap_ptr{in_beg + in_cap};
        if (RING_BUFFER_UNLIKELY(first > in_cap_ptr)) {
            // in : [in_cap, ..., first, ..., last]
            f(first - in_cap, last - in_cap, value);
        } else if (RING_BUFFER_UNLIKELY(last > in_cap_ptr)) {
            // in : [first, ..., in_cap, ..., last]
            f(first, in_cap_ptr, value);
            f(in_beg, last - in_cap, value);
        } else {
            // in : [first, ..., last, ..., in_cap]
            f(first, last, value);
        }
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    split_copy_assign_value(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap, OutputIt const& value) noexcept {
        basic_split_value_in(first, last, in_beg, in_cap, value, copy_assign_value<InputIt, OutputIt>);
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void split_copy_placement_value(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg,
                                                                        size_t in_cap, OutputIt const& value) noexcept {
        basic_split_value_in(first, last, in_beg, in_cap, value, copy_placement_value<InputIt, OutputIt>);
    }

    // placement new + destroy

    template <typename InputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void construct_placement(InputIt* __restrict__ first, InputIt* __restrict__ last) noexcept {
        for (; first != last; ++first) {
            new (first) InputIt{};
        }
    }

    template <typename InputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void destruct_placement(InputIt* __restrict__ first, InputIt* __restrict__ last) noexcept {
        for (; first != last; ++first) {
            first->~InputIt();
        }
    }

    template <typename InputIt, typename Fn>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    basic_split_placement_in(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap);
        InputIt* const __restrict__ in_cap_ptr{in_beg + in_cap};
        if (RING_BUFFER_UNLIKELY(first > in_cap_ptr)) {
            // in : [in_cap, ..., first, ..., last]
            f(first - in_cap, last - in_cap);
        } else if (RING_BUFFER_UNLIKELY(last > in_cap_ptr)) {
            // in : [first, ..., in_cap, ..., last]
            f(first, in_cap_ptr);
            f(in_beg, last - in_cap);
        } else {
            // in : [first, ..., last, ..., in_cap]
            f(first, last);
        }
    }

    template <typename InputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    split_construct_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap) noexcept {
        basic_split_placement_in(first, last, in_beg, in_cap, construct_placement<InputIt>);
    }

    template <typename InputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr void
    split_destruct_placement(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap) noexcept {
        basic_split_placement_in(first, last, in_beg, in_cap, destruct_placement<InputIt>);
    }

    // equal

    template <typename InputIt, typename OutputIt, typename Fn>
    [[nodiscard]] RING_BUFFER_HIDE_FROM_ABI constexpr bool
    basic_split_result_inout(InputIt* first, InputIt* last, InputIt* in_beg, size_t in_cap, OutputIt* out, OutputIt* out_beg, size_t out_cap, Fn f) noexcept {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap || static_cast<size_t>(last - first) <= out_cap);
        OutputIt* const out_cap_ptr{out_beg + out_cap};
        InputIt* const in_cap_ptr{in_beg + in_cap};

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
                return f(first, last, out);
            }
            case 1: {
                // in : [first, ..., in_cap , ..., last   ]
                // out: [out  , ..., out + n, ..., out_cap]
                return f(first, in_cap_ptr, out) && f(in_beg, last - in_cap, out + static_cast<size_t>(in_cap_ptr - first));
            }
            case 3: {
                // in : [in_cap, ..., first  , ..., last   ]
                // out: [out   , ..., out + n, ..., out_cap]
                return f(first - in_cap, last - in_cap, out);
            }
            case 4: {
                // in : [first, ..., last   , ..., in_cap ]
                // out: [out  , ..., out_cap, ..., out + n]
                size_t const out_wrap{static_cast<size_t>(out_cap_ptr - out)};
                return f(first, first + out_wrap, out) && f(first + out_wrap, last, out_beg);
            }
            case 5: {
                // in : [first, ..., in_cap , ..., last   ]
                // out: [out  , ..., out_cap, ..., out + n]
                size_t const in_wrap{static_cast<size_t>(in_cap_ptr - first)}, out_wrap{static_cast<size_t>(out_cap_ptr - out)};
                if (in_wrap <= out_wrap) {
                    return f(first, first + in_wrap, out) && f(in_beg, in_beg + out_wrap - in_wrap, out + in_wrap) &&
                        f(in_beg + out_wrap - in_wrap, last - in_cap, out_beg);
                } else {
                    return f(first, first + out_wrap, out) && f(first + out_wrap, first + in_wrap, out_beg) &&
                        f(in_beg, last - in_cap, out_beg + in_wrap - out_wrap);
                }
                break;
            }
            case 7: {
                // in : [in_cap, ..., first  , ..., last   ]
                // out: [out   , ..., out_cap, ..., out + n]
                size_t const out_wrap{static_cast<size_t>(out_cap_ptr - out)};
                return f(first - in_cap, first + out_wrap - in_cap, out) && f(first + out_wrap - in_cap, last - in_cap, out_beg);
            }
            case 12: {
                // in : [first  , ..., last, ..., in_cap ]
                // out: [out_cap, ..., out , ..., out + n]
                return f(first, last, out - out_cap);
            }
            case 13: {
                // in : [first  , ..., in_cap , ..., last   ]
                // out: [out_cap, ..., out    , ..., out + n]
                return f(first, in_cap_ptr, out - out_cap) && f(in_beg, last - in_cap, out + (in_cap_ptr - first) - out_cap);
            }
            case 15: {
                // in : [in_cap , ..., first, ..., last   ]
                // out: [out_cap, ..., out  , ..., out + n]
                return f(first - in_cap, last - in_cap, out - out_cap);
            }
            default:
                assert(false);
        }
        return true;
    }

    template <typename InputIt1, typename InputIt2>
    [[nodiscard]] RING_BUFFER_HIDE_FROM_ABI constexpr bool equal(InputIt1* first1, InputIt1* last1, InputIt2* first2) noexcept {
        for (; first1 != last1; ++first1, ++first2) {
            if (!(*first1 == *first2)) {
                return false;
            }
        }
        return true;
    }

    template <typename InputIt1, typename InputIt2>
    [[nodiscard]] RING_BUFFER_HIDE_FROM_ABI constexpr bool
    split_equal(InputIt1* first1, InputIt1* last1, InputIt1* in_beg, size_t in_cap, InputIt2* first2, InputIt2* out_beg, size_t out_cap) {
        return basic_split_result_inout(first1, last1, in_beg, in_cap, first2, out_beg, out_cap, equal<InputIt1, InputIt2>);
    }

    // move_backward

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr OutputIt* move_backward(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt* __restrict__ result) {
        while (first != last) {
            *--result = std::move(*--last);
        }
        return result;
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr OutputIt*
    split_move_backward(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap, OutputIt* __restrict__ result) {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap);
        InputIt* __restrict__ const in_cap_ptr{in_beg + in_cap};
        if (RING_BUFFER_UNLIKELY(first > in_cap_ptr)) {
            // in : [in_cap, ..., first, ..., last, ..., result]
            return move_backward(first - in_cap, last - in_cap, result - in_cap);
        } else if (RING_BUFFER_UNLIKELY(last > in_cap_ptr)) {
            // in : [first, ..., in_cap, ..., last, ..., result]
            size_t const wrap_last{static_cast<size_t>(last - in_cap_ptr)}, sz{static_cast<size_t>(in_cap_ptr - first)},
                wrap_result{static_cast<size_t>(result - last)};
            move_backward(last - in_cap - wrap_last, last - in_cap, result - in_cap);
            if (sz > wrap_result) {
                move_backward(in_cap_ptr - wrap_result, in_cap_ptr, result - in_cap);
                return move_backward(first, last - wrap_last - wrap_result, in_cap_ptr);
            } else {
                return move_backward(first, in_cap_ptr, result - in_cap - wrap_last);
            }
        } else if (RING_BUFFER_UNLIKELY(result > in_cap_ptr)) {
            // in : [first, ..., last, ..., in_cap, ..., result]
            size_t const wrap{static_cast<size_t>(result - in_cap_ptr)}, sz{static_cast<size_t>(last - first)};
            if (sz > wrap) {
                move_backward(last - wrap, last, result - in_cap);
                return move_backward(first, last - wrap, in_cap_ptr);
            } else {
                return move_backward(first, last, result - in_cap);
            }
        } else {
            // in : [first, ..., last, ..., result, ..., in_cap]
            return move_backward(first, last, result);
        }
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr OutputIt* move(InputIt* __restrict__ first, InputIt* __restrict__ last, OutputIt* __restrict__ result) {
        for (; first != last; ++first, ++result) {
            *result = std::move(*first);
        }
        return result;
    }

    template <typename InputIt, typename OutputIt>
    RING_BUFFER_HIDE_FROM_ABI constexpr OutputIt*
    split_move(InputIt* __restrict__ first, InputIt* __restrict__ last, InputIt* __restrict__ in_beg, size_t in_cap, OutputIt* __restrict__ result) {
        assert(first <= last);
        assert(static_cast<size_t>(last - first) <= in_cap);
        InputIt* __restrict__ const in_cap_ptr{in_beg + in_cap};
        if (RING_BUFFER_UNLIKELY(result > in_cap_ptr)) {
            // in : [in_cap, ..., result, ..., first, ..., last]
            return move(first - in_cap, last - in_cap, result - in_cap);
        } else if (RING_BUFFER_UNLIKELY(first > in_cap_ptr)) {
            // in : [result, ..., in_cap, ..., first, ..., last]
            size_t const wrap{static_cast<size_t>(in_cap_ptr - result)}, sz{static_cast<size_t>(last - first)};
            if (sz > wrap) {
                move(first - in_cap, first + wrap - in_cap, result);
                return move(first + wrap - in_cap, last - in_cap, in_beg);
            } else {
                return move(first - in_cap, last - in_cap, result);
            }
        } else if (RING_BUFFER_UNLIKELY(last > in_cap_ptr)) {
            // in : [result, ..., first, ..., in_cap, ..., last]
            size_t const wrap_last{static_cast<size_t>(last - in_cap_ptr)}, sz{static_cast<size_t>(first - result)},
                wrap_first{static_cast<size_t>(in_cap_ptr - first)};
            move(first, in_cap_ptr, result);
            if (sz >= wrap_last) {
                return move(in_beg, last - in_cap, result + wrap_first);
            } else {
                move(in_beg, in_beg + sz, result + wrap_first);
                return move(in_beg + sz, last - in_cap, in_beg);
            }
        } else {
            // in : [result, ..., first, ..., last, ..., in_cap]
            return move(first, last, result);
        }
    }

} // namespace wiz::details::algorithm
