//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Type erased proxy_reference to support type erased iterators.

#ifndef BLINK_ITERATOR_ANY_PROXY_REFERENCE_H_AHZ
#define BLINK_ITERATOR_ANY_PROXY_REFERENCE_H_AHZ

#include <blink/iterator/detail.h>
#include <memory>
namespace blink {
  namespace iterator {
    template<typename T>
    struct any_proxy_reference_interface
    {
      virtual ~any_proxy_reference_interface() {};
      //{};
      virtual operator T() = 0;
      virtual const any_proxy_reference_interface& operator=(const T&) = 0;
      virtual std::shared_ptr<any_proxy_reference_interface> clone() const = 0;
    };
    
    template<class Proxy, class T>
    struct any_proxy_reference_impl : public any_proxy_reference_interface < T >
    {
      ~any_proxy_reference_impl()
      {}
      
      any_proxy_reference_impl(const any_proxy_reference_impl& that) 
        : m_proxy(that.m_proxy)
      {}


      any_proxy_reference_impl(any_proxy_reference_impl&& that) 
        : m_proxy(std::move(that.m_proxy))
      {}

      any_proxy_reference_impl& operator=(const any_proxy_reference_impl& other)
      {
        m_proxy = other.m_proxy;
        return *this;
      }

      any_proxy_reference_impl& operator=(any_proxy_reference_impl&& other)
      {
        m_proxy = std::move(other.m_proxy);
        return *this;
      }

      template<class InProxy, class =
        disable_if_decay_to_same_t<InProxy, any_proxy_reference_impl> >
      any_proxy_reference_impl(InProxy&& p) 
        : m_proxy(std::forward<InProxy>(p)) //TODO avoid copying if possible
      {}
      
      any_proxy_reference_impl(Proxy p) : m_proxy(p) //TODO avoid copying if possible
      {}

      operator T()
      {
        return static_cast<T>(m_proxy);
      }

      const any_proxy_reference_impl& operator=(const T& v)
      {
        m_proxy = v;
        return *this;
      }

      std::shared_ptr<any_proxy_reference_interface> clone() const
      {
        return std::shared_ptr<any_proxy_reference_interface>(
          new any_proxy_reference_impl(m_proxy));
      };

    private:
      Proxy m_proxy;
    };

    template<typename T>
    struct any_proxy_reference
    {
      ~any_proxy_reference()
      {}

      any_proxy_reference(any_proxy_reference&& other)
        : m_impl(std::move(other.m_impl))
      {}

      any_proxy_reference(const any_proxy_reference& other)
        : m_impl(other.m_impl->clone())
      {}

      any_proxy_reference& operator=(const any_proxy_reference& other)
      {
        m_impl = other.m_impl->clone();
        return *this;
      }

      any_proxy_reference& operator=(any_proxy_reference&& other)
      {
        m_impl = std::move(other.m_impl);
        return *this;
      }

      template<class Proxy>
      any_proxy_reference(Proxy&& ref)
        : m_impl(new any_proxy_reference_impl< special_decay_t<Proxy>, T>
         (std::forward<Proxy>(ref)))
      {}

      // conversion to make the iterator readable
      operator T() const
      {
        return get();
      }

      // assignment to make the iterator writable
      const any_proxy_reference& operator=(const T& v)
      {
        put(v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator=(const any_proxy_reference<U>& that) 
      {
        return operator=(static_cast<U>(that));
      }

      //const any_proxy_reference& operator=(const any_proxy_reference& that) 
      //{
      //  return operator=(static_cast<T>(that));
      //}

      const any_proxy_reference& operator++() 
      {
        put(get() + 1);
        return *this;
      }
      const any_proxy_reference& operator--() 
      {
        put(get() - 1);
        return *this;
      }

      T operator++(int) 
      {
        T temp = get();
        put(get() + 1);
        return temp;
      }
      T operator--(int) 
      {
        T temp = get();
        put(get() - 1);
        return temp;
      }

      template<class U>
      const any_proxy_reference& operator+=(const U& v) 
      {
        put(get() + v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator-=(const U& v) 
      {
        put(get() - v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator/=(const U& v) 
      {
        put(get() / v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator*=(const U& v) 
      {
        put(get() * v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator%=(const U& v) 
      {
        put(get() % v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator&=(const U& v) 
      {
        put(get() & v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator|=(const U& v) 
      {
        put(get() | v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator^=(const U& v) 
      {
        put(get() ^ v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator<<=(const U& v) 
      {
        put(get() << v);
        return *this;
      }

      template<class U>
      const any_proxy_reference& operator>>=(const U& v) 
      {
        put(get() >> v);
        return *this;
      }

      T get() const
      {
        //return m_impl->operator T();
        return static_cast<T>(*m_impl);
      }

      void put(const T& v)
      {
        //m_impl->operator=(v);
        *m_impl = v;
      }
      std::shared_ptr<any_proxy_reference_interface<T> > m_impl;
    };
  }
}
#endif