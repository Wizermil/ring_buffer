#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("iterator erase(const_iterator)", "[ring_buffer]") {
    {
        wiz::ring_buffer<int> a{1, 2, 3};
        a.erase(a.cbegin());
        REQUIRE(a.size() == 2);
        REQUIRE(a.capacity() == 4);
        REQUIRE(a[0] == 2);
        REQUIRE(a[1] == 3);
    }

    {
        wiz::ring_buffer<int> b{1, 2, 3};
        b.erase(b.cbegin() + 1);
        REQUIRE(b.size() == 2);
        REQUIRE(b.capacity() == 4);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 3);
    }

    {
        wiz::ring_buffer<int> c{1, 2, 3, 4};
        c.pop_front();
        c.pop_front();
        c.pop_front();
        c.push_back(5);
        c.push_back(6);
        c.push_back(7);
        c.erase(c.cbegin());
        REQUIRE(c.size() == 3);
        REQUIRE(c.capacity() == 4);
        REQUIRE(c[0] == 5);
        REQUIRE(c[1] == 6);
        REQUIRE(c[2] == 7);
    }

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> d{nat{}, nat{}, nat{}, nat{}};
        for (auto it = d.cbegin(); it != d.cend(); it = d.erase(it)) {
        }
        REQUIRE(d.size() == 0);
        REQUIRE(d.capacity() == 4);
        REQUIRE(ctr == 4);
        REQUIRE(cpy == 4);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 8);
    }
    REQUIRE(ctr == 4);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 8);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> e{nat{}, nat{}, nat{}, nat{}};
        e.erase(e.cend() - 1);
        REQUIRE(e.size() == 3);
        REQUIRE(e.capacity() == 4);
        REQUIRE(e[0].cnt == 1);
        REQUIRE(e[1].cnt == 2);
        REQUIRE(e[2].cnt == 3);
        REQUIRE(ctr == 4);
        REQUIRE(cpy == 4);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 5);
    }
    REQUIRE(ctr == 4);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 8);
}

TEST_CASE("iterator erase(const_iterator, const_iterator)", "[ring_buffer]") {
    {
        wiz::ring_buffer<int> a{11, 12, 13};
        a.erase(a.cbegin(), a.cend());
        REQUIRE(a.size() == 0);
        REQUIRE(a.capacity() == 4);
    }

    {
        wiz::ring_buffer<int> b{1, 2, 3};
        b.erase(b.cbegin(), b.cbegin() + 1);
        REQUIRE(b.size() == 2);
        REQUIRE(b.capacity() == 4);
        REQUIRE(b[0] == 2);
        REQUIRE(b[1] == 3);
    }

    {
        wiz::ring_buffer<int> c{1, 2, 3};
        c.erase(c.cbegin(), c.cbegin() + 2);
        REQUIRE(c.size() == 1);
        REQUIRE(c.capacity() == 4);
        REQUIRE(c[0] == 3);
    }

    {
        wiz::ring_buffer<int> d{1, 2, 3};
        d.erase(d.cbegin() + 1, d.cend());
        REQUIRE(d.size() == 1);
        REQUIRE(d.capacity() == 4);
        REQUIRE(d[0] == 1);
    }

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> e{nat{}, nat{}, nat{}};
        e.erase(e.cbegin() + 1, e.cend());
        REQUIRE(e.size() == 1);
        REQUIRE(e.capacity() == 4);
        REQUIRE(e[0].cnt == 1);
        REQUIRE(ctr == 3);
        REQUIRE(cpy == 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 3 + 2);
    }
    REQUIRE(ctr == 3);
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 6);

    {
        wiz::ring_buffer<int> f{1, 2, 3, 4};
        f.pop_front();
        f.pop_front();
        f.pop_front();
        f.push_back(5);
        f.push_back(6);
        f.push_back(7);
        f.erase(f.cbegin() + 1, f.cbegin() + 3);
        REQUIRE(f.size() == 2);
        REQUIRE(f.capacity() == 4);
        REQUIRE(f[0] == 4);
        REQUIRE(f[1] == 7);
    }
}
