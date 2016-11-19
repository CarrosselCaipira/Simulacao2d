// -*-c++-*-

/***************************************************************************
                  buildertest.cpp  - A rcss::conf::Builder testing program
                             -------------------
    begin                : 10-June-2003
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

class TestBuilder : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestBuilder);
	CPPUNIT_TEST(NormalVar);
	CPPUNIT_TEST(MemberVar);
	CPPUNIT_TEST(MemberFunc);
	CPPUNIT_TEST(ExternalFunc);
    CPPUNIT_TEST_SUITE_END();
protected:
    //--------------------------------------------------------------------------
    /// XXX: Document what the test is for here.
    //--------------------------------------------------------------------------
	void NormalVar();
	void MemberVar();
	void MemberFunc();
	void ExternalFunc();
	
public:
    TestBuilder()
	{}
	
    ~TestBuilder()
	{}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestBuilder );

#include "rcssbase/conf/parser.hpp"
#include "rcssbase/conf/builder.hpp"
#include <stdio.h>

using namespace rcss::conf;

int GLOBAL = 5;

void
setFunc( int v )
{
    GLOBAL = v;
}

int
getFunc()
{
    return GLOBAL;
}

class test3
{
public:
    void
    operator()( int v )
    { test = v; }

    int
    operator()() const
    { return test; }

    int test;
};

class MyBuilder
    : public Builder
{
public:
  MyBuilder()
    : Builder( "buildertest", "test" ),
          m_test( 5 )
    {}

    void
    addExternalVar( int& test )
    { 
        Builder::addParam( "test", test, "test" ); 
    }

    void
    addMemVar()
    { 
        Builder::addParam( "test", m_test, "test" ); 
    }

    void
    addMemFunc()
    { 
        Builder::addParam( "test",
						   makeSetter( this, &MyBuilder::setTest ), 
						   makeGetter( this, &MyBuilder::getTest ), "test" ); 
    }

    void
    addExternalFunc( void(*s)(int), int(*g)() )
    { 
        Builder::addParam( "test", makeSetter( s ), makeGetter( g ), "test" ); 
    }

    template< typename V >
    void
    set( const std::string& param, V value )
    {
        Builder::set( param, value );
    }

    int
    getTest() const
    {
        return m_test;
    }

    void
    setTest( int value )
    {
        m_test = value;
    }

    template< typename V >
    bool
    get( const std::string& param, V& value )
    {
        return Builder::get( param, value );
    }
    
    int m_test;
};

void
TestBuilder::NormalVar() 
{   
    int test = 5;
    MyBuilder tester;
    tester.addExternalVar( test );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( test, tmp );
    tester.set( "test", 10 );
    CPPUNIT_ASSERT_EQUAL( 10, test );
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( test, tmp );
    test = 2;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( test, tmp );
}

void
TestBuilder::MemberVar() 
{   
    MyBuilder tester;
    tester.addMemVar();
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 5, tmp );
    tester.set( "test", 10 );
    CPPUNIT_ASSERT_EQUAL( 10, tester.m_test );
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 10, tmp );
    tester.m_test = 2;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void
TestBuilder::MemberFunc() 
{   
    MyBuilder tester;
    tester.addMemFunc();
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 5, tmp );
    tester.set( "test", 10 );
    CPPUNIT_ASSERT_EQUAL( 10, tester.m_test );
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 10, tmp );
    tester.m_test = 2;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void
TestBuilder::ExternalFunc() 
{   
    MyBuilder tester;
    tester.addExternalFunc( &setFunc, &getFunc );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 5, tmp );
    tester.set( "test", 10 );
    CPPUNIT_ASSERT_EQUAL( 10, GLOBAL );
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 10, tmp );
    GLOBAL = 2;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}


