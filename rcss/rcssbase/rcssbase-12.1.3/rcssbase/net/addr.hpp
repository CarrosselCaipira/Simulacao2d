// -*-c++-*-

/***************************************************************************
                          addr.hpp  - A network address class
                             -------------------
    begin                : 07-JAN-2003
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

#ifndef RCSS_NET_ADDR_HPP
#define RCSS_NET_ADDR_HPP

#include "../rcssbaseconfig.hpp"

struct sockaddr_in;

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

namespace rcss {
namespace net {

class AddrImpl;

class Addr {
public:
    typedef boost::uint16_t PortType;
    typedef boost::uint32_t HostType;
    typedef struct sockaddr_in AddrType;

    enum Error { S_ADDR_OK, S_SERV_NOT_FOUND, S_HOST_NOT_FOUND };

    RCSSBASE_API
    static const HostType BROADCAST;
    RCSSBASE_API
    static const HostType ANY;

    RCSSBASE_API
    Addr( PortType port = 0,
          HostType host = Addr::ANY );
    RCSSBASE_API
    Addr( const AddrType & addr );

    RCSSBASE_API
    bool setPort( PortType port = 0 );
    RCSSBASE_API
    bool setPort( const std::string & port,
                  const std::string & proto = "" );

    RCSSBASE_API
    bool setHost( HostType host = Addr::ANY );
    RCSSBASE_API
    bool setHost( const std::string & host );

    RCSSBASE_API
    const
    AddrType & getAddr() const;

    RCSSBASE_API
    PortType getPort() const;

    RCSSBASE_API
    HostType getHost() const;

    RCSSBASE_API
    std::string getHostStr() const;

    RCSSBASE_API
    std::string getPortStr( const std::string & proto = "" ) const;

private:
    boost::shared_ptr< AddrImpl > M_impl;
};

RCSSBASE_API
bool
operator==( const Addr & a,
            const Addr & b );

RCSSBASE_API
bool
operator!=( const Addr & a,
            const Addr & b );

RCSSBASE_API
std::ostream&
operator<<( std::ostream & o,
            const Addr & addr );

}
}

#endif
