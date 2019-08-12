#pragma once

#include <type_traits>

namespace wiz::details::bit {

    template <typename T>
    using bitop_unsigned_integer = std::integral_constant<bool,
                                                          std::is_integral_v<T> && std::is_unsigned_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool> &&
                                                              !std::is_same_v<std::remove_cv_t<T>, signed char> && !std::is_same_v<std::remove_cv_t<T>, wchar_t> &&
                                                              !std::is_same_v<std::remove_cv_t<T>, char16_t> && !std::is_same_v<std::remove_cv_t<T>, char32_t>>;

}
