#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <algorithm>
#include <array>
#include <utility>

#include <ring_buffer/details/split_helper.hpp>

TEST_CASE("constexpr OutputIt* split_move(InputIt*, InputIt*, InputIt*, size_t, OutputIt*)", "[split_move]") {
    SECTION("[in_cap, ..., result, ..., first, ..., last]") {
        {
            std::array<int, 4> a{-1, 1, 2, -1};
            int *buf{a.begin()}, *tail{buf + 1}, *head{tail + 2}, *remove{tail};
            auto ita = wiz::details::algorithm::split_move(remove + 1 + 4, head + 4, buf, 4, remove + 4);
            REQUIRE((a == std::array<int, 4>{-1, 2, 2, -1}));
            REQUIRE(ita == buf + 2);
        }
    }

    SECTION("[result, ..., in_cap, ..., first, ..., last]") {
        {
            std::array<int, 4> a{3, -1, 1, 2};
            int *buf{a.begin()}, *tail{buf + 2}, *head{tail + 3}, *remove{tail + 2};
            auto ita = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((a == std::array<int, 4>{3, -1, 1, 2}));
            REQUIRE(ita == buf + 4);
        }
        {
            std::array<int, 4> b{3, 4, 1, 2};
            int *buf{b.begin()}, *tail{buf + 2}, *head{tail + 4}, *remove{tail + 2};
            auto itb = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((b == std::array<int, 4>{4, 4, 1, 2}));
            REQUIRE(itb == buf + 1);
        }
        {
            std::array<int, 4> c{2, 3, 4, 1};
            int *buf{c.begin()}, *tail{buf + 3}, *head{tail + 4}, *remove{tail + 1};
            auto itc = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((c == std::array<int, 4>{3, 4, 4, 1}));
            REQUIRE(itc == buf + 2);
        }
    }

    SECTION("[result, ..., first, ..., in_cap, ..., last]") {
        {
            std::array<int, 4> a{2, 3, 4, 1};
            int *buf{a.begin()}, *tail{buf + 3}, *head{tail + 4}, *remove{tail};
            auto ita = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((a == std::array<int, 4>{3, 4, 4, 2}));
            REQUIRE(ita == buf + 2);
        }

        {
            std::array<int, 4> b{4, 1, 2, 3};
            int *buf{b.begin()}, *tail{buf + 1}, *head{tail + 4}, *remove{tail + 2};
            auto itb = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((b == std::array<int, 4>{4, 1, 2, 4}));
            REQUIRE(itb == buf + 4);
        }
    }

    SECTION("[result, ..., first, ..., last, ..., in_cap]") {
        {
            std::array<int, 4> a{-1, 1, 2, -1};
            int *buf{a.begin()}, *tail{buf + 1}, *head{tail + 2}, *remove{tail};
            auto ita = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((a == std::array<int, 4>{-1, 2, 2, -1}));
            REQUIRE(ita == buf + 2);
        }
        {
            std::array<int, 4> b{-1, 1, 2, -1};
            int *buf{b.begin()}, *tail{buf + 1}, *head{tail + 2}, *remove{tail + 1};
            auto itb = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((b == std::array<int, 4>{-1, 1, 2, -1}));
            REQUIRE(itb == buf + 2);
        }
        {
            std::array<int, 4> c{-1, 1, 2, 3};
            int *buf{c.begin()}, *tail{buf + 1}, *head{tail + 3}, *remove{tail + 1};
            auto itc = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((c == std::array<int, 4>{-1, 1, 3, 3}));
            REQUIRE(itc == buf + 3);
        }
        {
            std::array<int, 4> d{1, 2, 3, 4};
            int *buf{d.begin()}, *tail{buf}, *head{tail + 4}, *remove{tail + 1};
            auto itd = wiz::details::algorithm::split_move(remove + 1, head, buf, 4, remove);
            REQUIRE((d == std::array<int, 4>{1, 3, 4, 4}));
            REQUIRE(itd == buf + 3);
        }
    }
}
