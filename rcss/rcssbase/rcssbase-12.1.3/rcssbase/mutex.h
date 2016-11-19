// -*-c++-*-

/***************************************************************************
                                    mutex.h
                       Wrapper for pthread mutexes
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

#ifndef RCSSBASE_MUTEX_H
#define RCSSBASE_MUTEX_H

#include <list>

#include <pthread.h>

namespace rcss {
namespace thread {

class Mutex {
private:
    pthread_mutex_t M_mutex;

    Mutex( const Mutex& ); // not allowed

    Mutex & operator=( const Mutex& ); // not allowed

    int lock()
      {
          return pthread_mutex_lock( &M_mutex );
      }

    int tryLock()
      {
          return pthread_mutex_trylock( &M_mutex );
      }

    int unlock()
      {
          return pthread_mutex_unlock( &M_mutex );
      }

protected:
    pthread_mutex_t & getMutex()
      {
          return M_mutex;
      }

public:
    Mutex()
      {
          pthread_mutex_init( &M_mutex, NULL );
      }

    ~Mutex()
      {
          pthread_mutex_destroy( &M_mutex );
      }

    friend class Lock;
    class Lock {
    private:
        Mutex& M_mutex;
    public:
        Lock( Mutex & mutex )
            : M_mutex( mutex )
          {
              M_mutex.lock();
          }

        ~Lock()
          {
              M_mutex.unlock();
          }
    };

    friend class TryLock;
    class TryLock {
    private:
        Mutex * M_mutex;
    public:
        TryLock( Mutex & mutex )
            : M_mutex( &mutex )
          {
              if ( M_mutex->tryLock() != 0 )
              {
                  M_mutex = NULL;
              }
          }

        ~TryLock()
          {
              if ( M_mutex != NULL )
              {
                  M_mutex->unlock();
              }
          }

        bool locked() const
          {
              return M_mutex != NULL;
          }

    };
};

class RWMutex
    : public Mutex {
private:
    RWMutex( const RWMutex & ); // not allowed

    RWMutex & operator=( const RWMutex & ); // not allowed

    std::list< Mutex * > M_read_mutexes;
    std::list< Mutex::Lock * > M_read_locks;

    Mutex M_list_mutex;
    Mutex::Lock * M_list_lock;

    void writeSubLock()
      {
          Mutex::Lock * lock = new Mutex::Lock( M_list_mutex );

          // we only change the value of the list lock, once we are sure
          // we have the lock.
          M_list_lock = lock;
      }

    void
    writeSubUnlock()
      {
          // Make sure we clear the list lock, before we delete (and
          // thus give up) the lock.
          Mutex::Lock * lock = M_list_lock;
          M_list_lock = NULL;

          delete lock;
      }

    void
    writeLock()
      {
          // lock each read mutex and add the lock to the locks list
          writeSubLock();

          for ( std::list< Mutex * >::iterator i = M_read_mutexes.begin();
                i != M_read_mutexes.end();
                ++i )
          {
              M_read_locks.push_front( new Mutex::Lock( **i ) );
          }
      }

    void writeUnlock()
      {
          // unlock each read mutex and add then unlock to the locks list
          for ( std::list< Mutex::Lock * >::iterator i = M_read_locks.begin();
                i != M_read_locks.end();
                ++i )
          {
              delete *i;
          }
          M_read_locks.clear();

          writeSubUnlock();
      }

    std::pair< Mutex *, Mutex::Lock * > readLock()
      {
          std::pair< Mutex *, Mutex::Lock * > rval;
          rval.first = new Mutex;
          {
              // grab the list lock while we are updating the list only!
              // by avoiding locking both the list mutex and the newly
              // created mutex we don't even have to figure out if there
              // could be some way of deadlocking it.
              Mutex::Lock lock( M_list_mutex );
              M_read_mutexes.push_front( rval.first );
          }
          rval.second = new Mutex::Lock( *( rval.first ) );

          // we return a pointer to the lock and the mutex, so when we
          // unlock, we delete the lock and know what mutex to remove.
          return rval;
      }

    void readUnlock( std::pair< Mutex *, Mutex::Lock * >& mutex_and_lock )
      {
          // release our lock of the mutex.
          delete mutex_and_lock.second;

          // lock the list and remove the mutex
          Mutex::Lock lock( M_list_mutex );
          M_read_mutexes.remove( mutex_and_lock.first );

          // Get rid of the mutex.  We may cache these mutexes for reuse
          // later on, but we'll write that code later.
          delete mutex_and_lock.first;
      }

public:
    RWMutex()
      { }

    friend class ReadLock;
    class ReadLock {
    private:
        RWMutex & M_mutex;
        std::pair< Mutex *, Mutex::Lock * > M_read_mutex_and_lock;
    public:
        ReadLock( RWMutex & mutex )
            : M_mutex( mutex ),
              M_read_mutex_and_lock( M_mutex.readLock() )
          { }

        ~ReadLock()
          {
              M_mutex.readUnlock( M_read_mutex_and_lock );
          }
    };

    friend class WriteLock;
    class WriteLock {
    private:
        RWMutex& M_mutex;
    public:
        WriteLock( RWMutex & mutex )
            : M_mutex( mutex )
          {
              M_mutex.writeLock();
          }

        ~WriteLock()
          {
              M_mutex.writeUnlock();
          }
    };
};

}
}

#endif
