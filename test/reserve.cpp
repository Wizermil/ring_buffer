#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("void reserve(size_type new_cap)", "[ring_buffer]") {
    wiz::ring_buffer<int> a;
    REQUIRE(a.capacity() == 0);
    a.reserve(1);
    REQUIRE(a.empty());
    REQUIRE(a.size() == 0);
    REQUIRE(a.capacity() == 1);
    a.reserve(0);
    REQUIRE(a.empty());
    REQUIRE(a.size() == 0);
    REQUIRE(a.capacity() == 1);

    wiz::ring_buffer<int> b{3, 1};
    REQUIRE(b.capacity() == 4);
    b.reserve(5);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b.size() == 3);
    REQUIRE(b.capacity() == 8);
    REQUIRE(b[0] == 1);
    REQUIRE(b[1] == 1);
    REQUIRE(b[2] == 1);
    b.reserve(9);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b.size() == 3);
    REQUIRE(b.capacity() == 16);
    REQUIRE(b[0] == 1);
    REQUIRE(b[1] == 1);
    REQUIRE(b[2] == 1);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> c;
        REQUIRE(c.capacity() == 0);
        c.reserve(1);
        REQUIRE(c.empty());
        REQUIRE(c.size() == 0);
        REQUIRE(c.capacity() == 1);
        REQUIRE(ctr == 0);
        REQUIRE(cpy == 0);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 0);
        c.reserve(0);
        REQUIRE(c.empty());
        REQUIRE(c.size() == 0);
        REQUIRE(c.capacity() == 1);
    }
    REQUIRE(ctr == 0);
    REQUIRE(cpy == 0);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 0);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> d{3, nat{}};
        REQUIRE(d.capacity() == 4);
        d.reserve(5);
        REQUIRE_FALSE(d.empty());
        REQUIRE(d.size() == 3);
        REQUIRE(d.capacity() == 8);
        REQUIRE(d[0].cnt == 1);
        REQUIRE(d[1].cnt == 1);
        REQUIRE(d[2].cnt == 1);
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 3);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3);
        d.reserve(9);
        REQUIRE_FALSE(d.empty());
        REQUIRE(d.size() == 3);
        REQUIRE(d.capacity() == 16);
        REQUIRE(d[0].cnt == 1);
        REQUIRE(d[1].cnt == 1);
        REQUIRE(d[2].cnt == 1);
    }
    REQUIRE(ctr == 1);
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 3 + 3);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 1 + 3 + 3 + 3);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat_no_move> e{3, nat_no_move{}};
        REQUIRE(e.capacity() == 4);
        e.reserve(5);
        REQUIRE_FALSE(e.empty());
        REQUIRE(e.size() == 3);
        REQUIRE(e.capacity() == 8);
        REQUIRE(e[0].cnt == 1);
        REQUIRE(e[1].cnt == 1);
        REQUIRE(e[2].cnt == 1);
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 3 + 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3);
        e.reserve(9);
        REQUIRE_FALSE(e.empty());
        REQUIRE(e.size() == 3);
        REQUIRE(e.capacity() == 16);
        REQUIRE(e[0].cnt == 1);
        REQUIRE(e[1].cnt == 1);
        REQUIRE(e[2].cnt == 1);
    }
    REQUIRE(ctr == 1);
    REQUIRE(cpy == 3 + 3 + 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 1 + 3 + 3 + 3);
}
