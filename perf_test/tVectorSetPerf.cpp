
#include "../src/VectorSet.hpp"
#include <benchmark/benchmark.h>
#include <set>
#include <unordered_set>
#include <random>
#include <vector>
#include <string>

constexpr int kTestMaxSize = 10000;
constexpr int kTestMinSize = 10;
constexpr int kRangeMult = 10;

std::vector<std::string> generateRandomData(size_t size) {
    std::mt19937 rng(42); // fixed seed for repeatability
    std::uniform_int_distribution<int> dist(0, static_cast<int>(size) * 10); 
    std::vector<std::string> data(size);
    for (auto& x : data) x = std::string("MyData_") + std::to_string(dist(rng));
    return data; 
}

static void BM_StdSet_Insert(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    for (auto _ : state) {
        std::set<std::string> s;
        for (const auto& x : input) s.insert(x);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StdSet_Insert)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_StdUnorderedSet_Insert(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    for (auto _ : state) {
        std::unordered_set<std::string> s;
        for (const auto& x : input) s.insert(x);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StdUnorderedSet_Insert)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_VectorSet_Insert(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    for (auto _ : state) {
        VectorSet<std::string> vs;
        for (const auto& x : input) vs.insert(x);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_VectorSet_Insert)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_StdSet_Lookup(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    std::set<std::string> s(input.begin(), input.end());

    for (auto _ : state) {
        for (const auto& x : input) {
            benchmark::DoNotOptimize(s.find(x));
        }
    }
}
BENCHMARK(BM_StdSet_Lookup)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_StdUnorderedSet_Lookup(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    std::unordered_set<std::string> s(input.begin(), input.end());

    for (auto _ : state) {
        for (const auto& x : input) {
            benchmark::DoNotOptimize(s.find(x));
        }
    }
}
BENCHMARK(BM_StdUnorderedSet_Lookup)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_VectorSet_Lookup(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    VectorSet<std::string> vs(input.begin(), input.end());

    for (auto _ : state) {
        for (const auto& x : input) {
            benchmark::DoNotOptimize(vs.find(x));
        }
    }
}
BENCHMARK(BM_VectorSet_Lookup)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_StdSet_Erase(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        std::set<std::string> s(input.begin(), input.end());
        std::vector<std::string> shuffled = input;
        std::shuffle(shuffled.begin(), shuffled.end(), std::mt19937(42));
        state.ResumeTiming();

        for (const auto& x : shuffled) {
            s.erase(x);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StdSet_Erase)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_StdUnorderedSet_Erase(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        std::unordered_set<std::string> s(input.begin(), input.end());
        std::vector<std::string> shuffled = input;
        std::shuffle(shuffled.begin(), shuffled.end(), std::mt19937(42));
        state.ResumeTiming();

        for (const auto& x : shuffled) {
            s.erase(x);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StdUnorderedSet_Erase)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

static void BM_VectorSet_Erase(benchmark::State& state) {
    auto input = generateRandomData(state.range(0));
    for (auto _ : state) {
        state.PauseTiming();
        VectorSet<std::string> vs(input.begin(), input.end());
        std::vector<std::string> shuffled = input;
        std::shuffle(shuffled.begin(), shuffled.end(), std::mt19937(42));
        state.ResumeTiming();

        for (const auto& x : shuffled) {
            vs.erase(x);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_VectorSet_Erase)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);


static void BM_StdSet_ConstructIteratorRange(benchmark::State& state) {
    std::vector<std::string> data = generateRandomData(state.range(0));

    for (auto _ : state) {
        benchmark::DoNotOptimize(std::set<std::string>(data.begin(), data.end()));
    }
}
BENCHMARK(BM_StdSet_ConstructIteratorRange)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);


static void BM_StdUnorderedSet_ConstructIteratorRange(benchmark::State& state) {
    std::vector<std::string> data = generateRandomData(state.range(0));

    for (auto _ : state) {
        benchmark::DoNotOptimize(std::unordered_set<std::string>(data.begin(), data.end()));
    }
}
BENCHMARK(BM_StdUnorderedSet_ConstructIteratorRange)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);


static void BM_VectorSet_ConstructIteratorRange(benchmark::State& state) {
    std::vector<std::string> data = generateRandomData(state.range(0));

    for (auto _ : state) {
        benchmark::DoNotOptimize(VectorSet<std::string>(data.begin(), data.end()));
    }
}

BENCHMARK(BM_VectorSet_ConstructIteratorRange)->RangeMultiplier(kRangeMult)->Range(kTestMinSize, kTestMaxSize);

