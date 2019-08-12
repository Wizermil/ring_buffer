#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include <cstddef>

namespace wiz {

    template <typename T>
    void constructor_default(benchmark::State& state) noexcept {
        for (auto _ : state) {
            T const data;
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T>
    void constructor_size_copy(benchmark::State& state) noexcept {
        for (auto _ : state) {
            T const data(static_cast<typename T::size_type>(state.range(0)), static_cast<typename T::value_type>(0xFFFFFFFF));
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T>
    void constructor_size(benchmark::State& state) noexcept {
        for (auto _ : state) {
            T const data(static_cast<typename T::size_type>(state.range(0)));
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
