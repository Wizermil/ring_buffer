#include <cstddef>
#include <algorithm>
#include <random>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

static inline std::size_t get_random(std::size_t a, std::size_t b) noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dis(a, b);
    return dis(gen);
}

TEST_CASE("long run", "[ring_buffer]") {

    for (std::size_t i = 0; i < 1000000; ++i) {
        wiz::ring_buffer<std::size_t> a;
        wiz::ring_buffer<std::size_t> b;

        std::size_t unique = 0;

        for (std::size_t j = 0, jmax = get_random(1, 10); j < jmax; ++j) {
            if (a.empty() || get_random(0, 100) > 50) { // add

                if (get_random(0, 100) > 50) {
                    std::vector<std::size_t> to_add;
                    std::size_t add_max = get_random(1, 10);
                    to_add.reserve(add_max);
                    for (std::size_t k = 0; k < add_max; ++k) {
                        to_add.push_back(++unique);
                    }
                    a.assign(&to_add[0], &to_add[add_max]);
                } else {
                    for (std::size_t k = 0, add_max = get_random(1, 10); k < add_max; ++k) {
                        a.push_back(++unique);
                    }
                }
            } else { // remove
                for (std::size_t k = 0, remove_max = get_random(1, std::min(static_cast<std::size_t>(5), a.size())); k < remove_max; ++k) {
                    a.pop_front();
                }
            }

            if (b.empty() || get_random(0, 100) > 50) { // add
                for (std::size_t k = 0, add_max = get_random(1, 10); k < add_max; ++k) {
                    b.push_back(++unique);
                }
            } else { // remove
                std::ptrdiff_t beg = static_cast<std::ptrdiff_t>(get_random(0, b.size()));
                std::ptrdiff_t end = static_cast<std::ptrdiff_t>(get_random(static_cast<std::size_t>(beg), b.size()));
                b.erase(b.cbegin() + beg, b.cbegin() + end);
            }
        }

        a = b;
        REQUIRE((a == b));
    }

}
