
#ifndef GT_SPACE_FORWARD_H
#define GT_SPACE_FORWARD_H

#include "defs.h"

namespace gt
{
namespace space
{

struct host_only
{};

#ifdef GTENSOR_USE_THRUST
struct thrust
{};
#endif

#ifdef GTENSOR_DEVICE_CUDA
struct cuda
{};
struct cuda_managed
{};
struct cuda_host
{};
#endif

#ifdef GTENSOR_DEVICE_HIP
struct hip
{};
struct sycl_managed
{};
struct sycl_host
{};
#endif

#ifdef GTENSOR_DEVICE_SYCL
struct sycl
{};
struct sycl_managed
{};
struct sycl_host
{};
#endif

#ifdef GTENSOR_HAVE_DEVICE

#if GTENSOR_USE_THRUST
using device = thrust;
#elif GTENSOR_DEVICE_CUDA
using device = cuda;
#elif GTENSOR_DEVICE_HIP
using device = hip;
#elif GTENSOR_DEVICE_SYCL
using device = sycl;
#endif
using host = host_only;

#else // !  GTENSOR_HAVE_DEVICE

using device = host;
using host = host_only;

#endif

} // namespace space
} // namespace gt

#endif
