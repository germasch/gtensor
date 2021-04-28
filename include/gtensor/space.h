
#ifndef GTENSOR_SPACE_H
#define GTENSOR_SPACE_H

#include "allocator.h"
#include "defs.h"
#include "gtensor_storage.h"
#include "helper.h"
#include "meta.h"
#include "span.h"

#include <vector>

#ifdef GTENSOR_USE_THRUST
#include <thrust/device_allocator.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#endif

#ifndef GTENSOR_DEFAULT_HOST_ALLOCATOR
#define GTENSOR_DEFAULT_HOST_ALLOCATOR(T) gt::backend::system::host_allocator<T>
#endif

#ifndef GTENSOR_DEFAULT_DEVICE_ALLOCATOR
#define GTENSOR_DEFAULT_DEVICE_ALLOCATOR(T)                                    \
  gt::allocator::caching_allocator<T, gt::backend::system::device_allocator<T>>
#endif

namespace gt
{

// ======================================================================
// space

namespace space
{

struct any;

struct kernel;

#ifdef GTENSOR_USE_THRUST

template <typename T, typename A = GTENSOR_DEFAULT_HOST_ALLOCATOR(T)>
using host_vector = thrust::host_vector<T, A>;

#ifdef GTENSOR_HAVE_DEVICE
template <typename T, typename A = GTENSOR_DEFAULT_DEVICE_ALLOCATOR(T)>
using device_vector = thrust::device_vector<T, A>;
#endif

#else

template <typename T, typename A = GTENSOR_DEFAULT_HOST_ALLOCATOR(T)>
using host_vector = gt::backend::host_storage<T, A>;

#ifdef GTENSOR_HAVE_DEVICE
template <typename T, typename A = GTENSOR_DEFAULT_DEVICE_ALLOCATOR(T)>
using device_vector = gt::backend::device_storage<T, A>;
#endif

#endif // GTENSOR_USE_THRUST

template <typename S>
struct space_traits
{};

template <>
struct space_traits<host>
{
  template <typename T>
  using storage_type = host_vector<T>;
  template <typename T>
  using span_type = span<T>;
};

#ifdef GTENSOR_HAVE_DEVICE

template <>
struct space_traits<device>
{
  template <typename T>
  using storage_type = device_vector<T>;
  template <typename T>
  using span_type = device_span<T>;
};

#endif

} // namespace space

// ======================================================================
// has_space_type

template <typename E, typename S, typename = void>
struct has_space_type : std::false_type
{};

template <typename T, typename S>
struct has_space_type<T, S,
                      gt::meta::void_t<std::enable_if_t<
                        std::is_same<expr_space_type<T>, S>::value>>>
  : std::true_type
{};

template <typename T, typename S>
constexpr bool has_space_type_v = has_space_type<T, S>::value;

// ======================================================================
// has_space_type_device

template <typename E, typename = void>
struct has_space_type_device : has_space_type<E, gt::space::device>
{};

template <typename T>
constexpr bool has_space_type_device_v = has_space_type_device<T>::value;

// ======================================================================
// has_space_type_host

template <typename E, typename = void>
struct has_space_type_host : has_space_type<E, gt::space::host>
{};

template <typename T>
constexpr bool has_space_type_host_v = has_space_type_host<T>::value;

} // namespace gt

#endif
