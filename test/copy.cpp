#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("ring_buffer copy", "[ring_buffer]") {
    SECTION("ring_buffer(ring_buffer const& other)") {
        wiz::ring_buffer<int> a{2, 1};
        wiz::ring_buffer<int> b{a};
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 2);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 1);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> c{3, nat{}};
            wiz::ring_buffer<nat> d{c};
            REQUIRE_FALSE(d.empty());
            REQUIRE(d.size() == 3);
            REQUIRE(d[0].cnt == 1);
            REQUIRE(d[1].cnt == 1);
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 3 + 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3 + 3);

        wiz::ring_buffer<int> e{};
        wiz::ring_buffer<int> f{e};
        REQUIRE(f.empty());
        REQUIRE(f.size() == 0);
    }

    SECTION("ring_buffer& operator=(ring_buffer const& other)") {
        wiz::ring_buffer<int> const a{2, 1};
        wiz::ring_buffer<int> b = a;
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 2);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 1);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> c{3, nat{}};
            wiz::ring_buffer<nat> d = c;
            REQUIRE_FALSE(d.empty());
            REQUIRE(d.size() == 3);
            REQUIRE(d[0].cnt == 1);
            REQUIRE(d[1].cnt == 1);
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 3 + 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3 + 3);

        wiz::ring_buffer<int> const e{};
        wiz::ring_buffer<int> f = e;
        REQUIRE(f.empty());
        REQUIRE(f.size() == 0);

        wiz::ring_buffer<int> g{2, 1};
        wiz::ring_buffer<int> const h;
        g = h;
        REQUIRE(g.empty());
        REQUIRE(g.size() == 0);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> i{2, nat{}};
            wiz::ring_buffer<nat> const j;
            i = j;
            REQUIRE(i.empty());
            REQUIRE(i.size() == 0);
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 2);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 2);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> k{2, nat{}};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
            // UB ring_buffer doesn't handle this case
            k = k;
#pragma clang diagnostic pop
            REQUIRE_FALSE(k.empty());
            REQUIRE(k.size() == 2);
            REQUIRE(k[0].cnt == 1);
            REQUIRE(k[1].cnt == 1);
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 2);
        REQUIRE(cpy_assign == 2);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 2);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> l{7, nat{}};
            wiz::ring_buffer<nat> const m{5, nat{}};
            l = m;
            REQUIRE_FALSE(l.empty());
            REQUIRE(l.size() == 5);
            REQUIRE(l[0].cnt == 2);
            REQUIRE(l[1].cnt == 2);
            REQUIRE(l[2].cnt == 2);
            REQUIRE(l[3].cnt == 2);
            REQUIRE(l[4].cnt == 2);
        }
        REQUIRE(ctr == 2);
        REQUIRE(cpy == 5 + 7);
        REQUIRE(cpy_assign == 5);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 2 + 5 + 7);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> n{5, nat{}};
            wiz::ring_buffer<nat> const o{7, nat{}};
            n = o;
            REQUIRE_FALSE(n.empty());
            REQUIRE(n.size() == 7);
            REQUIRE(n[0].cnt == 2);
            REQUIRE(n[1].cnt == 2);
            REQUIRE(n[2].cnt == 2);
            REQUIRE(n[3].cnt == 2);
            REQUIRE(n[4].cnt == 2);
            REQUIRE(n[5].cnt == 2);
            REQUIRE(n[6].cnt == 2);
        }
        REQUIRE(ctr == 2);
        REQUIRE(cpy == 5 + 7 + (7 - 5));
        REQUIRE(cpy_assign == 5);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 2 + 5 + 7 + 2);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> p{5, nat{}};
            wiz::ring_buffer<nat> const q{5, nat{}};
            p = q;
            REQUIRE_FALSE(p.empty());
            REQUIRE(p.size() == 5);
            REQUIRE(p[0].cnt == 2);
            REQUIRE(p[1].cnt == 2);
            REQUIRE(p[2].cnt == 2);
            REQUIRE(p[3].cnt == 2);
            REQUIRE(p[4].cnt == 2);
        }
        REQUIRE(ctr == 2);
        REQUIRE(cpy == 5 + 5);
        REQUIRE(cpy_assign == 5);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 2 + 5 + 5);
    }
}
