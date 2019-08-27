#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <ring_buffer/ring_buffer.hpp>

#include "nat.hpp"

TEST_CASE("bool operator==(ring_buffer<T> const&, ring_buffer<T> const&)", "[ring_buffer]") {
    {
        wiz::ring_buffer<int> const a{1, 2, 3};
        wiz::ring_buffer<int> const b{1, 2, 3, 4, 5};
        REQUIRE_FALSE((a == b));
    }

    {
        wiz::ring_buffer<int> const c{1, 2, 3};
        wiz::ring_buffer<int> const d{1, 2, 3};
        REQUIRE((c == d));
    }

    {
        wiz::ring_buffer<int> const e{1, 2, 3};
        wiz::ring_buffer<int> const f{1, 2, 4};
        REQUIRE_FALSE((e == f));
    }

    {
        wiz::ring_buffer<int> const g{1, 2, 3};
        REQUIRE((g == g));
    }
}
