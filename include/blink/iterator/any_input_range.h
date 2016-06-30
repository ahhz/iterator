//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Type erased input range


#ifndef BLINK_ITERATOR_ANY_INPUT_RANGE_H_AHZ
#define BLINK_ITERATOR_ANY_INPUT_RANGE_H_AHZ

#include <blink/iterator/any_input_iterator.h>
namespace blink {
  namespace iterator {
    template<typename T>
    struct any_input_range_interface
    {
      using iterator = any_input_iterator < T > ;
      virtual iterator begin() = 0;
      virtual iterator end() = 0;
    };

    template<class Range, class T>
    struct any_input_range_impl : public any_input_range_interface < T >
    {
      any_input_range_impl(const any_input_range_impl& that) : m_range(that.m_range)
      {}

      any_input_range_impl(const Range& r) : m_range(r) //TODO avoid copying if possible
      {}

      using iterator = any_input_iterator < T > ;
      iterator begin()
      {
        return iterator(m_range.begin());
      }

      iterator end()
      {
        return iterator(m_range.end());
      }

    private:
      Range m_range;
    };


    template<class T>
    class any_input_range
      : public std::iterator < std::input_iterator_tag, T, any_proxy_reference<T> >
    {

    public:
      using iterator = any_input_iterator < T > ;

      any_input_range()
      {};

      any_input_range(const any_input_range& other) = default;

      template<class Range>
      any_input_range(Range&& r) 
        : m_impl(new any_input_range_impl<special_decay_t<Range>, T>(std::forward<Range>(r)) )
      {}
      //      any_input_range(any_input_range&& other) = default;
      any_input_range& operator=(const any_input_range& other) = default;
      //      any_input_range& operator=( any_input_range&& other) = default;
      ~any_input_range() = default;
      iterator begin()
      {
        return m_impl->begin();
      }

      iterator end()
      {
        return m_impl->end();
      }

    private:
      std::shared_ptr<any_input_range_interface<T> > m_impl;
    };
  }
}

#endif
