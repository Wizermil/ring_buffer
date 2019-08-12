#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("[[nodiscard]] bool empty() const noexcept", "[ring_buffer]") {
    wiz::ring_buffer<int> a{3, 1};
    REQUIRE_FALSE(a.empty());

    wiz::ring_buffer<int> b;
    REQUIRE(b.empty());
}
