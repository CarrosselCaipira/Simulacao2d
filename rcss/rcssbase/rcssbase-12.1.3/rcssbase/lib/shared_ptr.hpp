// -*-c++-*-

/***************************************************************************
                            shared_ptr.hpp
                           -------------------

    shared_ptr is a smart referenced counted pointer that also keeps
    a pointer refernce to a lib object to prevent the lib from closing
    prematurely.


    begin                : 2002-10-08
    copyright            : (C) 2002 by The RoboCup Soccer Simulator
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU LGPL as published by the Free Software  *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#ifndef RCSS_LIB_SHARED_PTR_H
#define RCSS_LIB_SHARED_PTR_H

#include "loader.hpp"

namespace rcss {
namespace lib {

template< typename T > class weak_ptr;

template< class T >
class shared_ptr {
private:
    typedef shared_ptr< T > this_type;
public:
    typedef T element_type;

    shared_ptr()
      {}

    ~shared_ptr()
      {}

    template<typename Y>
    explicit
    shared_ptr( Y* p )
        : m_item( p )
      {}

    template<typename Y>
    explicit
    shared_ptr( Y* p, const Loader::Impl& lib )
        : m_lib( lib ),
          m_item( p )
      {}

    template<typename Y, typename D>
    shared_ptr( Y *p, D d, const Loader::Impl& lib )
        : m_lib( lib ),
          m_item( p, d )
      {}

    template<typename Y>
    explicit
    shared_ptr( weak_ptr< Y > const & r )
        : m_lib( r.m_lib ),
          m_item( r.m_item )
      {}

    template<typename Y>
    shared_ptr( shared_ptr<Y> const & r )
        : m_lib( r.m_lib ),
          m_item( r.m_item )
      {}

#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)

    template<typename Y>
    shared_ptr&
    operator=( shared_ptr<Y> const & r )
      {
          m_item = r.m_item;
          m_lib = r.m_lib;
          return *this;
	    }
#endif

    void
    reset()
      { this_type().swap(*this); }

    template<typename Y>
    void
    reset( Y * p, const Loader::Impl& lib )
      { this_type( p, lib ).swap( *this ); }

    template<typename Y, typename D>
    void
    reset( Y * p, D d, const Loader::Impl& lib )
      { this_type( p, d, lib ).swap( *this ); }

    typename boost::detail::shared_ptr_traits<T>::reference
    operator*() const
      { return *m_item; }

    boost::shared_ptr< T >
    operator->() const
      { return m_item; }

    T* get() const
      { return m_item.get(); }

    // implicit conversion to "bool"

    //         typedef T * (this_type::*unspecified_bool_type)() const;

    //         operator unspecified_bool_type() const
    //         { return m_item.get() ? 1 : 0; }


    operator bool() const
      { return m_item.get() ? true : false; }

    bool
    operator!() const
      { return !((bool)*this); }

    bool
    unique() const
      { return m_item.unique(); }

    long
    use_count() const // never throws
      { return m_item.use_count(); }

    void
    swap( shared_ptr<T>& other )
      { std::swap( other, *this ); }

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<typename Y> friend class shared_ptr;
    template<typename Y> friend class weak_ptr;


#endif
    Loader::Impl m_lib;
    boost::shared_ptr< T > m_item;

};

template<typename T, typename U>
inline
bool
operator==( shared_ptr<T> const& a, shared_ptr<U> const& b )
{ return a.get() == b.get(); }

template<typename T, typename U>
inline
bool
operator!=( shared_ptr<T> const& a, shared_ptr<U> const& b )
{ return a.get() != b.get(); }

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<typename T>
inline
bool
operator!=( shared_ptr<T> const& a, shared_ptr<T> const& b )
{ return a.get() != b.get(); }

#endif

template<typename T>
inline
bool
operator<( shared_ptr<T> const& a, shared_ptr<T> const& b )
{ return std::less<T*>()( a.get(), b.get() ); }

template<typename T>
inline
void
swap( shared_ptr<T> & a, shared_ptr<T> & b )
{ a.swap( b ); }

}
}

#endif
