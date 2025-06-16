file(GLOB TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(SequenceMapVectorSetTests ${TEST_SOURCES})

target_link_libraries(SequenceMapVectorSetTests
    PRIVATE
        SequenceMapVectorSet
        GTest::gtest
        GTest::gtest_main
)

add_test(NAME smvs_unit_tests COMMAND SequenceMapVectorSetTests)