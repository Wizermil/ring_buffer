#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("void resize(size_type count)", "[ring_buffer]") {
    wiz::ring_buffer<int> a;
    REQUIRE(a.capacity() == 0);
    a.resize(1);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a.size() == 1);
    REQUIRE(a.capacity() == 1);
    REQUIRE(a[0] == 0);
    a.resize(0);
    REQUIRE(a.empty());
    REQUIRE(a.size() == 0);
    REQUIRE(a.capacity() == 1);

    wiz::ring_buffer<int> b(3, 1);
    REQUIRE(b.capacity() == 4);
    b.resize(4);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b.size() == 4);
    REQUIRE(b.capacity() == 4);
    REQUIRE(b[0] == 1);
    REQUIRE(b[1] == 1);
    REQUIRE(b[2] == 1);
    REQUIRE(b[3] == 0);
    b.resize(5);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b.size() == 5);
    REQUIRE(b.capacity() == 8);
    REQUIRE(b[0] == 1);
    REQUIRE(b[1] == 1);
    REQUIRE(b[2] == 1);
    REQUIRE(b[3] == 0);
    REQUIRE(b[4] == 0);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> d(3, nat{});
        REQUIRE(d.capacity() == 4);
        d.resize(4);
        REQUIRE_FALSE(d.empty());
        REQUIRE(d.size() == 4);
        REQUIRE(d.capacity() == 4);
        REQUIRE(d[0].cnt == 1);
        REQUIRE(d[1].cnt == 1);
        REQUIRE(d[2].cnt == 1);
        REQUIRE(d[3].cnt == 2);
        REQUIRE(ctr == 1 + 1);
        REQUIRE(cpy == 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1);
        d.resize(5);
        REQUIRE_FALSE(d.empty());
        REQUIRE(d.size() == 5);
        REQUIRE(d.capacity() == 8);
        REQUIRE(d[0].cnt == 1);
        REQUIRE(d[1].cnt == 1);
        REQUIRE(d[2].cnt == 1);
        REQUIRE(d[3].cnt == 2);
        REQUIRE(d[4].cnt == 3);
    }
    REQUIRE(ctr == 1 + 1 + 1);
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 4);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 1 + 4 + 5);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> e(3, nat{});
        e.pop_front();
        e.push_back(nat{});
        e.resize(4);
        REQUIRE_FALSE(e.empty());
        REQUIRE(e.size() == 4);
        REQUIRE(e.capacity() == 4);
        REQUIRE(e[0].cnt == 1);
        REQUIRE(e[1].cnt == 1);
        REQUIRE(e[2].cnt == 2);
        REQUIRE(e[3].cnt == 3);
    }
    REQUIRE(ctr == 2 + (4 - 3));
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 1);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 2 + 1 + 4);

    wiz::ring_buffer<int> f(4, 1);
    f.pop_front();
    f.push_back(2);
    f.pop_front();
    f.pop_front();
    f.resize(4);
    REQUIRE_FALSE(f.empty());
    REQUIRE(f.size() == 4);
    REQUIRE(f.capacity() == 4);
    REQUIRE(f[0] == 1);
    REQUIRE(f[1] == 2);
    REQUIRE(f[2] == 0);
    REQUIRE(f[3] == 0);

    wiz::ring_buffer<int> g(4, 1);
    g.reserve(8);
    g.pop_front();
    g.pop_front();
    g.pop_front();
    g.push_back(4);
    g.push_back(5);
    g.resize(7);
    REQUIRE_FALSE(g.empty());
    REQUIRE(g.size() == 7);
    REQUIRE(g.capacity() == 8);
    REQUIRE(g[0] == 1);
    REQUIRE(g[1] == 4);
    REQUIRE(g[2] == 5);
    REQUIRE(g[3] == 0);
    REQUIRE(g[4] == 0);
    REQUIRE(g[5] == 0);
    REQUIRE(g[6] == 0);

    wiz::ring_buffer<int> h(4, 1);
    h.reserve(8);
    h.pop_front();
    h.pop_front();
    h.pop_front();
    h.push_back(4);
    h.push_back(5);
    h.resize(5);
    REQUIRE_FALSE(h.empty());
    REQUIRE(h.size() == 5);
    REQUIRE(h.capacity() == 8);
    REQUIRE(h[0] == 1);
    REQUIRE(h[1] == 4);
    REQUIRE(h[2] == 5);
    REQUIRE(h[3] == 0);
    REQUIRE(h[4] == 0);

    wiz::ring_buffer<int> i(4, 1);
    i.reserve(8);
    i.pop_front();
    i.pop_front();
    i.pop_front();
    i.push_back(4);
    i.push_back(5);
    i.resize(4);
    REQUIRE_FALSE(i.empty());
    REQUIRE(i.size() == 4);
    REQUIRE(i.capacity() == 8);
    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 4);
    REQUIRE(i[2] == 5);
    REQUIRE(i[3] == 0);

    wiz::ring_buffer<int> j(4, 1);
    j.reserve(8);
    j.pop_front();
    j.pop_front();
    j.pop_front();
    j.push_back(4);
    j.push_back(5);
    j.push_back(6);
    j.push_back(7);
    j.resize(4);
    REQUIRE_FALSE(j.empty());
    REQUIRE(j.size() == 4);
    REQUIRE(j.capacity() == 8);
    REQUIRE(j[0] == 1);
    REQUIRE(j[1] == 4);
    REQUIRE(j[2] == 5);
    REQUIRE(j[3] == 6);

    wiz::ring_buffer<int> k(4, 0);
    k.reserve(8);
    k.pop_front();
    k.pop_front();
    k.pop_front();
    k.push_back(4);
    k.push_back(5);
    k.push_back(6);
    k.push_back(7);
    k.push_back(8);
    k.push_back(9);
    k.push_back(10);
    k.resize(7);
    REQUIRE_FALSE(k.empty());
    REQUIRE(k.size() == 7);
    REQUIRE(k.capacity() == 8);
    REQUIRE(k[0] == 0);
    REQUIRE(k[1] == 4);
    REQUIRE(k[2] == 5);
    REQUIRE(k[3] == 6);
    REQUIRE(k[4] == 7);
    REQUIRE(k[5] == 8);
    REQUIRE(k[6] == 9);
}

TEST_CASE("void resize(size_type, value_type const&)", "[ring_buffer]") {
    {
        wiz::ring_buffer<int> a;
        REQUIRE(a.capacity() == 0);
        a.resize(1, 42);
        REQUIRE_FALSE(a.empty());
        REQUIRE(a.size() == 1);
        REQUIRE(a.capacity() == 1);
        REQUIRE(a[0] == 42);
        a.resize(0);
        REQUIRE(a.empty());
        REQUIRE(a.size() == 0);
        REQUIRE(a.capacity() == 1);
    }

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> a;
        REQUIRE(a.capacity() == 0);
        a.resize(1, nat{42});
        REQUIRE_FALSE(a.empty());
        REQUIRE(a.size() == 1);
        REQUIRE(a.capacity() == 1);
        REQUIRE(a[0].cnt == 42);
        a.resize(0);
        REQUIRE(a.empty());
        REQUIRE(a.size() == 0);
        REQUIRE(a.capacity() == 1);
    }
    REQUIRE(ctr == 1);
    REQUIRE(cpy == 1);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 2);
}
