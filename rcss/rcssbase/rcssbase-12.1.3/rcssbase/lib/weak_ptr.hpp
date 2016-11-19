// -*-c++-*-

/***************************************************************************
                            weak_ptr.h
                           -------------------

    weak_ptr is a reference to an item pointed to by a shared pointer
    that doesn't prevent the item from being dertoyed.  Use
    make_shared to convert it to a shared_ptr.  This can throw a
    boost::use_count_zero if the item has been destroyed.

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

#include "lib_shared_ptr.hpp"

namespace rcss {
namespace lib {

template< typename T >
class weak_ptr {
private:

    // Borland 5.5.1 specific workarounds
    typedef weak_ptr< T > this_type;

public:

    typedef T element_type;

    weak_ptr()
      { }

    template< typename Y >
    weak_ptr( weak_ptr< Y > const & r)
        : M_lib( r.M_lib )
        , M_item( r.M_item )
      { }

    template< typename Y >
    weak_ptr( shared_ptr< Y > const & r)
        : M_lib( r.M_lib )
        , M_item( r.M_item )
      { }

#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)

    template<typename Y>
    weak_ptr & operator=( weak_ptr<Y> const & r )
      {
          M_item( r.M_item );
          M_lib( r.M_lib );
          return *this;
      }

    template<typename Y>
    weak_ptr&
    operator=( shared_ptr<Y> const & r )
      {
          M_item( r.M_item );
          M_lib( r.M_lib );
          return *this;
      }

#endif

    void reset()
      {
          this_type().swap( *this );
      }

    long use_count() const
      {
          return M_item.use_count();
      }

    bool expired() const
      {
          return M_item.use_count() == 0;
      }

    void swap( this_type & other ) // never throws
      {
          std::swap( other, *this );
      }

    bool less( this_type const & rhs ) const // implementation detail, never throws
      {
          return pn < rhs.pn;
      }

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template< typename Y > friend class weak_ptr;
    template< typename Y > friend class shared_ptr;

#endif
    WeakLib & M_lib;
    boost::weak_ptr< T > M_item;

};  // weak_ptr

template< class T, class U >
inline
bool
operator==( weak_ptr< T > const & a,
            weak_ptr< U > const & b )
{
    return a.get() == b.get();
}

template< class T, class U >
inline
bool
operator!=( weak_ptr< T > const & a,
            weak_ptr< U > const & b )
{
    return a.get() != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template< typename T >
inline
bool
operator!=( weak_ptr< T > const & a,
            weak_ptr< T > const & b )
{
    return a.get() != b.get();
}

#endif

template< class T >
inline
bool
operator<( weak_ptr< T > const & a,
           weak_ptr< T > const & b )
{
    return a.less( b );
}

template< class T >
void
swap( weak_ptr< T > & a,
      weak_ptr< T > & b )
{
    a.swap(b);
}

template< class T >
shared_ptr< T >
make_shared( weak_ptr< T > const & r ) // never throws
{
    // optimization: avoid throw overhead
    if ( r.use_count() == 0 )
    {
        return shared_ptr< T >();
    }

    try
    {
        return shared_ptr< T >( r );
    }
    catch ( boost::use_count_is_zero const & )
    {
        return shared_ptr< T >();
    }
}

} // namespace lib
} // namespace rcss

#endif  // #ifndef RCSS_DEPOBJ
