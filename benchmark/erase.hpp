#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include <cstddef>

#include "helper/get_random.hpp"

namespace wiz {

    template <typename T>
    void erase_seq(benchmark::State& state) noexcept {
        for (auto _ : state) {
            state.PauseTiming();
            T data(static_cast<typename T::size_type>(state.range(0)), static_cast<typename T::value_type>(0xFFFFFFFF));
            state.ResumeTiming();
            for (std::size_t i = 0, max = static_cast<std::size_t>(state.range(0)); i < max; ++i) {
                data.erase(data.cbegin());
            }
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T>
    void erase_rnd(benchmark::State& state) noexcept {
        std::size_t const max{static_cast<std::size_t>(state.range(0))};
        std::vector<std::size_t> rnd;
        rnd.reserve(max);
        for (std::size_t i = 0; i < max; ++i) {
            rnd.emplace_back(get_random(0, max - i - 1));
        }

        for (auto _ : state) {
            state.PauseTiming();
            T data(static_cast<typename T::size_type>(state.range(0)), static_cast<typename T::value_type>(0xFFFFFFFF));
            state.ResumeTiming();
            for (std::size_t i = 0; i < max; ++i) {
                data.erase(data.cbegin() + rnd[i]);
            }
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
