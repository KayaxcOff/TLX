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

add_library(TLX::TLX ALIAS TLX)

target_include_directories(TLX
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (TLX_CUDA)
    target_link_libraries(TLX PUBLIC
        ${CUDA_LIBRARIES}
    )

    target_compile_definitions(TLX PUBLIC TLX_CUDA)
endif ()