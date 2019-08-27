#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include <algorithm>
#include <deque>
#include <vector>

#include <ring_buffer/ring_buffer.hpp>

#include "access.hpp"
#include "constructor.hpp"
#include "copy.hpp"
#include "erase.hpp"

namespace wiz {

    template <std::size_t SIZE>
    class data {
        unsigned char* _data;

    public:
        data()
        : _data(new unsigned char[SIZE]) {}
        data(data const& other)
        : _data(new unsigned char[SIZE]) {
            std::copy(other._data, other._data + SIZE, _data);
        }
        data& operator=(data const& other) {
            std::copy(other._data, other._data + SIZE, _data);
            return *this;
        }
        data(data&& other) noexcept
        : _data{other._data} {
            other._data = nullptr;
        }
        data& operator=(data&& other) noexcept {
            _data = other._data;
            other._data = nullptr;
            return *this;
        }
        ~data() {
            if (_data != nullptr) {
                delete[] _data;
            }
        }
    };

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

    //    BENCHMARK_TEMPLATE(constructor_default, std::vector<int>);
    //    BENCHMARK_TEMPLATE(constructor_default, std::deque<int>);
    //    BENCHMARK_TEMPLATE(constructor_default, ring_buffer<int>);

    //    BENCHMARK_TEMPLATE(constructor_size_copy, std::vector<int>)->Arg(128);
    //    BENCHMARK_TEMPLATE(constructor_size_copy, std::deque<int>)->Arg(128);
    //    BENCHMARK_TEMPLATE(constructor_size_copy, ring_buffer<int>)->Arg(128);
    //
    //    BENCHMARK_TEMPLATE(constructor_size, std::vector<int>)->Arg(128);
    //    BENCHMARK_TEMPLATE(constructor_size, std::deque<int>)->Arg(128);
    //    BENCHMARK_TEMPLATE(constructor_size, ring_buffer<int>)->Arg(128);

    //    BENCHMARK_TEMPLATE(copy_assign, std::vector<int>)->Args({128, 64});
    //    BENCHMARK_TEMPLATE(copy_assign, std::deque<int>)->Args({128, 64});
    //    BENCHMARK_TEMPLATE(copy_assign, ring_buffer<int>)->Args({128, 64});

    //    BENCHMARK_TEMPLATE(copy_assign, std::vector<int>)->Args({128, 129});
    //    BENCHMARK_TEMPLATE(copy_assign, std::deque<int>)->Args({128, 129});
    //    BENCHMARK_TEMPLATE(copy_assign, ring_buffer<int>)->Args({128, 129});

    //    BENCHMARK_TEMPLATE(copy_assign, std::vector<int>)->Args({128, 64});
    //    BENCHMARK_TEMPLATE(copy_assign, std::deque<int>)->Args({128, 64});
    //    BENCHMARK_TEMPLATE(copy_assign, ring_buffer<int>)->Args({128, 64});

    BENCHMARK_TEMPLATE(erase_seq, std::vector<int>)->Arg(256);
    BENCHMARK_TEMPLATE(erase_seq, std::deque<int>)->Arg(256);
    BENCHMARK_TEMPLATE(erase_seq, ring_buffer<int>)->Arg(256);

    BENCHMARK_TEMPLATE(erase_rnd, std::vector<int>)->Arg(256);
    BENCHMARK_TEMPLATE(erase_rnd, std::deque<int>)->Arg(256);
    BENCHMARK_TEMPLATE(erase_rnd, ring_buffer<int>)->Arg(256);

    //    BENCHMARK_TEMPLATE(access_seq, std::vector<int>)
    //        ->Arg(256)
    //        ->Arg(512)
    //        ->Arg(1024)
    //        ->Arg(2048) // L1 Apple A11
    //        ->Arg(4096)
    //        ->Arg(8192) // L1 Intel i7-7920HQ
    //        ->Arg(32768) // L2 Intel i7-7920HQ
    //        ->Arg(65536)
    //        ->Arg(131072)
    //        ->Arg(262144) // L3 Intel i7-7920HQ | L2 Apple A11
    //        ->Arg(1048576);
    //    BENCHMARK_TEMPLATE(access_seq, std::deque<int>)
    //        ->Arg(256)
    //        ->Arg(512)
    //        ->Arg(1024)
    //        ->Arg(2048) // L1 Apple A11
    //        ->Arg(4096)
    //        ->Arg(8192) // L1 Intel i7-7920HQ
    //        ->Arg(32768) // L2 Intel i7-7920HQ
    //        ->Arg(65536)
    //        ->Arg(131072)
    //        ->Arg(262144) // L3 Intel i7-7920HQ | L2 Apple A11
    //        ->Arg(1048576);
    //    BENCHMARK_TEMPLATE(access_seq, ring_buffer<int>)
    //        ->Arg(256)
    //        ->Arg(512)
    //        ->Arg(1024)
    //        ->Arg(2048) // L1 Apple A11
    //        ->Arg(4096)
    //        ->Arg(8192) // L1 Intel i7-7920HQ
    //        ->Arg(32768) // L2 Intel i7-7920HQ
    //        ->Arg(65536)
    //        ->Arg(131072)
    //        ->Arg(262144) // L3 Intel i7-7920HQ | L2 Apple A11
    //        ->Arg(1048576);

    //    BENCHMARK_TEMPLATE(access_rnd, std::vector<int>)->Arg(1048576);
    //    BENCHMARK_TEMPLATE(access_rnd, std::deque<int>)->Arg(1048576);
    //    BENCHMARK_TEMPLATE(access_rnd, ring_buffer<int>)->Arg(1048576);

#pragma clang diagnostic pop
} // namespace wiz

BENCHMARK_MAIN();
