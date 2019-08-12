#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include <cstddef>

namespace wiz {

    template <typename T>
    void copy_assign(benchmark::State& state) noexcept {
        T const data(static_cast<typename T::size_type>(state.range(0)));
        T a(static_cast<typename T::size_type>(state.range(1)));
        for (auto _ : state) {
            a = data;
            benchmark::DoNotOptimize(a);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
