// -*-c++-*-

/***************************************************************************
                                    thread.h
                             Wrapper for pthreads
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

#ifndef RCSSBASE_THREAD_H
#define RCSSBASE_THREAD_H

#include <pthread.h>
#include <iostream>

namespace rcss {
namespace thread {

class Thread {
private:
    class RunnableHolderBase {
    public:
        RunnableHolderBase()
          { }

        virtual
        ~RunnableHolderBase()
          { }

        virtual
        void operator()() = 0;
    };

    template< typename T >
    class RunnableHolder
        : public RunnableHolderBase {
    private:
        typedef T Runnable;

        Runnable & M_runnable;
    public:
        RunnableHolder( Runnable & runnable )
            : M_runnable( runnable )
          { }

        virtual
        ~RunnableHolder()
          { }

        virtual
        void operator()()
          {
              M_runnable();
          }
    };

    class Param {
    private:
        RunnableHolderBase * M_runnable_holder;
    public:
        Param( RunnableHolderBase * runnable_holder )
            : M_runnable_holder( runnable_holder )
          {}

        ~Param()
          {
              delete M_runnable_holder;
          }

        void operator()()
          {
              (*M_runnable_holder)();
          }
    };

    static
    void * startThread( void * param )
      {
          Param* p = static_cast<Param*>(param);
          (*p)();
          return NULL;
      }

    pthread_t M_thread;

    Param M_param;
    // param has the same lifetime as the thread because if it only has
    // scope for the duration of the constructor, the param object may
    // be detroyed before the thread is started.

public:
    // Thread requires that runnable is a class that implements operator()()
    template< typename T >
    Thread( T& runnable )
        : M_param( new RunnableHolder< T >( runnable ) )
      { }

    ~Thread()
      {
          cancel();
          join();
      }

    bool start()
      {
          int err = pthread_create( &M_thread, 0, &startThread, &M_param );
          return err == 0;
      }

    void join()
      {
          pthread_join( M_thread, NULL );
      }

    void cancel()
      {
          pthread_cancel( M_thread );
      }

    static
    void testCancel()
      {
          pthread_testcancel();
      }
};

}
}

#endif
