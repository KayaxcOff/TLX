//
// Created by muham on 18.10.2026.
//

#ifndef TLX_DETAIL_FLAGS_HPP
#define TLX_DETAIL_FLAGS_HPP

#ifdef TLX_CUDA
    #include <cuda_runtime.h>
    #define TLX_HD __host__ __device__
    #define TLX_HOST __host__
    #define TLX_DEVICE __device__
#else //#ifdef TLX_CUDA
    #define TLX_HD
    #define TLX_DEVICE
    #define TLX_HOST
#endif //#ifdef TLX_CUDA #else

#define TLX_INLINE inline
#define TLX_STRONG_INLINE __forceinline

#define TLX_SAFETY_EXIT 0
#define TLX_ERROR_EXIT 1

#endif //TLX_DETAIL_FLAGS_HPP