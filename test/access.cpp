#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("ring_buffer access", "[ring_buffer]") {
    SECTION("reference at(size_type pos)") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.at(0) == 1);
        REQUIRE(a.at(1) == 1);
        REQUIRE(a.at(2) == 1);
        a.at(0) = 2;
        REQUIRE(a.at(0) == 2);
        REQUIRE(a.at(1) == 1);
        REQUIRE(a.at(2) == 1);
    }
    SECTION("const_reference at(size_type pos) const") {
        wiz::ring_buffer<int> const a(3, 1);
        REQUIRE(a.at(0) == 1);
        REQUIRE(a.at(1) == 1);
        REQUIRE(a.at(2) == 1);
    }
    SECTION("reference operator[](size_type pos)") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a[0] == 1);
        REQUIRE(a[1] == 1);
        REQUIRE(a[2] == 1);
        a[0] = 2;
        REQUIRE(a[0] == 2);
        REQUIRE(a[1] == 1);
        REQUIRE(a[2] == 1);
    }
    SECTION("const_reference operator[](size_type pos) const") {
        wiz::ring_buffer<int> const a(3, 1);
        REQUIRE(a[0] == 1);
        REQUIRE(a[1] == 1);
        REQUIRE(a[2] == 1);
    }
    SECTION("reference front()") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.front() == 1);
        a.at(0) = 2;
        REQUIRE(a.front() == 2);
    }
    SECTION("const_reference front() const") {
        wiz::ring_buffer<int> const a(3, 1);
        REQUIRE(a.front() == 1);
    }
    SECTION("reference back()") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.back() == 1);
        a[2] = 2;
        REQUIRE(a.back() == 2);
    }
    SECTION("const_reference back() const") {
        wiz::ring_buffer<int> const a(3, 1);
        REQUIRE(a.back() == 1);
    }
}
