#pragma once

#include <cstddef>
#include <random>

namespace wiz {

    static inline std::size_t get_random(std::size_t a, std::size_t b) noexcept {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<std::size_t> dis(a, b);
        return dis(gen);
    }

} // namespace wiz
