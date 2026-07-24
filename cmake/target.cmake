add_library(TLX STATIC
        source/exception.cpp
)

target_include_directories(TLX PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

if (TLX_CUDA)
    target_link_libraries(TLX PRIVATE
        ${CUDA_LIBRARIES}
    )

    target_compile_definitions(TLX PRIVATE TLX_CUDA)
endif ()