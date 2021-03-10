#include <cstddef>
#include <algorithm>
#include <random>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

static inline std::size_t get_random(std::size_t a, std::size_t b) noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dis(a, b);
    return dis(gen);
}

TEST_CASE("fuzz testing", "[ring_buffer]") {

    // test void ring_buffer<T>::pop_front()
    reset_static_nat_counter();
    for (std::size_t i = 0ul; i < 1000000ul; ++i) {
        wiz::ring_buffer<nat> a;
        std::vector<nat> b;

        int unique = 0;

        for (std::size_t j = 0ul, jmax = get_random(1ul, 10ul); j < jmax; ++j) {
            if (a.empty() || get_random(0ul, 100ul) > 50ul) { // add
                for (std::size_t k = 0ul, add_max = get_random(1ul, 10ul); k < add_max; ++k) {
                    int const to_add = ++unique;
                    a.push_back(nat{to_add});
                    b.push_back(nat{to_add});
                }
            } else { // remove
                for (std::size_t k = 0ul, remove_max = get_random(1ul, std::min(5ul, a.size())); k < remove_max; ++k) {
                    a.pop_front();
                    b.erase(b.cbegin());

                    REQUIRE((a.size() == b.size()));

                    bool equal = true;
                    for (std::size_t l = 0ul; l <  a.size(); ++l) {
                        if (a[l].cnt != b[l].cnt) {
                            equal = false;
                            break;
                        }
                    }
                    REQUIRE(equal);
                }
            }
        }
    }
    REQUIRE(ctr + cpy + mv == dtr);

    // test iterator ring_buffer<T>::erase(const_iterator first)
    reset_static_nat_counter();
    for (std::size_t i = 0ul; i < 1000000ul; ++i) {
        wiz::ring_buffer<nat> a;
        std::vector<nat> b;

        int unique = 0;

        for (std::size_t j = 0ul, jmax = get_random(1ul, 10ul); j < jmax; ++j) {
            if (a.empty() || get_random(0ul, 100ul) > 50ul) { // add
                for (std::size_t k = 0ul, add_max = get_random(1ul, 10ul); k < add_max; ++k) {
                    int const to_add = ++unique;
                    a.push_back(nat{to_add});
                    b.push_back(nat{to_add});
                }
            } else { // remove
                for (std::size_t k = 0ul, remove_max = get_random(1ul, std::min(5ul, a.size())); k < remove_max; ++k) {
                    std::ptrdiff_t const index_to_remove = static_cast<std::ptrdiff_t>(get_random(0ul, a.size() - 1ul));

                    a.erase(a.cbegin() + index_to_remove);
                    b.erase(b.cbegin() + index_to_remove);
                    
                    REQUIRE((a.size() == b.size()));

                    bool equal = true;
                    for (std::size_t l = 0ul; l <  a.size(); ++l) {
                        if (a[l].cnt != b[l].cnt) {
                            equal = false;
                            break;
                        }
                    }
                    REQUIRE(equal);
                }
            }
        }
    }
    REQUIRE(ctr + cpy + mv == dtr);

    // test iterator ring_buffer<T>::erase(const_iterator first, const_iterator last)
    reset_static_nat_counter();
    for (std::size_t i = 0ul; i < 1000000ul; ++i) {
        wiz::ring_buffer<nat> a;
        std::vector<nat> b;
        
        int unique = 0;
        
        for (std::size_t j = 0ul, jmax = get_random(1ul, 10ul); j < jmax; ++j) {
            if (a.empty() || get_random(0ul, 100ul) > 50ul) { // add
                for (std::size_t k = 0ul, add_max = get_random(1ul, 10ul); k < add_max; ++k) {
                    int const to_add = ++unique;
                    a.push_back(nat{to_add});
                    b.push_back(nat{to_add});
                }
            } else { // remove
                std::size_t const beg = get_random(0ul, a.size() - 1ul);
                std::size_t const end = get_random(beg, a.size());
                
                a.erase(a.cbegin() + static_cast<std::ptrdiff_t>(beg), a.cbegin() + static_cast<std::ptrdiff_t>(end));
                b.erase(b.cbegin() + static_cast<std::ptrdiff_t>(beg), b.cbegin() + static_cast<std::ptrdiff_t>(end));
                
                REQUIRE((a.size() == b.size()));
                
                bool equal = true;
                for (std::size_t l = 0ul; l <  a.size(); ++l) {
                    if (a[l].cnt != b[l].cnt) {
                        equal = false;
                        break;
                    }
                }
                REQUIRE(equal);
            }
        }
    }
    REQUIRE(ctr + cpy + mv == dtr);

    // test mix
    reset_static_nat_counter();
    for (std::size_t i = 0ul; i < 1000000ul; ++i) {
        wiz::ring_buffer<nat> a;
        std::vector<nat> b;

        int unique = 0;

        for (std::size_t j = 0, jmax = get_random(1ul, 10ul); j < jmax; ++j) {
            if (a.empty() || get_random(0ul, 100ul) > 50ul) { // add
                if (get_random(0ul, 100ul) > 90ul) {
                    std::vector<nat> to_add;
                    std::size_t add_max = get_random(1ul, 10ul);
                    to_add.reserve(add_max);
                    for (std::size_t k = 0ul; k < add_max; ++k) {
                        to_add.push_back(nat{++unique});
                    }
                    a.assign(&to_add[0ul], &to_add[add_max]);
                    b.assign(&to_add[0ul], &to_add[add_max]);
                } else {
                    for (std::size_t k = 0ul, add_max = get_random(1ul, 10ul); k < add_max; ++k) {
                        int const to_add = ++unique;
                        a.push_back(nat{to_add});
                        b.push_back(nat{to_add});
                    }
                }
            } else { // remove
                std::size_t const rnd = get_random(0ul, 100ul);

                if (rnd > 66ul) { // pop_front
                    for (std::size_t k = 0ul, remove_max = get_random(1ul, std::min(5ul, a.size())); k < remove_max; ++k) {
                        a.pop_front();
                        b.erase(b.cbegin());
                    }
                } else if (rnd > 33ul) { // erase(const_iterator)
                    for (std::size_t k = 0ul, remove_max = get_random(1, std::min(5ul, a.size())); k < remove_max; ++k) {
                        std::ptrdiff_t const index_to_remove = static_cast<std::ptrdiff_t>(get_random(0ul, a.size() - 1ul));

                        a.erase(a.cbegin() + index_to_remove);
                        b.erase(b.cbegin() + index_to_remove);
                    }
                } else { // erase(const_iterator, const_iterator)
                    std::size_t const beg = get_random(0ul, a.size() - 1ul);
                    std::size_t const end = get_random(beg, a.size());

                    a.erase(a.cbegin() + static_cast<std::ptrdiff_t>(beg), a.cbegin() + static_cast<std::ptrdiff_t>(end));
                    b.erase(b.cbegin() + static_cast<std::ptrdiff_t>(beg), b.cbegin() + static_cast<std::ptrdiff_t>(end));
                }

                REQUIRE((a.size() == b.size()));

                bool equal = true;
                for (std::size_t l = 0ul; l <  a.size(); ++l) {
                    if (a[l].cnt != b[l].cnt) {
                        equal = false;
                        break;
                    }
                }
                REQUIRE(equal);
            }
        }
    }
    REQUIRE(ctr + cpy + mv == dtr);
}
