cmake_minimum_required(VERSION 3.20)
project(SequenceMapVectorSet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# --- Dependencies ---
include(FetchContent)

# ---- GoogleTest (FetchContent) ----
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(googletest)

# ---- Google Benchmark (FetchContent) ----
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(benchmark)

# --- SubDirectories ---
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(perf_test)