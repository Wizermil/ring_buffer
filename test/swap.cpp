#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("void swap(ring_buffer& other) noexcept", "[ring_buffer]") {
    {
        wiz::ring_buffer<int> a{11, 12, 13};
        wiz::ring_buffer<int> b{21, 22, 23, 24, 25};
        a.swap(b);
        REQUIRE_FALSE(a.empty());
        REQUIRE(a.size() == 5);
        REQUIRE(a.capacity() == 8);
        REQUIRE(a[0] == 21);
        REQUIRE(a[1] == 22);
        REQUIRE(a[2] == 23);
        REQUIRE(a[3] == 24);
        REQUIRE(a[4] == 25);
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 3);
        REQUIRE(b.capacity() == 4);
        REQUIRE(b[0] == 11);
        REQUIRE(b[1] == 12);
        REQUIRE(b[2] == 13);
    }
}

TEST_CASE("void swap(ring_buffer<T>&, ring_buffer<T>&) noexcept", "[ring_buffer]") {
    {
        wiz::ring_buffer<int> a{11, 12, 13};
        wiz::ring_buffer<int> b{21, 22, 23, 24, 25};
        wiz::swap(a, b);
        REQUIRE_FALSE(a.empty());
        REQUIRE(a.size() == 5);
        REQUIRE(a.capacity() == 8);
        REQUIRE(a[0] == 21);
        REQUIRE(a[1] == 22);
        REQUIRE(a[2] == 23);
        REQUIRE(a[3] == 24);
        REQUIRE(a[4] == 25);
        REQUIRE_FALSE(b.empty());
        REQUIRE(b.size() == 3);
        REQUIRE(b.capacity() == 4);
        REQUIRE(b[0] == 11);
        REQUIRE(b[1] == 12);
        REQUIRE(b[2] == 13);
    }
}
