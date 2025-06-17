# Header-only INTERFACE library
add_library(SequenceMapVectorSet INTERFACE)
target_include_directories(SequenceMapVectorSet INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/detail
)