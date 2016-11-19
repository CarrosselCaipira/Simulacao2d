// -*-c++-*-

/***************************************************************************
            udpsockettest.cpp  - A rcss::UDPSocket testing program 
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

#include "rcssbase/net/udpsocket.hpp"

class TestUDPSocket : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestUDPSocket);
    CPPUNIT_TEST( defaultConstruction ); 
    CPPUNIT_TEST( GetName ); 
    CPPUNIT_TEST( Bind ); 
    CPPUNIT_TEST( DoubleBind ); 
    CPPUNIT_TEST( GetPeer ); 
    CPPUNIT_TEST( BindConstruction ); 
    CPPUNIT_TEST( Connect ); 
    CPPUNIT_TEST( ConnectConstruction ); 
    CPPUNIT_TEST( SendRecv ); 
    CPPUNIT_TEST_SUITE_END();
protected:
    //--------------------------------------------------------------------------
    /// XXX: Document what the test is for here.
    //--------------------------------------------------------------------------
    void defaultConstruction(); 
    void GetName(); 
    void Bind(); 
    void DoubleBind(); 
    void GetPeer(); 
    void BindConstruction(); 
    void Connect(); 
    void ConnectConstruction(); 
    void SendRecv(); 
	
public:
    TestUDPSocket()
	{}
	
    ~TestUDPSocket()
	{}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestUDPSocket );

using namespace rcss::net;


void
TestUDPSocket::defaultConstruction() 
{
   UDPSocket s;
   CPPUNIT_ASSERT( s.isOpen() );
   CPPUNIT_ASSERT( !s.isConnected() );
}

void
TestUDPSocket::GetName() 
{
    UDPSocket s;
    CPPUNIT_ASSERT_EQUAL( Addr(), s.getName() );
}


void
TestUDPSocket::Bind() 
{
    UDPSocket s;
    Addr a( 6000 );
    s.bind( a );
    CPPUNIT_ASSERT_EQUAL( a, s.getName() );
}

void
TestUDPSocket::DoubleBind() 
{
    Addr a( 6000 );
    UDPSocket s1( a ), s2( a );
    CPPUNIT_ASSERT( s2.getName() != a );
}

void
TestUDPSocket::GetPeer() 
{
    UDPSocket s;
    CPPUNIT_ASSERT_EQUAL( s.getPeer(), Addr() );
}


void
TestUDPSocket::BindConstruction() 
{
    UDPSocket s( Addr( 6000 ) );
    CPPUNIT_ASSERT_EQUAL( Addr( 6000 ), s.getName() );
    CPPUNIT_ASSERT( s.isOpen() );
    CPPUNIT_ASSERT( !s.isConnected() );
}

void
TestUDPSocket::Connect() 
{
    Addr a( 6000 );
    a.setHost( "localhost" );
    UDPSocket s( a ), s2;
    s2.connect( a );
    CPPUNIT_ASSERT( s2.isConnected() );
    CPPUNIT_ASSERT_EQUAL( a, s2.getPeer() );
}

void
TestUDPSocket::ConnectConstruction() 
{
    Addr a( 6000 ), b;
    a.setHost( "localhost" );
    UDPSocket s( a ), s2( b, a );
    CPPUNIT_ASSERT( s2.isConnected() );
    CPPUNIT_ASSERT_EQUAL( a, s2.getPeer() );
}

void
TestUDPSocket::SendRecv() 
{
	Addr a( 6000 ), b;
    a.setHost( "localhost" );
    UDPSocket s( a ), s2( b, a );
    std::string testmsg( "This is a test message" );
	int sent = s2.send( testmsg.c_str(), testmsg.size() + 1 );
    CPPUNIT_ASSERT_EQUAL( (int)testmsg.size() + 1, sent );
    char buf[64];
    int recved = s.recv( buf, 64 );
    CPPUNIT_ASSERT_EQUAL( sent, recved );
    CPPUNIT_ASSERT_EQUAL( testmsg, std::string( buf ) );
}

// void
// TestUDPSocket::SendRecvBCast() 
// {
//     Addr a( 6000 ), b( 6000, Addr::BROADCAST );
//     a.setHost( "localhost" );
//     UDPSocket s( a ), s2;
//     CPPUNIT_ASSERT_EQUAL( 0, s2.setBroadcast() );
//     std::string testmsg( "This is a test message" );
//     int sent = s2.send( testmsg.c_str(), testmsg.size() + 1, b );
//     CPPUNIT_ASSERT_EQUAL( (int)testmsg.size() + 1, sent );
//     if( sent == -1 )
//         std::cerr << strerror( errno ) << std::endl;
//     else
//     {
//         char buf[64];
//         Addr src;
//         int recved = s.recv( buf, 64, src );
//         CPPUNIT_ASSERT_EQUAL( sent, recved );
//         CPPUNIT_ASSERT_EQUAL( testmsg, buf );
//     }
// }

