// -*-c++-*-

/***************************************************************************
                                    cond.h
                    Wrapper for pthread condition variables
                             -------------------
    begin                : 23-APR-2002
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

#ifndef RCSSBASE_COND_H
#define RCSSBASE_COND_H

#include <rcssbase/mutex.h>

#include <exception>
#include <cerrno>
#include <sys/time.h>
#include <pthread.h>

namespace rcss {
namespace thread {

class Cond
    : public Mutex {
public:
    class TimeOut
        : public std::exception {
    public:
        const
        char * what() const throw()
          {
              static char rval[] = "Timeout waiting on condition";
              return rval;
          }
    };

    class Interrupted
        : public std::exception {
    public:
        const
        char * what() const throw()
          {
              static char rval[] = "Waiting on condition was interrupted";
              return rval;
          }
    };

private:
    pthread_cond_t M_cond;

public:
    Cond()
      {
          pthread_cond_init( &M_cond, NULL );
      }

    ~Cond()
      {
          pthread_cond_destroy( &M_cond );
      }

    void signal()
      {
          pthread_cond_signal( &M_cond );
      }

    void broadcast()
      {
          pthread_cond_broadcast( &M_cond );
      }

    void wait( const Lock & )
      {
          pthread_cond_wait( &M_cond, &getMutex() );
      }

    void
    wait( const Lock &,
          long seconds,
          long nanoseconds = 0 ) throw( TimeOut, Interrupted )
      {
          timeval tv;
          gettimeofday( &tv, NULL );
          timespec abstime = { seconds + tv.tv_sec, nanoseconds + tv.tv_usec * 1000 };
          abstime.tv_sec += abstime.tv_nsec / 1000000000;
          abstime.tv_nsec %= 1000000000;
          int err = pthread_cond_timedwait( &M_cond, &getMutex(), &abstime );
          switch( err ) {
          case ETIMEDOUT:
              throw TimeOut();
          case EINTR:
              throw Interrupted();
          default:
              break;
          }
      }

    void wait()
      {
          Lock lock( *this );
          wait( lock );
      }

    void wait( long seconds,
               long nanoseconds = 0 ) throw( TimeOut, Interrupted )
      {
          Lock lock( *this );
          wait( lock, seconds, nanoseconds );
      }
};

}
}

#endif
