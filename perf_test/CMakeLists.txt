file(GLOB PERF_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(SequenceMapVectorSetPerf ${PERF_SOURCES})

target_link_libraries(SequenceMapVectorSetPerf
    PRIVATE
        SequenceMapVectorSet
        benchmark::benchmark
)

add_test(NAME smvs_perf_tests COMMAND SequenceMapVectorSetPerf)
