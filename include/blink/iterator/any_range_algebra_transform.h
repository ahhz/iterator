
//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Applies the range_algebra_transform on the input ranges and values and 
// returns an any_input_range type. This is less efficient than the normal
// range_algebra_function, but as the return type is independent of the 
// input ranges, it is better suited for runtime polymorphy


#ifndef BLINK_ITERATOR_ANY_RANGE_ALGEBRA_TRANSFORM_H_AHZ
#define BLINK_ITERATOR_ANY_RANGE_ALGEBRA_TRANSFORM_H_AHZ

#include <blink/iterator/any_input_range.h>
#include <blink/iterator/range_algebra_transform.h>


namespace blink {
  namespace iterator {

    template<class T>
    using any_range_algebra_wrapper = range_algebra_wrapper < any_input_range<T> > ;

    template<class Function, class... Args>
    using range_algebra_transform_value_type = typename range_algebra_transform<
      special_decay_t<Function>, special_decay_t<Args>...>::value_type;

    template<class Function, class...Args>
    any_input_range<range_algebra_transform_value_type<Function, Args...> >
      any_range_algebra_transform(Function&& f, Args&&... a)
    {

      return
      any_input_range<range_algebra_transform_value_type<Function, Args...> >(
          make_range_algebra_transform(std::forward<Function>(f)
          , std::forward<Args>(a)...) ) ;
    }

    template<class Function, class...Args>
    range_algebra_wrapper<any_input_range<range_algebra_transform_value_type<Function, Args...> > >
      any_range_algebra_function(Function&& f, Args&&... a)
    {
      return range_algebra(
        any_range_algebra_transform(std::forward<Function>(f)
        , std::forward<Args>(a)...));
    }
  }
}
#endif
