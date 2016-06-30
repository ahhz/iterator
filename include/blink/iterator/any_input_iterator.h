//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Type erased input iterator to support type erased input range.


#ifndef BLINK_ITERATOR_ANY_INPUT_ITERATOR_H_AHZ
#define BLINK_ITERATOR_ANY_INPUT_ITERATOR_H_AHZ

#include <blink/iterator/any_proxy_reference.h>
#include <memory>

namespace blink {
  namespace iterator {
    template<class T> class any_input_iterator;

    template<typename T>
    struct any_input_iterator_interface
    {
      using reference = any_proxy_reference < T > ;
      
      virtual ~any_input_iterator_interface()
      {};
      virtual any_input_iterator_interface& operator++() = 0;
      virtual any_input_iterator<T> operator++(int) = 0;
      virtual reference operator*() = 0;

      virtual bool operator==(const any_input_iterator_interface&) const= 0;
      virtual bool operator!=(const any_input_iterator_interface&) const = 0;
      virtual std::shared_ptr<any_input_iterator_interface> clone() const = 0;
    };

    template<class Iter, class T>
    struct any_input_iterator_impl : public any_input_iterator_interface < T >
    {

      ~any_input_iterator_impl()
      {}

      any_input_iterator_impl(const any_input_iterator_impl& that) : m_iter(that.m_iter)
      {}

      any_input_iterator_impl(any_input_iterator_impl&& that) : m_iter(std::move(that.m_iter))
      {}

      any_input_iterator_impl& operator=(const any_input_iterator_impl& other)
      {
        m_iter = other.m_iter;
        return *this;
      }

      any_input_iterator_impl& operator=(any_input_iterator_impl&& other)
      {
        m_iter = std::move(other.m_iter);
        return *this;
      }
      
      template<class InIter, class =
        disable_if_decay_to_same_t<InIter, any_input_iterator_impl> >
      any_input_iterator_impl(InIter&& i) : m_iter(std::forward<InIter>(i)) //TODO avoid copying if possible
      {}

      using reference = any_proxy_reference < T > ;

      any_input_iterator_interface& operator++()
      {
        ++m_iter;
        return *this;
      }
      any_input_iterator < T > operator++(int)
      {
        return any_input_iterator<T>(m_iter++);
      }

      reference operator*()
      {
        T value = *m_iter;
        return reference(value);
      }

      bool operator==(const any_input_iterator_interface& other) const
      {
        const any_input_iterator_impl* b = dynamic_cast<const any_input_iterator_impl*>(&other);
        return b != nullptr && m_iter == b->m_iter;
      };

      bool operator!=(const any_input_iterator_interface& other) const
      {
        const any_input_iterator_impl* b = dynamic_cast<const any_input_iterator_impl*>(&other);
        return b == nullptr || m_iter != b->m_iter;
      };

      std::shared_ptr<any_input_iterator_interface> clone() const
      {
        return std::shared_ptr<any_input_iterator_interface>(
          new any_input_iterator_impl(m_iter));
      };

    private:
      Iter m_iter;
    };


    template<class T>
    class any_input_iterator
      : public std::iterator < std::input_iterator_tag, T, any_proxy_reference<T> >
    {
    public:
      using reference = any_proxy_reference < T > ;
      ~any_input_iterator()
      {}

      any_input_iterator()
      {};

      any_input_iterator(const any_input_iterator& other)
        : m_impl(other.m_impl->clone())
      {
      }

      any_input_iterator(any_input_iterator&& other)
        : m_impl(std::move(other.m_impl))
      {
      }

      template<class Iter, class =
        disable_if_decay_to_same_t<Iter, any_input_iterator> > 
      any_input_iterator(Iter&& i) 
        : m_impl(new any_input_iterator_impl<special_decay_t<Iter>, T>(std::forward<Iter>(i)))
      {}

      
      any_input_iterator& operator=(
        const any_input_iterator& other)
      {
        m_impl = other.m_impl->clone();
        return *this;
      }
     
      any_input_iterator& operator=(any_input_iterator&& other)
      {
        m_impl = std::move(other.m_impl);
        return *this;
      }
          
      any_input_iterator& operator++()
      {
        ++(*m_impl);
        return *this;
      }

      any_input_iterator operator++(int)
      {
        return(*m_impl)++;
      }

      reference operator*() const
      {
        return **m_impl;
      }

      bool operator==(const any_input_iterator& other) const
      {
        return *m_impl == *other.m_impl;
      }

      bool operator!=(const any_input_iterator& other) const
      {
        return *m_impl != *other.m_impl;
      }
      std::shared_ptr<any_input_iterator_interface<T> > m_impl;
    };
  }
}
#endif
