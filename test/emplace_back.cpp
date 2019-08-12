#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("void emplace_back(Args&&... args)", "[ring_buffer]") {
    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> a;
        REQUIRE(a.empty());
        REQUIRE(a.size() == 0);
        REQUIRE(a.capacity() == 0);
        a.emplace_back(1);
        REQUIRE_FALSE(a.empty());
        REQUIRE(a.size() == 1);
        REQUIRE(a.capacity() == 1);
        REQUIRE(a[0].cnt == 1);
        a.emplace_back(2, 3);
        REQUIRE_FALSE(a.empty());
        REQUIRE(a.size() == 2);
        REQUIRE(a.capacity() == 2);
        REQUIRE(a[0].cnt == 1);
        REQUIRE(a[1].cnt == 5);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 0);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 1);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 3);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> b{4, nat{}};
        b.pop_front();
        b.pop_front();
        b.pop_front();
        b.pop_front();
        b.push_back(nat{});
        b.push_back(nat{});
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 2);
        REQUIRE(b.capacity() == 4);
        REQUIRE(b[0].cnt == 2);
        REQUIRE(b[1].cnt == 3);
        b.emplace_back(1);
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 3);
        REQUIRE(b.capacity() == 4);
        REQUIRE(b[0].cnt == 2);
        REQUIRE(b[1].cnt == 3);
        REQUIRE(b[2].cnt == 1);
        b.emplace_back(2, 3);
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 4);
        REQUIRE(b.capacity() == 4);
        REQUIRE(b[0].cnt == 2);
        REQUIRE(b[1].cnt == 3);
        REQUIRE(b[2].cnt == 1);
        REQUIRE(b[3].cnt == 5);
    }
    REQUIRE(ctr == 3 + 2);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 2);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 5 + 4 + 2);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> c{4, nat{}};
        c.pop_front();
        c.pop_front();
        c.push_back(nat{});
        c.push_back(nat{});
        REQUIRE_FALSE(c.empty());
        REQUIRE(c.size() == 4);
        REQUIRE(c.capacity() == 4);
        REQUIRE(c[0].cnt == 1);
        REQUIRE(c[1].cnt == 1);
        REQUIRE(c[2].cnt == 2);
        REQUIRE(c[3].cnt == 3);
        c.emplace_back(4);
        REQUIRE_FALSE(c.empty());
        REQUIRE(c.size() == 5);
        REQUIRE(c.capacity() == 8);
        REQUIRE(c[0].cnt == 1);
        REQUIRE(c[1].cnt == 1);
        REQUIRE(c[2].cnt == 2);
        REQUIRE(c[3].cnt == 3);
        REQUIRE(c[4].cnt == 4);
        c.emplace_back(2, 3);
        REQUIRE_FALSE(c.empty());
        REQUIRE(c.size() == 6);
        REQUIRE(c.capacity() == 8);
        REQUIRE(c[0].cnt == 1);
        REQUIRE(c[1].cnt == 1);
        REQUIRE(c[2].cnt == 2);
        REQUIRE(c[3].cnt == 3);
        REQUIRE(c[4].cnt == 4);
        REQUIRE(c[5].cnt == 5);
    }
    REQUIRE(ctr == 3 + 2);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 2 + 4);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 5 + 4 + 6);
}
