add_library(TLX STATIC
        source/buffer.cpp
        source/errors.cpp
        source/exception.cpp
        source/file_system.cpp
        source/memory.cpp
        source/status.cpp
        source/string.cpp
        source/types.cpp
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