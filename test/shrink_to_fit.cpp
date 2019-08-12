#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("void shrink_to_fit()", "[ring_buffer]") {
    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> a;
        REQUIRE(a.empty());
        REQUIRE(a.size() == 0);
        REQUIRE(a.capacity() == 0);
        a.shrink_to_fit();
        REQUIRE(a.empty());
        REQUIRE(a.size() == 0);
        REQUIRE(a.capacity() == 0);
    }
    REQUIRE(ctr == 0);
    REQUIRE(cpy == 0);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 0);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> b;
        REQUIRE(b.empty());
        REQUIRE(b.size() == 0);
        REQUIRE(b.capacity() == 0);
        b.resize(8);
        b.pop_front();
        b.shrink_to_fit();
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 7);
        REQUIRE(b.capacity() == 8);
        REQUIRE(ctr == 8);
        REQUIRE(cpy == 0);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1);
        b.pop_front();
        b.pop_front();
        b.pop_front();
        b.shrink_to_fit();
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 4);
        REQUIRE(b.capacity() == 4);
        REQUIRE(ctr == 8);
        REQUIRE(cpy == 0);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 4);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3 + 4);
        b.pop_front();
        b.pop_front();
        b.shrink_to_fit();
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 2);
        REQUIRE(b.capacity() == 2);
        REQUIRE(ctr == 8);
        REQUIRE(cpy == 0);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 4 + 2);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3 + 4 + 4);
        b.pop_front();
        b.shrink_to_fit();
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 1);
        REQUIRE(b.capacity() == 1);
        REQUIRE(ctr == 8);
        REQUIRE(cpy == 0);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 4 + 2 + 1);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3 + 4 + 4 + 2);
        b.pop_front();
        b.shrink_to_fit();
        REQUIRE(b.empty());
        REQUIRE(b.size() == 0);
        REQUIRE(b.capacity() == 0);
    }
    REQUIRE(ctr == 8);
    REQUIRE(cpy == 0);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 7);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 7 + 8);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> c;
        REQUIRE(c.empty());
        REQUIRE(c.size() == 0);
        REQUIRE(c.capacity() == 0);
        c.resize(8);
        c.pop_front();
        c.pop_front();
        c.pop_front();
        c.pop_front();
        c.pop_front();
        c.pop_front();
        c.pop_front();
        c.shrink_to_fit();
        REQUIRE_FALSE(c.empty());
        REQUIRE(c.size() == 1);
        REQUIRE(c.capacity() == 1);
    }
    REQUIRE(ctr == 8);
    REQUIRE(cpy == 0);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 1);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 8 + 1);
}
