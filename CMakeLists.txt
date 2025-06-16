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

# ---- Boost (Header-only) ----
# ick wish I could just fetch stuff needed for boost_pool easily but its a pain
FetchContent_Declare(boost_assert
  GIT_REPOSITORY https://github.com/boostorg/assert.git
  GIT_TAG boost-1.84.0
)

FetchContent_Declare(boost_config
  GIT_REPOSITORY https://github.com/boostorg/config.git
  GIT_TAG boost-1.84.0
)

FetchContent_Declare(boost_integer
  GIT_REPOSITORY https://github.com/boostorg/integer.git
  GIT_TAG boost-1.84.0
)

FetchContent_Declare(boost_throw_exception
  GIT_REPOSITORY https://github.com/boostorg/throw_exception.git
  GIT_TAG boost-1.84.0
)

FetchContent_Declare(boost_type_traits
  GIT_REPOSITORY https://github.com/boostorg/type_traits.git
  GIT_TAG boost-1.84.0
)

FetchContent_Declare(boost_winapi
  GIT_REPOSITORY https://github.com/boostorg/winapi.git
  GIT_TAG boost-1.84.0
)

FetchContent_Declare(boost_pool
  GIT_REPOSITORY https://github.com/boostorg/pool.git
  GIT_TAG boost-1.84.0
)

FetchContent_MakeAvailable(
  boost_assert
  boost_config
  boost_integer
  boost_throw_exception
  boost_type_traits
  boost_winapi
  boost_pool
)

# --- SubDirectories ---
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(perf_test)