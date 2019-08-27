#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <array>

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("assign(size_type count, T const& value)", "[ring_buffer]") {
    wiz::ring_buffer<int> a;
    a.assign(0, 1);
    REQUIRE(a.empty());
    REQUIRE(a.size() == 0);

    wiz::ring_buffer<int> b;
    b.assign(0, 1);
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);
    b.assign(0, 2);
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);
    b.assign(2, 3);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b.size() == 2);
    REQUIRE(b[0] == 3);
    REQUIRE(b[1] == 3);
    b.assign(1, 4);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b.size() == 1);
    REQUIRE(b[0] == 4);
    b.assign(3, 5);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 5);
    REQUIRE(b[1] == 5);
    REQUIRE(b[2] == 5);

    wiz::ring_buffer<int> c(2, 0);
    c.assign(0, 1);
    REQUIRE(c.empty());
    REQUIRE(c.size() == 0);
    c.assign(3, 2);
    REQUIRE_FALSE(c.empty());
    REQUIRE(c.size() == 3);
    REQUIRE(c[0] == 2);
    REQUIRE(c[1] == 2);
    REQUIRE(c[2] == 2);
    c.assign(0, 3);
    REQUIRE(c.empty());
    REQUIRE(c.size() == 0);
    c.assign(1, 4);
    REQUIRE_FALSE(c.empty());
    REQUIRE(c.size() == 1);
    REQUIRE(c[0] == 4);

    // increase + default => capacity increase
    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> d;
        d.assign(3, nat{});
        REQUIRE_FALSE(d.empty());
        REQUIRE(d.size() == 3);
        REQUIRE(d[0].cnt == 1);
        REQUIRE(d[1].cnt == 1);
        REQUIRE(d[2].cnt == 1);
    }
    REQUIRE(ctr == 1);
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 1 + 3);

    // decrease
    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> e(5, nat{});
        e.assign(3, nat{});
        REQUIRE_FALSE(e.empty());
        REQUIRE(e.size() == 3);
        REQUIRE(e[0].cnt == 2);
        REQUIRE(e[1].cnt == 2);
        REQUIRE(e[1].cnt == 2);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 5);
    REQUIRE(cpy_assign == 3);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 2 + (5 - 3) + 3);

    // increase
    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> f(3, nat{});
        f.assign(4, nat{});
        REQUIRE_FALSE(f.empty());
        REQUIRE(f.size() == 4);
        REQUIRE(f[0].cnt == 2);
        REQUIRE(f[1].cnt == 2);
        REQUIRE(f[2].cnt == 2);
        REQUIRE(f[3].cnt == 2);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 3 + (4 - 3));
    REQUIRE(cpy_assign == 3);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 2 + 4);

    // increase + capacity increase
    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> g(3, nat{});
        g.assign(5, nat{});
        REQUIRE_FALSE(g.empty());
        REQUIRE(g.size() == 5);
        REQUIRE(g[0].cnt == 2);
        REQUIRE(g[1].cnt == 2);
        REQUIRE(g[2].cnt == 2);
        REQUIRE(g[3].cnt == 2);
        REQUIRE(g[4].cnt == 2);
    }
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 3 + 5);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 2 + 3 + 5);

    reset_static_nat_counter();
    wiz::ring_buffer<nat> h(3, nat{});
    h.assign(0, nat{});
    REQUIRE(h.empty());
    REQUIRE(h.size() == 0);
    REQUIRE(ctr == 2);
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 2 + 3);
    reset_static_nat_counter();
    h.assign(3, nat{});
    REQUIRE_FALSE(h.empty());
    REQUIRE(h.size() == 3);
    REQUIRE(h[0].cnt == 1);
    REQUIRE(h[1].cnt == 1);
    REQUIRE(h[2].cnt == 1);
    REQUIRE(ctr == 1);
    REQUIRE(cpy == 3);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 1);
    reset_static_nat_counter();
    h.assign(0, nat{});
    REQUIRE(h.empty());
    REQUIRE(h.size() == 0);
    REQUIRE(ctr == 1);
    REQUIRE(cpy == 0);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 1 + 3);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> i;
        i.assign(0, nat{});
        REQUIRE(i.empty());
        REQUIRE(i.size() == 0);
    }
    REQUIRE(ctr == 1);
    REQUIRE(cpy == 0);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 1);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> j(3, nat{});
        j.pop_front();
        j.pop_front();
        j.pop_front();
        j.push_back(nat{});
        j.pop_front();

        wiz::ring_buffer<nat> k(3, nat{});
        k.pop_front();
        k.pop_front();
        k.push_back(nat{});
        k.push_back(nat{});
        REQUIRE_FALSE(k.empty());
        REQUIRE(k.size() == 3);
        REQUIRE(k[0].cnt == 3);
        REQUIRE(k[1].cnt == 4);
        REQUIRE(k[2].cnt == 5);

        k = j;

        REQUIRE(k.empty());
        REQUIRE(k.size() == 0);
    }
    REQUIRE(ctr == 5);
    REQUIRE(cpy == 6);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 3);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 5 + 6 + 3);

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> l(3, nat{});
        l.pop_front();
        l.pop_front();
        l.pop_front();
        l.push_back(nat{});
        l.push_back(nat{});
        REQUIRE_FALSE(l.empty());
        REQUIRE(l.size() == 2);
        REQUIRE(l[0].cnt == 2);
        REQUIRE(l[1].cnt == 3);

        wiz::ring_buffer<nat> m(3, nat{});
        m.pop_front();
        m.pop_front();
        m.push_back(nat{});
        m.push_back(nat{});
        REQUIRE_FALSE(m.empty());
        REQUIRE(m.size() == 3);
        REQUIRE(m[0].cnt == 4);
        REQUIRE(m[1].cnt == 5);
        REQUIRE(m[2].cnt == 6);

        m = l;

        REQUIRE_FALSE(m.empty());
        REQUIRE(m.size() == 2);
        REQUIRE(m[0].cnt == 2);
        REQUIRE(m[1].cnt == 3);
    }
    REQUIRE(ctr == 6);
    REQUIRE(cpy == 6);
    REQUIRE(cpy_assign == 2);
    REQUIRE(mv == 4);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 6 + 6 + 4);
}

TEST_CASE("void assign(InputIt*, InputIt*)", "[ring_buffer]") {
    {
        std::array<int, 4> const a{1, 2, 3, 4};
        wiz::ring_buffer<int> b;
        b.assign(a.begin(), a.end());
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 4);
        REQUIRE(b.capacity() == 4);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 2);
        REQUIRE(b[2] == 3);
        REQUIRE(b[3] == 4);
    }

    {
        reset_static_nat_counter();
        std::array<nat, 4> const c{};
        wiz::ring_buffer<nat> d;
        d.assign(c.begin(), c.end());
        REQUIRE_FALSE(d.empty());
        REQUIRE(d.size() == 4);
        REQUIRE(d.capacity() == 4);
        REQUIRE(d[0].cnt == 1);
        REQUIRE(d[1].cnt == 2);
        REQUIRE(d[2].cnt == 3);
        REQUIRE(d[3].cnt == 4);
    }
    REQUIRE(ctr == 4);
    REQUIRE(cpy == 4);
    REQUIRE(cpy_assign == 0);
    REQUIRE(mv == 0);
    REQUIRE(mv_assign == 0);
    REQUIRE(dtr == 8);
}

TEST_CASE("void assign(std::initializer_list<T>)", "[ring_buffer]") {
    {
        wiz::ring_buffer<int> a;
        a.assign({1, 2, 3, 4});
        REQUIRE_FALSE(a.empty());
        REQUIRE(a.size() == 4);
        REQUIRE(a.capacity() == 4);
        REQUIRE(a[0] == 1);
        REQUIRE(a[1] == 2);
        REQUIRE(a[2] == 3);
        REQUIRE(a[3] == 4);
    }

    {
        reset_static_nat_counter();
        wiz::ring_buffer<nat> b;
        b.assign({nat{}, nat{}, nat{}, nat{}});
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 4);
        REQUIRE(b.capacity() == 4);
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
}
