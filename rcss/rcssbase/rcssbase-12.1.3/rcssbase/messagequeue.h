// -*-c++-*-

/***************************************************************************
                               messagequeue.h
                      Class storing messages to multiple threads
                             -------------------
    begin                : 22-APR-2002
    copyright            : (C) 2002 by The RoboCup Soccer Server
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

#ifndef RCSSBASE_MESSAGEQUEUE_H
#define RCSSBASE_MESSAGEQUEUE_H

//#include "types.h"
#include <iostream>
#include <deque>
#include <rcssbase/cond.h>

namespace rcss {
namespace thread {

template< typename DATA_TYPE >
class FreeDestroyPolicy {
public:
    static
    void destroy( DATA_TYPE & data )
      {
          free( data );
      }
};

template< typename DATA_TYPE >
class VoidDestroyPolicy {
public:
    static
    void destroy( DATA_TYPE & )
      { }
};

template< typename DATA_TYPE >
class DeleteDestroyPolicy {
public:
    static
    void destroy( DATA_TYPE & data )
      {
          delete data;
      }
};

template< typename DATA_TYPE >
class DeleteArrayDestroyPolicy {
public:
    static
    void destroy( DATA_TYPE & data )
      {
          delete [] data;
      }
};


template< typename DATA_TYPE,
          template< class > class DESTROY_POLICY = VoidDestroyPolicy >
class MessageQueue {
public:
    typedef DATA_TYPE DataType;
    typedef DESTROY_POLICY< DataType > DestroyPolicy;
private:
    std::deque< DataType > M_data_queue;
    Cond M_cond;
public:
    ~MessageQueue()
      {
          Cond::Lock lock( M_cond );
          while ( ! M_data_queue.empty() )
          {
              DestroyPolicy::destroy( M_data_queue.back() );
              M_data_queue.pop_back();
          }
      }

    void queueMessage( const DataType & data )
      {
          Cond::Lock lock( M_cond );
          M_data_queue.push_back( data );
          M_cond.signal();
      }

    DataType getNext()
      {
          Cond::Lock lock( M_cond );
          if ( M_data_queue.empty() )
          {
              M_cond.wait( lock );
          }
          DataType rval = M_data_queue.front();
          M_data_queue.pop_front();
          return rval;
      }

    DataType getNext( long seconds,
                      long usec ) throw( Cond::TimeOut,
                                         Cond::Interrupted )
      {
          Cond::Lock lock( M_cond );
          if ( M_data_queue.empty() )
          {
              M_cond.wait( lock, seconds, usec ); // will throw if times out.
          }
          DataType rval = M_data_queue.front();
          M_data_queue.pop_front();
          return rval;
      }
};

}
}

#endif
