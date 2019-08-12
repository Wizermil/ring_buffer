#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("ring_buffer constructor", "[ring_buffer]") {
    SECTION("ring_buffer()") {
        wiz::ring_buffer<int> a;
        REQUIRE(a.empty());
        REQUIRE(a.size() == 0);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> c;
            REQUIRE(c.empty());
            REQUIRE(c.size() == 0);
        }
        REQUIRE(ctr == 0);
        REQUIRE(cpy == 0);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 0);
    }

    SECTION("ring_buffer(size_type count, T const& value)") {
        wiz::ring_buffer<int> a{2, 1};
        REQUIRE_FALSE(a.empty());
        REQUIRE(a.size() == 2);
        REQUIRE(a[0] == 1);
        REQUIRE(a[1] == 1);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> b{3, nat{}};
            REQUIRE_FALSE(b.empty());
            REQUIRE(b.size() == 3);
            REQUIRE(b[0].cnt == 1);
            REQUIRE(b[1].cnt == 1);
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3);
    }
}
