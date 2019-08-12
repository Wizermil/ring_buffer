#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include <cstddef>

namespace wiz {

    template <typename T>
    void access_seq(benchmark::State& state) noexcept {
        T const data(static_cast<typename T::size_type>(state.range(0)), static_cast<typename T::value_type>(0xFFFFFFFF));
        for (auto _ : state) {
            for (std::size_t i = 0, max = static_cast<std::size_t>(state.range(0)); i < max; ++i) {
                benchmark::DoNotOptimize(data[i]);
            }
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T>
    void access_rnd(benchmark::State& state) noexcept {
        T const data(static_cast<typename T::size_type>(state.range(0)), static_cast<typename T::value_type>(0xFFFFFFFF));

        std::size_t const max{static_cast<std::size_t>(state.range(0))};
        std::vector<std::size_t> rnd;
        rnd.reserve(max);
        for (std::size_t i = 0; i < max; ++i) {
            if (i % 2 == 0) {
                rnd.emplace_back(i);
            } else {
                rnd.emplace_back(max - 1 - i);
            }
        }

        for (auto _ : state) {
            for (std::size_t i = 0; i < max; ++i) {
                benchmark::DoNotOptimize(data[rnd[i]]);
            }
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
