//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// MACRO to provide operator overloads for range_algebra_wrappers
//
// Overload raster_algebra operator "op" and ties it to function "func". 

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_OPERATOR_MACRO_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_OPERATOR_MACRO_H_AHZ

#include <blink/iterator/range_algebra_transform.h>
#include <blink/iterator/range_algebra_wrapper.h>
#include <blink/iterator/transform_range.h>
#include <functional>


#define BLINK_ITERATOR_RA_BINARY_OP(op, func)                                      \
  namespace blink {                                                                \
    namespace iterator {                                                           \
                                                                                   \
      template<class R, class T>                                                   \
      auto operator op(range_algebra_wrapper<R>&& r, T&& v)->decltype              \
        (range_algebra_function(func{}, std::move(r), std:::forward<T>(v)))        \
      {                                                                            \
        return range_algebra_function(func{}, std::move(r), std:::forward<T>(v));  \
      }                                                                            \
                                                                                   \
      template<class R, class T>                                                   \
      auto operator op(range_algebra_wrapper<R>& r, T&& v)->decltype               \
        (range_algebra_function(func{}, std::ref(r), std::forward<T>(v)))          \
      {                                                                            \
        return range_algebra_function(func{}, std::ref(r), std::forward<T>(v));    \
      }                                                                            \
                                                                                   \
      template<class R, class T>                                                   \
      auto operator op(T&& v, range_algebra_wrapper<R>&& r)->decltype              \
        (range_algebra_function(func{}, std::forward<T>(v), std::move(r)))         \
      {                                                                            \
        return range_algebra_function(func{}, std::forward<T>(v), std::move(r));   \
      }                                                                            \
                                                                                   \
      template<class R, class T>                                                   \
      auto operator op(T&& v, range_algebra_wrapper<R>& r)->decltype               \
        (range_algebra_function(func{}, std::forward<T>(v), std::ref(r)))          \
      {                                                                            \
        return range_algebra_function(func{}, std::forward<T>(v), std::ref(r));    \
      }                                                                            \
                                                                                   \
      template<class R1, class R2>                                                 \
      auto operator op(range_algebra_wrapper<R1>&& r1,                             \
        range_algebra_wrapper<R2>&& r2)->decltype                                  \
        (range_algebra_function(func{}, std::move(r1), std::move(r2)))             \
      {                                                                            \
        return range_algebra_function(func{}, std::move(r1), std::move(r2));       \
      }                                                                            \
                                                                                   \
      template<class R1, class R2>                                                 \
      auto operator op(range_algebra_wrapper<R1>&& r1,                             \
        range_algebra_wrapper<R2>& r2)->decltype                                   \
        (range_algebra_function(func{}, std::move(r1), std::ref(r2)))              \
      {                                                                            \
        return range_algebra_function(func{}, std::move(r1), std::ref(r2));        \
      }                                                                            \
                                                                                   \
      template<class R1, class R2>                                                 \
      auto operator op(range_algebra_wrapper<R1>& r1,                              \
        range_algebra_wrapper<R2>&& r2)->decltype                                  \
        (range_algebra_function(func{}, std::ref(r1), std::move(r2)))              \
      {                                                                            \
        return range_algebra_function(func{}, std::ref(r1), std::move(r2));        \
      }                                                                            \
                                                                                   \
      template<class R1, class R2>                                                 \
      auto operator op(range_algebra_wrapper<R1>& r1,                              \
        range_algebra_wrapper<R2>& r2)->decltype                                   \
        (range_algebra_function(func{}, std::ref(r1), std::ref(r2)))               \
      {                                                                            \
        return range_algebra_function(func{}, std::ref(r1), std::ref(r2));         \
      }                                                                            \
    }                                                                              \
  } // end of BLINK_ITERATOR_RA_BINARY_OP(op, func)
 
#define BLINK_ITERATOR_RA_UNARY_OP(op, func)                                       \
  namespace blink {                                                                \
    namespace iterator {                                                           \
                                                                                   \
      template<class R>                                                            \
      auto operator op(range_algebra_wrapper<R>& r)->decltype                      \
        (range_algebra_function(func{}, std::ref(r)))                              \
      {                                                                            \
        return range_algebra_function(func{}, std::ref(r));                        \
      }                                                                            \
                                                                                   \
      template<class R>                                                            \
      auto operator op(range_algebra_wrapper<R>&& r)->decltype                     \
        (range_algebra_function(func{}, std::move(r)))                             \
      {                                                                            \
        return range_algebra_function(func{}, std::move(r));                       \
      }                                                                            \
  }                                                                                \
} // end of BLINK_ITERATOR_RA_UNARY_OP(op, func)

BLINK_ITERATOR_RA_BINARY_OP(+, std::plus<>)
BLINK_ITERATOR_RA_BINARY_OP(-, std::minus<>)
BLINK_ITERATOR_RA_BINARY_OP(/ , std::divides<>)
BLINK_ITERATOR_RA_BINARY_OP(%, std::modulus<>)
BLINK_ITERATOR_RA_BINARY_OP(*, std::multiplies<>)
BLINK_ITERATOR_RA_BINARY_OP(&&, std::logical_and<>)
BLINK_ITERATOR_RA_BINARY_OP(|| , std::logical_or<>)
BLINK_ITERATOR_RA_BINARY_OP(> , std::greater<>)
BLINK_ITERATOR_RA_BINARY_OP(>= , std::greater_equal<>)
BLINK_ITERATOR_RA_BINARY_OP(< , std::less<>)
BLINK_ITERATOR_RA_BINARY_OP(<= , std::less_equal<>)
BLINK_ITERATOR_RA_BINARY_OP(== , std::equal_to<>)
BLINK_ITERATOR_RA_BINARY_OP(!= , std::not_equal_to<>)

BLINK_ITERATOR_RA_UNARY_OP(-, std::negate<>)
BLINK_ITERATOR_RA_UNARY_OP(! , std::logical_not<>)

#endif