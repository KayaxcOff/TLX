include(CheckLanguage)

check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    find_package(CUDAToolkit QUIET)

    if(CUDAToolkit_FOUND)
        set(TLX_CUDA TRUE)
        set(CUDA_LIBRARIES CUDA::cudart)
    endif()
endif()