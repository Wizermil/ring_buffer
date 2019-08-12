#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("size_type size() const noexcept", "[ring_buffer]") {
    wiz::ring_buffer<int> a{3, 1};
    REQUIRE(a.size() == 3);

    wiz::ring_buffer<int> b;
    REQUIRE(b.size() == 0);

    wiz::ring_buffer<int> c{2, 1};
    REQUIRE(c.size() == 2);
}
