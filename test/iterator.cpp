#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("ring_buffer iterator", "[ring_buffer]") {
    SECTION("iterator begin() noexcept") {
        wiz::ring_buffer<int> a{3, 1};
        REQUIRE(*a.begin() == 1);

        wiz::ring_buffer<int> b;
        REQUIRE(b.begin() == b.end());
    }
}
