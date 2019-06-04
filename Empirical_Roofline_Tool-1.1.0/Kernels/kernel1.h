#ifndef KERNEL1_H
#define KERNEL1_H

#include "rep.h"

#ifdef ERT_GPU
extern int gpu_blocks;
extern int gpu_threads;
#endif

#define KERNEL1(a,b,c)   ((a) = (b) + (c))
#define KERNEL2(a,b,c)   ((a) = (a)*(b) + (c))

template <typename T>
void initialize(uint64_t nsize,
                T* __restrict__ A,
                T value)
{
#ifdef ERT_INTEL
  __assume_aligned(A, ERT_ALIGN);
#elif __xlC__
  __alignx(ERT_ALIGN, A);
#endif

  uint64_t i;
  for (i = 0; i < nsize; ++i) {
    A[i] = value;
  }
}

#ifdef ERT_GPU
template <typename T>
__global__ void block_stride(uint64_t ntrials, uint64_t nsize, T *A)
{
  uint64_t total_thr = gridDim.x * blockDim.x;
  uint64_t elem_per_thr = (nsize + (total_thr-1)) / total_thr;
  uint64_t blockOffset = blockIdx.x * blockDim.x; 

  uint64_t start_idx  = blockOffset + threadIdx.x;
  uint64_t end_idx    = start_idx + elem_per_thr * total_thr;
  uint64_t stride_idx = total_thr;

  if (start_idx > nsize) {
    start_idx = nsize;
  }

  if (end_idx > nsize) {
    end_idx = nsize;
  }

  T alpha = 0.5;
  uint64_t i, j;
  for (j = 0; j < ntrials; ++j) {
    for (i = start_idx; i < end_idx; i += stride_idx) {
      T beta = 0.8;
#if (ERT_FLOP & 1) == 1       /* add 1 flop */
      KERNEL1(beta,A[i],alpha);
#endif
#if (ERT_FLOP & 2) == 2       /* add 2 flops */
      KERNEL2(beta,A[i],alpha);
#endif
#if (ERT_FLOP & 4) == 4       /* add 4 flops */
      REP2(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 8) == 8       /* add 8 flops */
      REP4(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 16) == 16     /* add 16 flops */
      REP8(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 32) == 32     /* add 32 flops */
      REP16(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 64) == 64     /* add 64 flops */
      REP32(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 128) == 128   /* add 128 flops */
      REP64(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 256) == 256   /* add 256 flops */
      REP128(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 512) == 512   /* add 512 flops */
      REP256(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 1024) == 1024 /* add 1024 flops */
      REP512(KERNEL2(beta,A[i],alpha));
#endif

      A[i] = beta;
    }
    alpha = alpha * (1 - 1e-8);
  }
}

template <typename T>
void gpuKernel(uint64_t nsize,
               uint64_t ntrials,
               T* __restrict__ A,
               int* bytes_per_elem,
               int* mem_accesses_per_elem)
{
  *bytes_per_elem        = sizeof(*A);
  *mem_accesses_per_elem = 2;

#ifdef ERT_INTEL
  __assume_aligned(A, ERT_ALIGN);
#elif __xlC__
  __alignx(ERT_ALIGN, A);
#endif

  block_stride<T><<< gpu_blocks, gpu_threads>>> (ntrials, nsize, A);
}
#else
template <typename T>
void kernel(uint64_t nsize,
            uint64_t ntrials,
            T* __restrict__ A,
            int* bytes_per_elem,
            int* mem_accesses_per_elem)
{
  *bytes_per_elem        = sizeof(*A);
  *mem_accesses_per_elem = 2;

#ifdef ERT_INTEL
  __assume_aligned(A, ERT_ALIGN);
#elif __xlC__
  __alignx(ERT_ALIGN, A);
#endif

  T alpha = 0.5;
  uint64_t i, j;
  for (j = 0; j < ntrials; ++j) {
#pragma unroll (8)
    for (i = 0; i < nsize; ++i) {
      T beta = 0.8;
#if (ERT_FLOP & 1) == 1       /* add 1 flop */
      KERNEL1(beta,A[i],alpha);
#endif
#if (ERT_FLOP & 2) == 2       /* add 2 flops */
      KERNEL2(beta,A[i],alpha);
#endif
#if (ERT_FLOP & 4) == 4       /* add 4 flops */
      REP2(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 8) == 8       /* add 8 flops */
      REP4(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 16) == 16     /* add 16 flops */
      REP8(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 32) == 32     /* add 32 flops */
      REP16(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 64) == 64     /* add 64 flops */
      REP32(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 128) == 128   /* add 128 flops */
      REP64(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 256) == 256   /* add 256 flops */
      REP128(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 512) == 512   /* add 512 flops */
      REP256(KERNEL2(beta,A[i],alpha));
#endif
#if (ERT_FLOP & 1024) == 1024 /* add 1024 flops */
      REP512(KERNEL2(beta,A[i],alpha));
#endif

      A[i] = beta;
    }
    alpha = alpha * (1 - 1e-8);
  }
}
#endif

#endif
