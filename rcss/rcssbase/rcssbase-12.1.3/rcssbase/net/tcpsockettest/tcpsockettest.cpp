// -*-c++-*-

/***************************************************************************
            tcpsockettest.cpp  - A rcss::TCPSocket testing program 
                             -------------------
    begin                : 2003-11-12
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

#include "rcssbase/net/tcpsocket.hpp"
#include <iostream>
#include <errno.h>

class TestTCPSocket : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestTCPSocket);
    CPPUNIT_TEST( defaultConstruction ); 
	CPPUNIT_TEST(GetName);
	CPPUNIT_TEST(Bind);
	CPPUNIT_TEST(DoubleBind);
	CPPUNIT_TEST(GetPeer);
	CPPUNIT_TEST(BindConstruction);
	CPPUNIT_TEST(Connect);
	CPPUNIT_TEST(ConnectConstruction);
	CPPUNIT_TEST(SendRecv);
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
    TestTCPSocket()
	{}
	
    ~TestTCPSocket()
	{}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTCPSocket );

using namespace rcss::net;

void
TestTCPSocket::defaultConstruction() 
{
   TCPSocket s;
   CPPUNIT_ASSERT( s.isOpen() );
   CPPUNIT_ASSERT( !s.isConnected() );
}

void
TestTCPSocket::GetName() 
{
    TCPSocket s;
    CPPUNIT_ASSERT_EQUAL( Addr(), s.getName() );
}


void
TestTCPSocket::Bind() 
{
    TCPSocket s;
    Addr a( 12001 );
    bool res;
    CPPUNIT_ASSERT( res = s.bind( a ) );
    if( !res )
    {
      std::cerr << strerror( errno ) << std::endl;
    }
    else
        CPPUNIT_ASSERT_EQUAL( a, s.getName() );
}

void
TestTCPSocket::DoubleBind() 
{
    Addr a( 12001 );
    TCPSocket s1( a ), s2( a );
    CPPUNIT_ASSERT( s2.getName() != a );
}

void
TestTCPSocket::GetPeer() 
{
    TCPSocket s;
    CPPUNIT_ASSERT_EQUAL( s.getPeer(), Addr() );
}


void
TestTCPSocket::BindConstruction() 
{
    TCPSocket s( Addr( 12001 ) );
    CPPUNIT_ASSERT_EQUAL( Addr( 12001 ), s.getName() );
    CPPUNIT_ASSERT( s.isOpen() );
    CPPUNIT_ASSERT( !s.isConnected() );
}

void
TestTCPSocket::Connect() 
{
    Addr a( 12001 );
    a.setHost( "localhost" );
    TCPSocket s( a );
    s.listen( 1 );
    TCPSocket s2, s3;
    bool res;
    CPPUNIT_ASSERT( res = s2.connect( a ) );
    if( !res )
    {
        std::cerr << strerror( errno ) << std::endl;
    }
    else
    {
        CPPUNIT_ASSERT( s.accept( s3 ) );
        CPPUNIT_ASSERT( s2.isConnected() );
        CPPUNIT_ASSERT( s3.isConnected() );
        CPPUNIT_ASSERT_EQUAL( s3.getName(), s2.getPeer() );
        CPPUNIT_ASSERT_EQUAL( s2.getName(), s3.getPeer() );
    }
}

void
TestTCPSocket::ConnectConstruction() 
{
    Addr a( 12002 ), b;
    CPPUNIT_ASSERT( a.setHost( "localhost" ) );
    TCPSocket s;
    bool res;
    CPPUNIT_ASSERT( res = s.bind( a ) );
    if( !res )
    {
        std::cerr << strerror( errno ) << std::endl;
    }
    else
    {
        CPPUNIT_ASSERT( s.listen( 1 ) );
        TCPSocket s2( b, a );
        TCPSocket s3;
        CPPUNIT_ASSERT( s.accept( s3 ) );
        CPPUNIT_ASSERT( s2.isConnected() );
        CPPUNIT_ASSERT_EQUAL( s3.getName(), s2.getPeer() );
        CPPUNIT_ASSERT_EQUAL( s3.getPeer(), s2.getName() );
    }
}

void
TestTCPSocket::SendRecv() 
{
	Addr a( 12003 ), b;
    a.setHost( "localhost" );
    TCPSocket s( a );
    s.listen( 1 );
    TCPSocket s2( b, a );
    TCPSocket s3;
    s.accept( s3 );
    std::string testmsg( "This is a test message" );
    int sent = s2.send( testmsg.c_str(), testmsg.size() + 1 );
    CPPUNIT_ASSERT_EQUAL( (int)testmsg.size() + 1, sent );
    char buf[64];
    int recved = s3.recv( buf, 64 );
    CPPUNIT_ASSERT_EQUAL( sent, recved );
    CPPUNIT_ASSERT_EQUAL( testmsg, std::string( buf ) );
}


