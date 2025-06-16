#include <benchmark/benchmark.h>
#include "../src/SequenceMap.hpp"

#include <unordered_map>
#include <string>
#include <random>
#include <sstream>

constexpr int kTestMaxSize = 10000;
constexpr int kTestMinSize = 10;
constexpr int kRangeMult = 10;

std::vector<std::string> GenerateKeys(size_t n) {
    std::vector<std::string> keys;
    keys.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        keys.emplace_back("key_" + std::to_string(i));
    }
    return keys;
}

template <typename Container>
void FillContainer(Container& container, const std::vector<std::string>& keys) {
    for (size_t i = 0; i < keys.size(); ++i) {
        container.emplace_back(keys[i], static_cast<int>(i));
    }
}

template <>
void FillContainer(std::unordered_map<std::string, int>& container, const std::vector<std::string>& keys) {
    for (size_t i = 0; i < keys.size(); ++i) {
        container.emplace(keys[i], static_cast<int>(i));
    }
}

static void BM_SequenceMap_Insert_Back(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    for (auto _ : state) {
        SequenceMap<std::string, int> map;
        FillContainer(map, keys);
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(BM_SequenceMap_Insert_Back)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_UnorderedMap_Insert(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    for (auto _ : state) {
        std::unordered_map<std::string, int> map;
        FillContainer(map, keys);
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(BM_UnorderedMap_Insert)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_SequenceMap_KeyLookup(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    SequenceMap<std::string, int> map;
    FillContainer(map, keys);
    for (auto _ : state) {
        int sum = 0;
        for (const auto& key : keys) {
            sum += map.find(key)->second;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_SequenceMap_KeyLookup)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_UnorderedMap_KeyLookup(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    std::unordered_map<std::string, int> map;
    FillContainer(map, keys);
    for (auto _ : state) {
        int sum = 0;
        for (const auto& key : keys) {
            sum += map.find(key)->second;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_UnorderedMap_KeyLookup)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_SequenceMap_IdxLookup(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    SequenceMap<std::string, int> map;
    FillContainer(map, keys);
    for (auto _ : state) {
        int sum = 0;
        for (int i = 0; i < state.range(0); ++i) {
            sum += map[i].second;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_SequenceMap_IdxLookup)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_Vector_IdxLookup(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    std::vector<std::pair<std::string, int>> vec;
    FillContainer(vec, keys);
    for (auto _ : state) {
        int sum = 0;
        for (int i = 0; i < state.range(0); ++i) {
            sum += vec[i].second;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Vector_IdxLookup)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_SequenceMap_Iteration(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    SequenceMap<std::string, int> map;
    FillContainer(map, keys);
    for (auto _ : state) {
        int sum = 0;
        for (const auto& kv : map) {
            sum += kv.second;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_SequenceMap_Iteration)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_UnorderedMap_Iteration(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    std::unordered_map<std::string, int> map;
    FillContainer(map, keys);
    for (auto _ : state) {
        int sum = 0;
        for (const auto& kv : map) {
            sum += kv.second;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_UnorderedMap_Iteration)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_Vector_Iteration(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    std::vector<std::pair<std::string, int>> vec;
    FillContainer(vec, keys);
    for (auto _ : state) {
        int sum = 0;
        for (const auto& kv : vec) {
            sum += kv.second;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Vector_Iteration)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_SequenceMap_EraseKey(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        SequenceMap<std::string, int> map;
        FillContainer(map, keys);
        state.ResumeTiming();

        for (const auto& key : keys) {
            map.erase(key);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SequenceMap_EraseKey)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_UnorderedMap_EraseKey(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        std::unordered_map<std::string, int> map;
        FillContainer(map, keys);
        state.ResumeTiming();

        for (const auto& key : keys) {
            map.erase(key);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_UnorderedMap_EraseKey)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);


static void BM_SequenceMap_EraseByIterator(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        SequenceMap<std::string, int> map;
        FillContainer(map, keys);
        state.ResumeTiming();

        while (!map.empty()) {
            map.erase(map.begin());
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SequenceMap_EraseByIterator)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_Vector_EraseByIterator(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        std::vector<std::pair<std::string, int>> vec;
        FillContainer(vec, keys);
        state.ResumeTiming();

        while (!vec.empty()) {
            vec.erase(vec.begin());
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_Vector_EraseByIterator)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);


static void BM_SequenceMap_PopBack(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        SequenceMap<std::string, int> map;
        FillContainer(map, keys);
        state.ResumeTiming();

        while (!map.empty()) {
            map.pop_back();
        }
        benchmark::ClobberMemory();
    }
    auto a = 1;
}
BENCHMARK(BM_SequenceMap_PopBack)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_Vector_PopBack(benchmark::State& state) {
    const auto keys = GenerateKeys(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        std::vector<std::pair<std::string, int>> vec;
        FillContainer(vec, keys);
        state.ResumeTiming();

        while (!vec.empty()) {
            vec.pop_back();
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_Vector_PopBack)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

