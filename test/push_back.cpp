#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("void push_back(T const& value)", "[ring_buffer]") {
    wiz::ring_buffer<int> a;
    REQUIRE(a.capacity() == 0);
    REQUIRE(a.size() == 0);
    REQUIRE(a.empty());
    int const value1{1};
    a.push_back(value1);
    REQUIRE(a.capacity() == 1);
    REQUIRE(a.size() == 1);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a[0] == 1);
    int const value2{2};
    a.push_back(value2);
    REQUIRE(a.capacity() == 2);
    REQUIRE(a.size() == 2);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 2);
    int const value3{3};
    a.push_back(value3);
    REQUIRE(a.capacity() == 4);
    REQUIRE(a.size() == 3);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 2);
    REQUIRE(a[2] == 3);

    wiz::ring_buffer<int> b{3, 0};
    REQUIRE(b.capacity() == 4);
    REQUIRE(b.size() == 3);
    REQUIRE_FALSE(b.empty());
    b.push_back(value1);
    REQUIRE(b.capacity() == 4);
    REQUIRE(b.size() == 4);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b[0] == 0);
    REQUIRE(b[1] == 0);
    REQUIRE(b[2] == 0);
    REQUIRE(b[3] == 1);
    b.push_back(value2);
    REQUIRE(b.capacity() == 8);
    REQUIRE(b.size() == 5);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b[0] == 0);
    REQUIRE(b[1] == 0);
    REQUIRE(b[2] == 0);
    REQUIRE(b[3] == 1);
    REQUIRE(b[4] == 2);
}

TEST_CASE("void push_back(T&& value)", "[ring_buffer]") {
    wiz::ring_buffer<int> a;
    REQUIRE(a.capacity() == 0);
    REQUIRE(a.size() == 0);
    REQUIRE(a.empty());
    a.push_back(1);
    REQUIRE(a.capacity() == 1);
    REQUIRE(a.size() == 1);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a[0] == 1);
    a.push_back(2);
    REQUIRE(a.capacity() == 2);
    REQUIRE(a.size() == 2);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 2);
    a.push_back(3);
    REQUIRE(a.capacity() == 4);
    REQUIRE(a.size() == 3);
    REQUIRE_FALSE(a.empty());
    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 2);
    REQUIRE(a[2] == 3);

    wiz::ring_buffer<int> b{3, 0};
    REQUIRE(b.capacity() == 4);
    REQUIRE(b.size() == 3);
    REQUIRE_FALSE(b.empty());
    b.push_back(1);
    REQUIRE(b.capacity() == 4);
    REQUIRE(b.size() == 4);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b[0] == 0);
    REQUIRE(b[1] == 0);
    REQUIRE(b[2] == 0);
    REQUIRE(b[3] == 1);
    b.push_back(2);
    REQUIRE(b.capacity() == 8);
    REQUIRE(b.size() == 5);
    REQUIRE_FALSE(b.empty());
    REQUIRE(b[0] == 0);
    REQUIRE(b[1] == 0);
    REQUIRE(b[2] == 0);
    REQUIRE(b[3] == 1);
    REQUIRE(b[4] == 2);
}
