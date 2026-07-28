add_library(TLX STATIC
        source/byte.cpp
        source/exception.cpp
        source/memory.cpp
        source/status.cpp
        source/types.cpp
        ../source/string.cpp
)

target_include_directories(TLX PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

if (TLX_CUDA)
    target_link_libraries(TLX PRIVATE
        ${CUDA_LIBRARIES}
    )

    target_compile_definitions(TLX PUBLIC TLX_CUDA)
endif ()