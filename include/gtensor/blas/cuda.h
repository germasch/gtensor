#ifndef GTENSOR_BLAS_CUDA_H
#define GTENSOR_BLAS_CUDA_H

#include "cublas_v2.h"

// typedef cudaStream_t gtblas_stream_t;
// typedef cuDoubleComplex gtblas_complex_double_t;
// typedef cuComplex gtblas_complex_float_t;
typedef int gtblas_index_t;

namespace gt
{

namespace blas
{

struct _handle_wrapper
{
  cublasHandle_t handle;
};

// ======================================================================
// types aliases

// using handle_t = cublasHandle_t;
using stream_t = cudaStream_t;
using index_t = int;

// ======================================================================
// handle and stream management

inline void create(handle_t* h)
{
  *h = new _handle_wrapper();
  gtGpuCheck((cudaError_t)cublasCreate(&((*h)->handle)));
}

inline void destroy(handle_t h)
{
  gtGpuCheck((cudaError_t)cublasDestroy(h->handle));
  delete h;
}

inline void set_stream(handle_t h, stream_t stream_id)
{
  gtGpuCheck((cudaError_t)cublasSetStream(h->handle, stream_id));
}

inline void get_stream(handle_t h, stream_t* stream_id)
{
  gtGpuCheck((cudaError_t)cublasGetStream(h->handle, stream_id));
}

// ======================================================================
// axpy

template <typename T>
inline void axpy(handle_t h, int n, T a, const T* x, int incx, T* y, int incy);

#define CREATE_AXPY(METHOD, GTTYPE, BLASTYPE)                                  \
  template <>                                                                  \
  inline void axpy<GTTYPE>(handle_t h, int n, GTTYPE a, const GTTYPE* x,       \
                           int incx, GTTYPE* y, int incy)                      \
  {                                                                            \
    gtGpuCheck((cudaError_t)METHOD(h->handle, n,                               \
                                   reinterpret_cast<BLASTYPE*>(&a),            \
                                   reinterpret_cast<const BLASTYPE*>(x), incx, \
                                   reinterpret_cast<BLASTYPE*>(y), incy));     \
  }

CREATE_AXPY(cublasZaxpy, gt::complex<double>, cuDoubleComplex)
CREATE_AXPY(cublasCaxpy, gt::complex<float>, cuComplex)
CREATE_AXPY(cublasDaxpy, double, double)
CREATE_AXPY(cublasSaxpy, float, float)

#undef CREATE_AXPY

// ======================================================================
// scal

template <typename T>
inline void scal(handle_t h, int n, T fac, T* arr, const int incx);

#define CREATE_SCAL(METHOD, GTTYPE, BLASTYPE)                                  \
  template <>                                                                  \
  inline void scal<GTTYPE>(handle_t h, int n, GTTYPE fac, GTTYPE* arr,         \
                           const int incx)                                     \
  {                                                                            \
    gtGpuCheck((cudaError_t)METHOD(h->handle, n,                               \
                                   reinterpret_cast<BLASTYPE*>(&fac),          \
                                   reinterpret_cast<BLASTYPE*>(arr), incx));   \
  }

CREATE_SCAL(cublasZscal, gt::complex<double>, cuDoubleComplex)
CREATE_SCAL(cublasCscal, gt::complex<float>, cuComplex)
CREATE_SCAL(cublasDscal, double, double)
CREATE_SCAL(cublasSscal, float, float)

#undef CREATE_SCAL

// ======================================================================
// copy

template <typename T>
inline void copy(handle_t h, int n, const T* x, int incx, T* y, int incy);

#define CREATE_COPY(METHOD, GTTYPE, BLASTYPE)                                  \
  template <>                                                                  \
  inline void copy<GTTYPE>(handle_t h, int n, const GTTYPE* x, int incx,       \
                           GTTYPE* y, int incy)                                \
  {                                                                            \
    gtGpuCheck((cudaError_t)METHOD(h->handle, n,                               \
                                   reinterpret_cast<const BLASTYPE*>(x), incx, \
                                   reinterpret_cast<BLASTYPE*>(y), incy));     \
  }

CREATE_COPY(cublasZcopy, gt::complex<double>, cuDoubleComplex)
CREATE_COPY(cublasCcopy, gt::complex<float>, cuComplex)
CREATE_COPY(cublasDcopy, double, double)
CREATE_COPY(cublasScopy, float, float)

#undef CREATE_COPY

// ======================================================================
// gemv

template <typename T>
inline void gemv(handle_t h, int m, int n, T alpha, const T* A, int lda,
                 const T* x, int incx, T beta, T* y, int incy);

#define CREATE_GEMV(METHOD, GTTYPE, BLASTYPE)                                  \
  template <>                                                                  \
  inline void gemv<GTTYPE>(handle_t h, int m, int n, GTTYPE alpha,             \
                           const GTTYPE* A, int lda, const GTTYPE* x,          \
                           int incx, GTTYPE beta, GTTYPE* y, int incy)         \
  {                                                                            \
    gtGpuCheck((cudaError_t)METHOD(h->handle, CUBLAS_OP_N, m, n,               \
                                   reinterpret_cast<BLASTYPE*>(&alpha),        \
                                   reinterpret_cast<const BLASTYPE*>(A), lda,  \
                                   reinterpret_cast<const BLASTYPE*>(x), incx, \
                                   reinterpret_cast<BLASTYPE*>(&beta),         \
                                   reinterpret_cast<BLASTYPE*>(y), incy));     \
  }

CREATE_GEMV(cublasZgemv, gt::complex<double>, cuDoubleComplex)
CREATE_GEMV(cublasCgemv, gt::complex<float>, cuComplex)
CREATE_GEMV(cublasDgemv, double, double)
CREATE_GEMV(cublasSgemv, float, float)

#undef CREATE_GEMV

// ======================================================================
// getrf/getrs batched

template <typename T>
inline void getrf_batched(handle_t h, int n, T** d_Aarray, int lda,
                          gtblas_index_t* d_PivotArray, int* d_infoArray,
                          int batchSize);

#define CREATE_GETRF_BATCHED(METHOD, GTTYPE, BLASTYPE)                         \
  template <>                                                                  \
  inline void getrf_batched<GTTYPE>(handle_t h, int n, GTTYPE** d_Aarray,      \
                                    int lda, gtblas_index_t* d_PivotArray,     \
                                    int* d_infoArray, int batchSize)           \
  {                                                                            \
    gtGpuCheck((cudaError_t)METHOD(                                            \
      h->handle, n, reinterpret_cast<BLASTYPE**>(d_Aarray), lda, d_PivotArray, \
      d_infoArray, batchSize));                                                \
  }

CREATE_GETRF_BATCHED(cublasZgetrfBatched, gt::complex<double>, cuDoubleComplex)
CREATE_GETRF_BATCHED(cublasCgetrfBatched, gt::complex<float>, cuComplex)
CREATE_GETRF_BATCHED(cublasDgetrfBatched, double, double)
CREATE_GETRF_BATCHED(cublasSgetrfBatched, float, float)

#undef CREATE_GETRF_BATCHED

template <typename T>
inline void getrs_batched(handle_t h, int n, int nrhs, T* const* d_Aarray,
                          int lda, gtblas_index_t* devIpiv, T** d_Barray,
                          int ldb, int batchSize);

#define CREATE_GETRS_BATCHED(METHOD, GTTYPE, BLASTYPE)                         \
  template <>                                                                  \
  inline void getrs_batched<GTTYPE>(                                           \
    handle_t h, int n, int nrhs, GTTYPE* const* d_Aarray, int lda,             \
    gtblas_index_t* devIpiv, GTTYPE** d_Barray, int ldb, int batchSize)        \
  {                                                                            \
    int info;                                                                  \
    gtGpuCheck((cudaError_t)METHOD(                                            \
      h->handle, CUBLAS_OP_N, n, nrhs,                                         \
      reinterpret_cast<BLASTYPE* const*>(d_Aarray), lda, devIpiv,              \
      reinterpret_cast<BLASTYPE**>(d_Barray), ldb, &info, batchSize));         \
    if (info != 0) {                                                           \
      fprintf(stderr, "METHOD failed, info=%d at %s %d\n", info, __FILE__,     \
              __LINE__);                                                       \
      abort();                                                                 \
    }                                                                          \
  }

CREATE_GETRS_BATCHED(cublasZgetrsBatched, gt::complex<double>, cuDoubleComplex)
CREATE_GETRS_BATCHED(cublasCgetrsBatched, gt::complex<float>, cuComplex)
CREATE_GETRS_BATCHED(cublasDgetrsBatched, double, double)
CREATE_GETRS_BATCHED(cublasSgetrsBatched, float, float)

#undef CREATE_GETRS_BATCHED

} // namespace blas

} // namespace gt

#endif
