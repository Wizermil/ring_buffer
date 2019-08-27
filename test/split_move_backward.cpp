#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <algorithm>
#include <array>

#include <ring_buffer/details/split_helper.hpp>

TEST_CASE("constexpr OutputIt* split_move_backward(InputIt*, InputIt*, InputIt*, size_t, OutputIt*)", "[split_move_backward]") {
    SECTION("[in_cap, ..., first, ..., last, ..., result]") {
        {
            std::array<int, 4> a{-1, 1, 2, -1};
            int *buf{a.begin()}, *tail{buf + 1}, *remove{tail};
            auto ita = wiz::details::algorithm::split_move_backward(tail + 4, remove + 4, buf, 4, remove + 1 + 4);
            REQUIRE((a == std::array<int, 4>{-1, 1, 2, -1}));
            REQUIRE(ita == buf + 2);
        }
    }

    SECTION("[first, ..., in_cap, ..., last, ..., result]") {
        {
            std::array<int, 4> a{2, 3, 4, 1};
            int *buf{a.begin()}, *tail{buf + 3}, *remove{tail + 2};
            auto ita = wiz::details::algorithm::split_move_backward(tail, remove, buf, 4, remove + 2);
            REQUIRE((a == std::array<int, 4>{2, 1, 2, 1}));
            REQUIRE(ita == buf + 1);
        }
    }

    SECTION("[first, ..., last, ..., in_cap, ..., result]") {
        {
            std::array<int, 4> a{3, -1, 1, 2};
            int *buf{a.begin()}, *tail{buf + 2}, *remove{tail + 2};
            auto ita = wiz::details::algorithm::split_move_backward(tail, remove, buf, 4, remove + 1);
            REQUIRE((a == std::array<int, 4>{2, -1, 1, 1}));
            REQUIRE(ita == buf + 3);
        }
        {
            std::array<int, 4> b{3, 4, 1, 2};
            int *buf{b.begin()}, *tail{buf + 2}, *remove{tail + 2};
            auto itb = wiz::details::algorithm::split_move_backward(tail, remove, buf, 4, remove + 2);
            REQUIRE((b == std::array<int, 4>{1, 2, 1, 2}));
            REQUIRE(itb == buf);
        }
        {
            std::array<int, 4> c{2, 3, 4, 1};
            int *buf{c.begin()}, *tail{buf + 3}, *remove{tail + 1};
            auto itc = wiz::details::algorithm::split_move_backward(tail, remove, buf, 4, remove + 2);
            REQUIRE((c == std::array<int, 4>{2, 1, 4, 1}));
            REQUIRE(itc == buf + 1);
        }
    }

    SECTION("[first, ..., last, ..., result, ..., in_cap]") {
        {
            std::array<int, 4> a{-1, 1, 2, -1};
            int *buf{a.begin()}, *tail{buf + 1}, *remove{tail};
            auto ita = wiz::details::algorithm::split_move_backward(tail, remove, buf, 4, remove + 1);
            REQUIRE((a == std::array<int, 4>{-1, 1, 2, -1}));
            REQUIRE(ita == buf + 2);
        }
        {
            std::array<int, 4> b{-1, 1, 2, -1};
            int *buf{b.begin()}, *tail{buf + 1}, *remove{tail + 1};
            auto itb = wiz::details::algorithm::split_move_backward(tail, remove, buf, 4, remove + 1);
            REQUIRE((b == std::array<int, 4>{-1, 1, 1, -1}));
            REQUIRE(itb == buf + 2);
        }
        {
            std::array<int, 4> c{-1, 1, 2, 3};
            int *buf{c.begin()}, *tail{buf + 1}, *remove{tail + 1};
            auto itc = wiz::details::algorithm::split_move_backward(tail, remove, buf, 4, remove + 2);
            REQUIRE((c == std::array<int, 4>{-1, 1, 2, 1}));
            REQUIRE(itc == buf + 3);
        }
    }
}
