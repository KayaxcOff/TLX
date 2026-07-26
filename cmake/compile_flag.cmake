target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/arch:AVX2>
)

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CUDA>>:
        --expt-relaxed-constexpr
        >
)

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        /arch:AVX2
        /W4
        /Zi
        /permissive-
        >
)

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CUDA>>:
        --expt-relaxed-constexpr
        -G
        -lineinfo
        >
)

target_compile_options(TLX
        PUBLIC
        $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
)