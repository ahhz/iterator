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

#include <blink/iterator/detail.h>
#include <blink/iterator/any_input_iterator.h>

namespace blink {
  namespace iterator {
    template<typename T>
    struct any_input_range_interface
    {
      virtual ~any_input_range_interface() {};
      using iterator = any_input_iterator < T > ;
      virtual iterator begin() = 0;
      virtual iterator end() = 0;
      virtual std::shared_ptr<any_input_range_interface> clone() const = 0;
    };

    template<class Range, class T>
    struct any_input_range_impl : public any_input_range_interface < T >
    {
      any_input_range_impl(const any_input_range_impl& that) : m_range(that.m_range)
      {}

      template<class InRange>
      any_input_range_impl(InRange&& r) : m_range(std::forward<InRange>(r))
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

      std::shared_ptr<any_input_range_interface> clone() const
      {
        return std::shared_ptr<any_input_range_interface>(
          new any_input_range_impl(m_range));
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
      {}
      
      any_input_range(const any_input_range& other) 
        : m_impl(other.m_impl->clone())
      {}

      any_input_range(any_input_range&& other) : m_impl(std::move(other.m_impl))
      {}

      template<typename Range, typename = 
        disable_if_decay_to_same_t<Range, any_input_range> >
      any_input_range(Range&& r) : m_impl(new any_input_range_impl<
        special_decay_t<Range>, T>(std::forward<Range>(r)) )
      {}
      
      any_input_range& operator=(const any_input_range& other)
      {
        m_impl = other.m_impl->clone();
        return *this;
      }

      any_input_range& operator=(any_input_range&& other)
      {
        m_impl = std::move(other.m_impl);
        return *this;
      }

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

    template<class Range>
    any_input_range<typename Range::iterator::value_type>
      make_any_input_range(Range&& r)
    {
      using value_type = typename Range::iterator::value_type;
      return any_input_range<value_type>(std::forward<Range>(r));
    }

  }
}

#endif
