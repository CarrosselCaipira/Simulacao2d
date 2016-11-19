// -*-c++-*-

/***************************************************************************
                          tcpsocket.hpp  -  A simple tcp socket class
                             -------------------
    begin                : 2003-11-11
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


#ifndef RCSS_NET_TCPSOCKET_HPP
#define RCSS_NET_TCPSOCKET_HPP

#include "../rcssbaseconfig.hpp"

#include "socket.hpp"

namespace rcss {
namespace net {

class TCPSocket
    : public Socket {
public:
    RCSSBASE_API
    TCPSocket();
    RCSSBASE_API
    TCPSocket( SocketDesc & s );
    RCSSBASE_API
    TCPSocket( const Addr & addr );
    RCSSBASE_API
    TCPSocket( const Addr & addr,
               const Addr & dest );

    RCSSBASE_API
    bool accept( TCPSocket & sock );

    RCSSBASE_API
    bool listen( int backlog );

private:
    bool doOpen( SocketDesc& fd );
};

}
}

#endif
