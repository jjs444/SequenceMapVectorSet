# Header-only INTERFACE library
add_library(SequenceMapVectorSet INTERFACE)
target_include_directories(SequenceMapVectorSet INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/detail
    ${boost_pool_SOURCE_DIR}/include
    ${boost_assert_SOURCE_DIR}/include
    ${boost_config_SOURCE_DIR}/include
    ${boost_integer_SOURCE_DIR}/include
    ${boost_throw_exception_SOURCE_DIR}/include
    ${boost_type_traits_SOURCE_DIR}/include
    ${boost_winapi_SOURCE_DIR}/include
)