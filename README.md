# SequenceMapVectorSet

A high-performance C++20 header-only library providing specialized containers that combine the benefits of contiguous memory with associative lookups.

## Containers

### SequenceMap

`SequenceMap` is ideal when you need to maintain a specific sequence  but still require fast key-based access.

```cpp
#include "SequenceMap.hpp"
#include <string>
#include <iostream>

int main() {
    SequenceMap<std::string, int> map;
    map.push_back({"Alpha", 1});
    map.push_back({"Beta", 2});

    // Access by key
    std::cout << map.atKey("Alpha") << std::endl; // Prints 1

    // Access by index (preserves insertion order)
    std::cout << map[1].second << std::endl;      // Prints 2
}
```

### VectorSet

`VectorSet` is a set interface wrapped around a sorted vector.

```cpp
#include "VectorSet.hpp"
#include <iostream>

int main() {
    VectorSet<int> mySet = {5, 2, 8, 1}; // Automatically sorted: {1, 2, 5, 8}

    if (mySet.contains(5)) {
        std::cout << "Found 5!" << std::endl;
    }
}
```

## Testing & Benchmarking
The project uses GoogleTest for unit testing and Google Benchmark for performance evaluation.

## Dependencies
* **Language:** C++20 compatible compiler (e.g., GCC 10+, Clang 10+, or MSVC 19.29+).
* **Build System:** CMake 3.20 or higher.
* **Libraries:**
    * [GoogleTest](https://github.com/google/googletest) (for unit testing)
    * [Google Benchmark](https://github.com/google/benchmark) (for performance evaluation)

## Build and Run

```bash
# 1. Configure the project
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# 2. Build the targets
cmake --build build --config Release

# 3. Run Unit Tests
ctest --test-dir build -R "^(smvs_unit_tests)$" --output-on-failure

# 4. Run Performance Benchmarks
./build/perf_test/SequenceMapVectorSetPerf
```
