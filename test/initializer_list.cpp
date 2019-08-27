#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("ring_buffer(std::initializer_list<T> init)") {
    wiz::ring_buffer<int> a{1, 1};
    REQUIRE_FALSE(a.empty());
    REQUIRE(a.size() == 2);
    REQUIRE(a.capacity() == 2);
    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 1);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> b{nat{}, nat{}, nat{}};
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.capacity() == 4);
        REQUIRE(b.size() == 3);
        REQUIRE(b[0].cnt == 1);
        REQUIRE(b[1].cnt == 2);
        REQUIRE(b[2].cnt == 3);
    }
    REQUIRE(ctr == 3);
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 6);

    wiz::ring_buffer<int> c{std::initializer_list<int>{}};
    REQUIRE(c.empty());
    REQUIRE(c.size() == 0);
    REQUIRE(c.capacity() == 0);
}

TEST_CASE("ring_buffer& operator=(std::initializer_list<T> ilist)") {
    wiz::ring_buffer<int> a = {1, 1};
    REQUIRE_FALSE(a.empty());
    REQUIRE(a.capacity() == 2);
    REQUIRE(a.size() == 2);
    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 1);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> b = {nat{}, nat{}, nat{}, nat{}};
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.capacity() == 4);
        REQUIRE(b.size() == 4);
        REQUIRE(b[0].cnt == 1);
        REQUIRE(b[1].cnt == 2);
        REQUIRE(b[2].cnt == 3);
        REQUIRE(b[3].cnt == 4);
    }
    REQUIRE(ctr == 4);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 8);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> c(4, nat{});
        c = {nat{}, nat{}, nat{}};
        REQUIRE_FALSE(c.empty());
        REQUIRE(c.capacity() == 4);
        REQUIRE(c.size() == 3);
        REQUIRE(c[0].cnt == 2);
        REQUIRE(c[1].cnt == 3);
        REQUIRE(c[2].cnt == 4);
    }
    REQUIRE(ctr == 4);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 3);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 8);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> d(4, nat{});
        d = {nat{}};
        REQUIRE_FALSE(d.empty());
        REQUIRE(d.capacity() == 4);
        REQUIRE(d.size() == 1);
        REQUIRE(d[0].cnt == 2);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 1);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 6);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> e(4, nat{});
        e = {nat{}, nat{}, nat{}, nat{}, nat{}};
        REQUIRE_FALSE(e.empty());
        REQUIRE(e.capacity() == 8);
        REQUIRE(e.size() == 5);
        REQUIRE(e[0].cnt == 2);
        REQUIRE(e[1].cnt == 3);
        REQUIRE(e[2].cnt == 4);
        REQUIRE(e[3].cnt == 5);
        REQUIRE(e[4].cnt == 6);
    }
    REQUIRE(ctr == 6);
    REQUIRE(cpy == 4 + 5);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 15);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> f(4, nat{});
        f.pop_front();
        f = {nat{}};
        REQUIRE_FALSE(f.empty());
        REQUIRE(f.capacity() == 4);
        REQUIRE(f.size() == 1);
        REQUIRE(f[0].cnt == 2);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 1);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 6);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> g(4, nat{});
        g.pop_front();
        g.pop_front();
        g = {nat{}};
        REQUIRE_FALSE(g.empty());
        REQUIRE(g.capacity() == 4);
        REQUIRE(g.size() == 1);
        REQUIRE(g[0].cnt == 2);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 1);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 6);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> h(4, nat{});
        h.pop_front();
        h.pop_front();
        h.pop_front();
        h = {nat{}};
        REQUIRE_FALSE(h.empty());
        REQUIRE(h.capacity() == 4);
        REQUIRE(h.size() == 1);
        REQUIRE(h[0].cnt == 2);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 1);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 6);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> i(4, nat{});
        i.pop_front();
        i.pop_front();
        i.pop_front();
        i.pop_front();
        i = {nat{}};
        REQUIRE_FALSE(i.empty());
        REQUIRE(i.capacity() == 4);
        REQUIRE(i.size() == 1);
        REQUIRE(i[0].cnt == 2);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 4 + 1);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 7);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> j(3, nat{});
        j.pop_front();
        j.push_back(nat{});
        j = {nat{}};
        REQUIRE_FALSE(j.empty());
        REQUIRE(j.capacity() == 4);
        REQUIRE(j.size() == 1);
        REQUIRE(j[0].cnt == 3);
    }
    REQUIRE(ctr == 3);
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 1);
    REQUIRE(mv == 1);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 7);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> k(4, nat{});
        k.pop_front();
        k.push_back(nat{});
        k.pop_front();
        k.pop_front();
        k.push_back(nat{});
        k = {nat{}};
        REQUIRE_FALSE(k.empty());
        REQUIRE(k.capacity() == 4);
        REQUIRE(k.size() == 1);
        REQUIRE(k[0].cnt == 4);
    }
    REQUIRE(ctr == 4);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 1);
    REQUIRE(mv == 2);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 10);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> l(4, nat{});
        l.pop_front();
        l.push_back(nat{});
        l.pop_front();
        l.pop_front();
        l.push_back(nat{});
        l = {nat{}, nat{}, nat{}};
        REQUIRE_FALSE(l.empty());
        REQUIRE(l.capacity() == 4);
        REQUIRE(l.size() == 3);
        REQUIRE(l[0].cnt == 4);
        REQUIRE(l[1].cnt == 5);
        REQUIRE(l[2].cnt == 6);
    }
    REQUIRE(ctr == 6);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 3);
    REQUIRE(mv == 2);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 12);
}
