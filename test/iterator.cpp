#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("ring_buffer iterator", "[ring_buffer]") {
    SECTION("bool operator==(_iterator const&) const noexcept") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.begin() == a.end() - 3);
        REQUIRE_FALSE(a.begin() == a.end());

        wiz::ring_buffer<int> const b(3, 1);
        REQUIRE(b.begin() == b.end() - 3);
        REQUIRE_FALSE(b.begin() == b.end());
    }

    SECTION("bool operator!=(_iterator const&) const noexcept") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.begin() != a.end());
        REQUIRE_FALSE(a.begin() != a.begin());

        wiz::ring_buffer<int> const b(3, 1);
        REQUIRE(b.begin() != b.end());
        REQUIRE_FALSE(b.begin() != b.begin());
    }

    SECTION("bool operator<(_iterator const&) const noexcept") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.begin() < a.end());
        REQUIRE(a.begin() < a.begin() + 1);
        REQUIRE_FALSE(a.begin() < a.begin());

        wiz::ring_buffer<int> const b(3, 1);
        REQUIRE(b.begin() < b.end());
        REQUIRE(b.begin() < b.begin() + 1);
        REQUIRE_FALSE(b.begin() < b.begin());
    }

    SECTION("bool operator>(_iterator const&) const noexcept") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.end() > a.begin());
        REQUIRE(a.begin() + 1 > a.begin());
        REQUIRE_FALSE(a.begin() > a.begin());

        wiz::ring_buffer<int> const b(3, 1);
        REQUIRE(b.end() > b.begin());
        REQUIRE(b.begin() + 1 > b.begin());
        REQUIRE_FALSE(b.begin() > b.begin());
    }

    SECTION("bool operator<=(_iterator const&) const noexcept") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.begin() <= a.end());
        REQUIRE(a.begin() <= a.begin() + 1);
        REQUIRE(a.begin() <= a.begin());

        wiz::ring_buffer<int> const b(3, 1);
        REQUIRE(b.begin() <= b.end());
        REQUIRE(b.begin() <= b.begin() + 1);
        REQUIRE(b.begin() <= b.begin());
    }

    SECTION("bool operator>=(_iterator const&) const noexcept") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(a.end() >= a.begin());
        REQUIRE(a.begin() + 1 >= a.begin());
        REQUIRE(a.begin() >= a.begin());

        wiz::ring_buffer<int> const b(3, 1);
        REQUIRE(b.end() >= b.begin());
        REQUIRE(b.begin() + 1 >= b.begin());
        REQUIRE(b.begin() >= b.begin());
    }

    SECTION("iterator begin() noexcept") {
        wiz::ring_buffer<int> a(3, 1);
        REQUIRE(*a.begin() == 1);
    }

    SECTION("const_iterator begin() const noexcept") {
        wiz::ring_buffer<int> const a(3, 1);
        REQUIRE(*a.begin() == 1);
    }

    SECTION("const_iterator cbegin() const noexcept") {
        wiz::ring_buffer<int> const a(3, 1);
        REQUIRE(*a.cbegin() == 1);
    }

    SECTION("iterator end() noexcept") {
        wiz::ring_buffer<int> b;
        REQUIRE(b.begin() == b.end());
    }

    SECTION("const_iterator end() const noexcept") {
        wiz::ring_buffer<int> const b;
        REQUIRE(b.begin() == b.end());
    }

    SECTION("const_iterator cend() const noexcept") {
        wiz::ring_buffer<int> const b;
        REQUIRE(b.begin() == b.cend());
    }

    SECTION("_iterator& operator++() noexcept") {
        wiz::ring_buffer<int> a;
        a.emplace_back(1);
        a.emplace_back(2);
        a.emplace_back(3);
        auto it = a.begin();
        REQUIRE(*++it == 2);
        REQUIRE(*++it == 3);
        REQUIRE(++it == a.end());
    }

    SECTION("_iterator& operator--() noexcept") {
        wiz::ring_buffer<int> a;
        a.emplace_back(1);
        a.emplace_back(2);
        a.emplace_back(3);
        auto it = a.end();
        REQUIRE(*--it == 3);
        REQUIRE(*--it == 2);
        REQUIRE(--it == a.begin());
    }

    SECTION("_iterator operator--(int) noexcept") {
        wiz::ring_buffer<int> a;
        a.emplace_back(1);
        a.emplace_back(2);
        a.emplace_back(3);
        auto it = a.end();
        REQUIRE(it-- == a.end());
        REQUIRE(*it-- == 3);
        REQUIRE(*it-- == 2);
        REQUIRE(it == a.begin());
    }

    SECTION("_iterator operator+(difference_type) noexcept") {
        {
            wiz::ring_buffer<int> a;
            a.emplace_back(1);
            a.emplace_back(2);
            a.emplace_back(3);
            auto it = a.begin();
            auto jt = it + 2;
            REQUIRE(*it == 1);
            REQUIRE(*jt == 3);
        }

        {
            wiz::ring_buffer<int> b;
            b.emplace_back(1);
            b.emplace_back(2);
            b.emplace_back(3);
            auto it = b.end();
            auto jt = it + (-2);
            REQUIRE(it == b.end());
            REQUIRE(*jt == 2);
        }
    }

    SECTION("_iterator operator-(difference_type) noexcept") {
        {
            wiz::ring_buffer<int> a;
            a.emplace_back(1);
            a.emplace_back(2);
            a.emplace_back(3);
            auto it = a.end() - 2;
            REQUIRE(*it == 2);
        }

        {
            wiz::ring_buffer<int> b;
            b.emplace_back(1);
            b.emplace_back(2);
            b.emplace_back(3);
            auto it = b.begin() - (-2);
            REQUIRE(*it == 3);
        }
    }

    SECTION("_iterator& operator+=(difference_type) noexcept") {
        {
            wiz::ring_buffer<int> a;
            a.emplace_back(1);
            a.emplace_back(2);
            a.emplace_back(3);
            auto it = a.begin();
            it += 2;
            REQUIRE(*it == 3);
        }

        {
            wiz::ring_buffer<int> b;
            b.emplace_back(1);
            b.emplace_back(2);
            b.emplace_back(3);
            auto it = b.end();
            it += -2;
            REQUIRE(*it == 2);
        }
    }

    SECTION("_iterator& operator-=(difference_type) noexcept") {
        {
            wiz::ring_buffer<int> a;
            a.emplace_back(1);
            a.emplace_back(2);
            a.emplace_back(3);
            auto it = a.end();
            it -= 2;
            REQUIRE(*it == 2);
        }

        {
            wiz::ring_buffer<int> b;
            b.emplace_back(1);
            b.emplace_back(2);
            b.emplace_back(3);
            auto it = b.begin();
            it -= -2;
            REQUIRE(*it == 3);
        }
    }
}
