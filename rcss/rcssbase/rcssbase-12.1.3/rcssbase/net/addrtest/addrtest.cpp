// -*-c++-*-

/***************************************************************************
                  addrtest.cpp  - A rcss::Addr testing program
                             -------------------
    begin                : 13-Mar-2003
    copyright            : (C) 2003 by The RoboCup Soccer Server 
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU GPL as published by the Free Software   *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#include <cppunit/extensions/HelperMacros.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rcssbase/net/addr.hpp"
#include <cstring>
#include <sys/types.h>
#ifdef HAVE_NETINET_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#include <iostream>


class TestAddr : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestAddr);
    CPPUNIT_TEST(defaultConstruction);
    CPPUNIT_TEST(PortOnlyConstruction);
    CPPUNIT_TEST(PortHostConstruction);
    CPPUNIT_TEST(SetHostStr);
    CPPUNIT_TEST(SetNullHostStr); 
    CPPUNIT_TEST(SetPortStr); 
    CPPUNIT_TEST(GetPortStr);
    CPPUNIT_TEST(CopyConstruction); 
    CPPUNIT_TEST(AddrConstruction);
    CPPUNIT_TEST(PortHostFromCopyConstruction); 
    CPPUNIT_TEST(Equality);
    CPPUNIT_TEST(Inequality); 
    CPPUNIT_TEST(Assign);
    CPPUNIT_TEST(SelfAssign); 
    CPPUNIT_TEST_SUITE_END();
protected:
    //--------------------------------------------------------------------------
    /// XXX: Document what the test is for here.
    //--------------------------------------------------------------------------
    void defaultConstruction();
    void PortOnlyConstruction();
    void PortHostConstruction();
    void SetHostStr();
    void SetNullHostStr(); 
    void SetPortStr(); 
    void GetPortStr();
    void CopyConstruction(); 
    void AddrConstruction();
    void PortHostFromCopyConstruction(); 
    void Equality();
    void Inequality(); 
    void Assign();
    void SelfAssign(); 
	
public:
    TestAddr()
	{}
	
    ~TestAddr()
	{}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestAddr );

using namespace rcss::net;

void
TestAddr::defaultConstruction() 
{
    Addr a;
    CPPUNIT_ASSERT_EQUAL( Addr::PortType( 0u ), a.getPort() );
    CPPUNIT_ASSERT_EQUAL( Addr::HostType( 0u ), a.getHost() );
}

void
TestAddr::PortOnlyConstruction() 
{
    Addr a( 6000 );
    CPPUNIT_ASSERT_EQUAL( Addr::PortType( 6000u ), a.getPort() );
    CPPUNIT_ASSERT_EQUAL( Addr::HostType( 0u ), a.getHost() );
}

void
TestAddr::PortHostConstruction() 
{
    Addr a( 6000, 0 );
    CPPUNIT_ASSERT_EQUAL( Addr::PortType( 6000u ), a.getPort() );
    CPPUNIT_ASSERT_EQUAL( Addr::HostType( 0u ), a.getHost() );
}


void
TestAddr::SetHostStr() 
{
	Addr a;
	bool result = a.setHost( "localhost" );
    CPPUNIT_ASSERT( result );
	CPPUNIT_ASSERT_EQUAL( std::string( "localhost" ), a.getHostStr() );
}

void
TestAddr::SetNullHostStr() 
{
    Addr a;
    CPPUNIT_ASSERT( !a.setHost( "" ) );
}

void
TestAddr::SetPortStr() 
{
	Addr a;
    a.setPort( "telnet" );
	CPPUNIT_ASSERT_EQUAL( std::string( "telnet" ), a.getPortStr() );
	CPPUNIT_ASSERT_EQUAL( Addr::PortType( 23u ), a.getPort() );
}

void
TestAddr::GetPortStr() 
{
	Addr a( 23 );
	CPPUNIT_ASSERT_EQUAL( Addr::PortType( 23 ), a.getPort() );
	CPPUNIT_ASSERT_EQUAL( std::string( "telnet" ), a.getPortStr() );
}

void
TestAddr::CopyConstruction() 
{
	Addr a( 6000 );
	a.setHost( "localhost" );
	Addr b( a );
	CPPUNIT_ASSERT_EQUAL( Addr::PortType( 6000u ), b.getPort() );
	CPPUNIT_ASSERT_EQUAL( std::string( "localhost" ), b.getHostStr() );
}

void
TestAddr::AddrConstruction() 
{
        Addr a( 6000 );
        a.setHost( "localhost" );
        Addr b( a.getAddr() );
        CPPUNIT_ASSERT_EQUAL( a.getPort(), b.getPort() );
        CPPUNIT_ASSERT_EQUAL( a.getHost(), b.getHost() );
}

void
TestAddr::PortHostFromCopyConstruction() 
{
    Addr a( 6000 );
    a.setHost( "localhost" );
    Addr b( a.getPort(), a.getHost() );
    CPPUNIT_ASSERT_EQUAL( a.getPort(), b.getPort() );
    CPPUNIT_ASSERT_EQUAL( a.getHost(), b.getHost() );
}

void
TestAddr::Equality() 
{
        Addr b( 6000 ), a( 6000 );
        a.setHost( "localhost" );
        b.setHost( "localhost" );
        CPPUNIT_ASSERT_EQUAL( a, b );
}

void
TestAddr::Inequality() 
{
    Addr b( 6000 ), a( 6001 );
    a.setHost( "localhost" );
    b.setHost( "localhost" );
    CPPUNIT_ASSERT( a != b );
}


void
TestAddr::Assign() 
{
    Addr b, a( 6000 );
    a.setHost( "localhost" );
    b = a;
    CPPUNIT_ASSERT_EQUAL( a, b );
    CPPUNIT_ASSERT_EQUAL( a.getHostStr(), b.getHostStr() );
}

void
TestAddr::SelfAssign() 
{
    Addr a( 6000 );
    a.setHost( "localhost" );
    a = a;
    CPPUNIT_ASSERT_EQUAL( Addr::PortType( 6000u ), a.getPort() );
    CPPUNIT_ASSERT_EQUAL( std::string( "localhost" ), a.getHostStr() );
}


