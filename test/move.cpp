#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("ring_buffer move", "[ring_buffer]") {
    SECTION("ring_buffer(ring_buffer&& other) noexcept") {
        wiz::ring_buffer<int> a{2, 1};
        wiz::ring_buffer<int> b{std::move(a)};
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 2);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 1);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> c{3, nat{}};
            wiz::ring_buffer<nat> d{std::move(c)};
            REQUIRE_FALSE(d.empty());
            REQUIRE(d.size() == 3);
            REQUIRE(d[0].cnt == 1);
            REQUIRE(d[1].cnt == 1);
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3);

        wiz::ring_buffer<int> e{};
        wiz::ring_buffer<int> f{e};
        REQUIRE(f.empty());
        REQUIRE(f.size() == 0);
    }

    SECTION("ring_buffer& operator=(ring_buffer&& other) noexcept") {
        wiz::ring_buffer<int> a{2, 1};
        wiz::ring_buffer<int> b = std::move(a);
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 2);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 1);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> c{3, nat{}};
            wiz::ring_buffer<nat> d = std::move(c);
            REQUIRE_FALSE(d.empty());
            REQUIRE(d.size() == 3);
            REQUIRE(d[0].cnt == 1);
            REQUIRE(d[1].cnt == 1);
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 3);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 3);

        wiz::ring_buffer<int> e{};
        wiz::ring_buffer<int> f = std::move(e);
        REQUIRE(f.empty());
        REQUIRE(f.size() == 0);

        wiz::ring_buffer<int> g;
        wiz::ring_buffer<int> h{2, 1};
        h = std::move(g);
        REQUIRE(h.empty());
        REQUIRE(h.size() == 0);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> i;
            wiz::ring_buffer<nat> j{2, nat{}};
            j = std::move(i);
            REQUIRE(h.empty());
            REQUIRE(h.size() == 0);
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 2);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 3);

        {
            reset_static_nat_counter();
            wiz::ring_buffer<nat> k{2, nat{}};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-move"
            // UB ring_buffer doesn't handle this case
            k = std::move(k);
#pragma clang diagnostic pop
        }
        REQUIRE(ctr == 1);
        REQUIRE(cpy == 2);
        REQUIRE(cpy_assign == 0);
        REQUIRE(mv == 0);
        REQUIRE(mv_assign == 0);
        REQUIRE(dtr == 1 + 2);
    }
}
