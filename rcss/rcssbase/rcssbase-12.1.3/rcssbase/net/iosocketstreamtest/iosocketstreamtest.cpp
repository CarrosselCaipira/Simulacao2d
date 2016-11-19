// -*-c++-*-

/***************************************************************************
       iosocketstreamtest.cpp  - A rcss::IOSocketStream testing program 
                             -------------------
    begin                : 17-Mar-2003
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
#include "rcssbase/net/tcpsocket.hpp"
#include "rcssbase/net/iosocketstream.hpp"

class TestIOSocketStream : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestIOSocketStream);
	CPPUNIT_TEST(UDPBiDirReadWrite);
	CPPUNIT_TEST(TCPBiDirReadWrite);
    CPPUNIT_TEST_SUITE_END();
protected:
    //--------------------------------------------------------------------------
    /// XXX: Document what the test is for here.
    //--------------------------------------------------------------------------
	void UDPBiDirReadWrite();
	void TCPBiDirReadWrite();
	
public:
    TestIOSocketStream()
	{}
	
    ~TestIOSocketStream()
	{}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestIOSocketStream );

using namespace rcss::net;

void
TestIOSocketStream::UDPBiDirReadWrite() 
{
    Addr a_addr( 6001 ), b_addr( 6000 );
    a_addr.setHost( "localhost" );
    b_addr.setHost( "localhost" );
    UDPSocket a_sock( a_addr ), b_sock( b_addr );
    IOSocketStream a( a_sock, b_addr ), b( b_sock, a_addr );
    std::string input( "This_is_a_test_message" );
    std::string output;
    a << input << std::endl;
    b >> output;
    CPPUNIT_ASSERT_EQUAL( input, output );
    output = "";
    b << input << std::endl;
    a >> output;
    CPPUNIT_ASSERT_EQUAL( input, output );
}

void
TestIOSocketStream::TCPBiDirReadWrite() 
{
    Addr a_addr( 6001 ), b_addr( 6000 );
    a_addr.setHost( "localhost" );
    b_addr.setHost( "localhost" );
    TCPSocket a_sock( a_addr ), b_sock( b_addr );
    a_sock.listen( 1 );
    b_sock.connect( a_addr );
    TCPSocket to_b_sock;
    a_sock.accept( to_b_sock );
    IOSocketStream a( b_sock ), b( to_b_sock );
    std::string input( "This_is_a_test_message" );
    std::string output;
    a << input << std::endl;
    b >> output;
    CPPUNIT_ASSERT_EQUAL( input, output );
    output = "";
    b << input << std::endl;
    a >> output;
    CPPUNIT_ASSERT_EQUAL( input, output );
}

