cmake_minimum_required(VERSION 4.1)

project(
        TLX
        VERSION 1.0.0
        LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH
        "${CMAKE_SOURCE_DIR}"
)

include(cmake/find_cuda)
include(cmake/standard)
include(cmake/target)
include(cmake/compile_flag)