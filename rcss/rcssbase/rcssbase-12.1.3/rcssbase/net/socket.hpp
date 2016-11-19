// -*-c++-*-

/***************************************************************************
               socket.hpp  -  Base newtork socket class
                             -------------------
    begin                : 08-JAN-2003
    copyright            : (C) 2003 by The RoboCup Soccer Server
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

#ifndef RCSS_NET_SOCKET_HPP
#define RCSS_NET_SOCKET_HPP

#include "../rcssbaseconfig.hpp"

#include "addr.hpp"

#ifdef HAVE_WINSOCK2_H
#include "Winsock2.h"
#endif

#include <boost/shared_ptr.hpp>

namespace rcss {
namespace net {

class Handler;

class Socket {
public:
// #ifdef HAVE_SOCKET
//     typedef SOCKET SocketDesc;
// #else
    typedef int SocketDesc;
//#endif

    static const SocketDesc INVALIDSOCKET;

    enum CheckingType { CHECK, DONT_CHECK };

public:
    RCSSBASE_API
    static
    void closeFD( SocketDesc * s );

    RCSSBASE_API
    Socket();

    RCSSBASE_API
    Socket( SocketDesc s );

    RCSSBASE_API
    virtual
    ~Socket();

    RCSSBASE_API
    bool open();

    RCSSBASE_API
    bool bind( const Addr & addr );

    RCSSBASE_API
    Addr getName() const;

    RCSSBASE_API
    bool connect( const Addr & addr );

    RCSSBASE_API
    Addr getPeer() const;

    RCSSBASE_API
    void close();

    RCSSBASE_API
    int setCloseOnExec( bool on = true );

    RCSSBASE_API
    int setNonBlocking( bool on = true );

    RCSSBASE_API
    int setAsync( bool on = true );

    RCSSBASE_API
    int setBroadcast( bool on = true );

    RCSSBASE_API
    SocketDesc getFD() const;

    RCSSBASE_API
    bool isOpen() const;

    RCSSBASE_API
    bool isConnected() const;

    RCSSBASE_API
    Addr getDest() const; // deprecated.  Use getPeer instead.

    RCSSBASE_API
    int send( const char * msg,
              size_t len,
              const Addr& dest,
              int flags = 0,
              CheckingType check = CHECK );

    RCSSBASE_API
    int send( const char * msg,
              size_t len,
              int flags = 0,
              CheckingType check = CHECK );

    RCSSBASE_API
    int recv( char * msg,
              size_t len,
              Addr& from,
              int flags = 0,
              CheckingType check = CHECK );

    RCSSBASE_API
    int recv( char * msg,
              size_t len,
              int flags = 0,
              CheckingType check = CHECK );

    // The following two methods allow a timeout to be specified.
    // Overall, it's slower than the untimed varients so if you do
    // need to specify a timeout and you just want it the recv to
    // block or not to block, then you are better off setting the
    // socket to blocking or non-blocking and using the version
    // without timeouts.
    RCSSBASE_API
    int recv( int timeout,
              char * msg,
              size_t len,
              Addr & from,
              int flags = 0 );

    RCSSBASE_API
    int recv( int timeout,
              char * msg,
              size_t len,
              int flags = 0 );

private:
    virtual
    bool doOpen( SocketDesc& fd ) = 0;

private:
    Handler * M_handler;
    boost::shared_ptr< SocketDesc > M_handle;
};
}
}

#endif
