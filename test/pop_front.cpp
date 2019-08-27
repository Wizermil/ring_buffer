#include <utility>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("void pop_front()", "[ring_buffer]") {
    wiz::ring_buffer<int> a(3, 0);
    REQUIRE(a.capacity() == 4);
    REQUIRE(a.size() == 3);
    REQUIRE_FALSE(a.empty());
    a.pop_front();
    REQUIRE(a.capacity() == 4);
    REQUIRE(a.size() == 2);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a[0] == 0);
    REQUIRE(a[1] == 0);
    a.pop_front();
    REQUIRE(a.capacity() == 4);
    REQUIRE(a.size() == 1);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a[0] == 0);
    a.pop_front();
    REQUIRE(a.capacity() == 4);
    REQUIRE(a.size() == 0);
    REQUIRE(a.empty());

    wiz::ring_buffer<int> b;
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);
    REQUIRE(b.capacity() == 4);
    REQUIRE(b.size() == 3);
    REQUIRE_FALSE(b.empty());
    b.pop_front();
    REQUIRE(b.capacity() == 4);
    REQUIRE(b.size() == 2);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b[0] == 2);
    REQUIRE(b[1] == 3);
    b.pop_front();
    REQUIRE(b.capacity() == 4);
    REQUIRE(b.size() == 1);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b[0] == 3);
    b.pop_front();
    REQUIRE(b.capacity() == 4);
    REQUIRE(b.size() == 0);
    REQUIRE(b.empty());
}
