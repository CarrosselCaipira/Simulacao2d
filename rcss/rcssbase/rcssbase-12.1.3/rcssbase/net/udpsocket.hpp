// -*-c++-*-

/***************************************************************************
                          udpsocket.hpp  -  A simple udp socket class
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


#ifndef RCSS_NET_UDPSOCKET_HPP
#define RCSS_NET_UDPSOCKET_HPP

#include "../rcssbaseconfig.hpp"

#include "socket.hpp"

namespace rcss {
namespace net {

class UDPSocket
    : public Socket {
public:
    RCSSBASE_API
    UDPSocket();
    RCSSBASE_API
    UDPSocket( SocketDesc & s );
    RCSSBASE_API
    UDPSocket( const Addr & addr );
    RCSSBASE_API
    UDPSocket( const Addr & addr,
               const Addr & dest );

private:
    bool
    doOpen( SocketDesc & fd );
};

}
}

#endif
