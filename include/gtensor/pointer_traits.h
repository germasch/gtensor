
#ifndef POINTER_TRAITS_H
#define POINTER_TRAITS_H

#include "device_ptr.h"
#include "meta.h"
#include "space_forward.h"

#ifdef GTENSOR_USE_THRUST
#include <thrust/device_ptr.h>
#endif

namespace gt
{

namespace detail
{

// -------------------------------------------------------------------------
// pointer_traits_element_type

template <typename P, typename Enable = void>
struct pointer_traits_element_type;

template <typename P>
struct pointer_traits_element_type<P,
                                   gt::meta::void_t<typename P::element_type>>
{
  using type = typename P::element_type;
};

template <typename T>
struct pointer_traits_element_type<T*>
{
  using type = T;
};

#ifdef GTENSOR_USE_THRUST
template <typename T>
struct pointer_traits_element_type<::thrust::device_ptr<T>>
{
  using type = T;
};
#endif

// -------------------------------------------------------------------------
// pointer_traits_reference

template <typename P, typename Enable = void>
struct pointer_traits_reference
{
  using type = typename pointer_traits_element_type<P>::type&;
};

template <typename P>
struct pointer_traits_reference<P, gt::meta::void_t<typename P::reference>>
{
  using type = typename P::reference;
};

// -------------------------------------------------------------------------
// pointer_traits_const_reference

template <typename P, typename Enable = void>
struct pointer_traits_const_reference
{
  using type = const typename pointer_traits_element_type<P>::type&;
};

template <typename P>
struct pointer_traits_const_reference<
  P, gt::meta::void_t<typename P::const_reference>>
{
  using type = typename P::const_reference;
};

// -------------------------------------------------------------------------
// pointer_traits_space_type

template <typename P, typename Enable = void>
struct pointer_traits_space_type
{};

template <typename P>
struct pointer_traits_space_type<P, gt::meta::void_t<typename P::space_type>>
{
  using type = typename P::space_type;
};

template <typename T>
struct pointer_traits_space_type<T*, void>
{
  using type = gt::space::host;
};

#ifdef GTENSOR_USE_THRUST
template <typename T>
struct pointer_traits_space_type<::thrust::device_ptr<T>, void>
{
  using type = gt::space::thrust;
};
#endif

// -------------------------------------------------------------------------
// pointer_traits_rebind

template <typename P, typename U, typename Enable = void>
struct pointer_traits_rebind
{};

template <typename P, typename U>
struct pointer_traits_rebind<P, U, gt::meta::void_t<typename P::rebind<U>>>
{
  using type = typename P::rebind<U>;
};

template <typename T, typename U>
struct pointer_traits_rebind<T*, U, void>
{
  using type = U*;
};

#ifdef GTENSOR_USE_THRUST
template <typename T, typename U>
struct pointer_traits_rebind<::thrust::device_ptr<T>, U, void>
{
  using type = ::thrust::device_ptr<U>;
};
#endif

// -------------------------------------------------------------------------
// pointer_traits_get

template <typename P>
GT_INLINE auto pointer_traits_get(P p)
{
  return p.get();
};

template <typename T>
GT_INLINE auto pointer_traits_get(T* p)
{
  return p;
};

} // namespace detail

template <typename P>
struct pointer_traits
{
  using pointer = P;

  template <typename U>
  using rebind = typename detail::pointer_traits_rebind<pointer, U>::type;

  using element_type =
    typename detail::pointer_traits_element_type<pointer>::type;
  using const_pointer = rebind<const element_type>;
  using reference = typename detail::pointer_traits_reference<pointer>::type;
  using const_reference =
    typename detail::pointer_traits_const_reference<pointer>::type;
  using space_type = typename detail::pointer_traits_space_type<pointer>::type;

  GT_INLINE static auto get(pointer p) { return detail::pointer_traits_get(p); }
};

} // namespace gt

#endif