// -*-c++-*-

/***************************************************************************
                  parsertest.cpp  - A rcss::conf::Parser testing program
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

class TestParser : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestParser);
	CPPUNIT_TEST(NullArgs);
	CPPUNIT_TEST(NullUnnamedStrm);
	CPPUNIT_TEST(NullNamedStrm);
	CPPUNIT_TEST(ValidArg);
	CPPUNIT_TEST(ValidSubArg);
	CPPUNIT_TEST(ValidArg2);
	CPPUNIT_TEST(ValidArg3);
	CPPUNIT_TEST(ValidArgMinus);
	CPPUNIT_TEST(ValidArgMinusMinus);
	CPPUNIT_TEST(InvalidArg);
	CPPUNIT_TEST(WrongNamespaceArg);
	CPPUNIT_TEST(ValidWSArg);
	CPPUNIT_TEST(ValidStrm);
	CPPUNIT_TEST(InvalidStrm);
	CPPUNIT_TEST(WrongnamespaceStrm);
	CPPUNIT_TEST(ValidWSStrm);
	CPPUNIT_TEST(ValidCommentStrm);
	CPPUNIT_TEST(ConfFile);
	CPPUNIT_TEST(Help);
	CPPUNIT_TEST(TestHelp);
	CPPUNIT_TEST(TestSubHelp);
	CPPUNIT_TEST(TestSetPath);
	CPPUNIT_TEST(TestAddPath);
	CPPUNIT_TEST(TestInclude);
	CPPUNIT_TEST(TestLoad);
    CPPUNIT_TEST_SUITE_END();
protected:
    //--------------------------------------------------------------------------
    /// XXX: Document what the test is for here.
    //--------------------------------------------------------------------------
	void NullArgs();
	void NullUnnamedStrm();
	void NullNamedStrm();
	void ValidArg();
	void ValidSubArg();
	void ValidArg2();
	void ValidArg3();
	void ValidArgMinus();
	void ValidArgMinusMinus();
	void InvalidArg();
	void WrongNamespaceArg();
	void ValidWSArg();
	void ValidStrm();
	void InvalidStrm();
	void WrongnamespaceStrm();
	void ValidWSStrm();
	void ValidCommentStrm();
	void ConfFile();
	void Help();
	void TestHelp();
	void TestSubHelp();
	void TestSetPath();
	void TestAddPath();
	void TestInclude();
	void TestLoad();

public:
    TestParser()
	{}

    ~TestParser()
	{}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestParser );

#include <fstream>
#include <cstdlib>
#include <cstdio>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream>
#endif

#include <iostream>

#include "parser.hpp"
#include "builder.hpp"
#include "../lib/loader.hpp"
#include <boost/filesystem/operations.hpp>
#include "streamstatushandler.hpp"

//std::ofstream dev_null( "/dev/null" );
std::ostream& dev_null = std::cout;

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
      : Builder( "parsertest", "test" ),
		m_test( 5 )
    {
	addHandler( m_handler );
        addMemVar();
    }

    ~MyBuilder()
    {
	removeHandler( m_handler );
    }

    void
    addMemVar()
    {
        Builder::addParam( "test",
			   makeSetter( m_test ),
			   makeGetter( m_test ),
			   "test desc" );
        Builder::addParam( "test_2", makeSetter( m_test ), makeGetter( m_test ), "test_2 desc" );
        Builder::addParam( "3%te-st_2", makeSetter( m_test ), makeGetter( m_test ), "3%te-st_2 desc" );
    }

    template< typename V >
    void
    set( const std::string& param, V value )
    {
        Builder::set< V >( param, value );
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
	StreamStatusHandler m_handler;
};


class MySubBuilder
    : public Builder
{
public:
    MySubBuilder( Builder* parent )
      : Builder( parent, "test::subtest" ),
		m_test( 6 )
    {
		addHandler( m_handler );
        addMemVar();
    }

	~MySubBuilder()
	{
		removeHandler( m_handler );
	}

    void
    addMemVar()
    {
        Builder::addParam( "test",
						   makeSetter( m_test ),
						   makeGetter( m_test ),
						   "test desc" );
        Builder::addParam( "test_2", makeSetter( m_test ), makeGetter( m_test ), "test_2 desc" );
        Builder::addParam( "3%te-st_2", makeSetter( m_test ), makeGetter( m_test ), "3%te-st_2 desc" );
    }

    template< typename V >
    void
    set( const std::string& param, V value )
    {
        Builder::set< V >( param, value );
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
	StreamStatusHandler m_handler;
};


void TestParser::NullArgs()
{
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    try
    {
        CPPUNIT_ASSERT( parser.parse( 0, NULL ) );
    }
    catch(...)
    {
        CPPUNIT_ASSERT( false );
    }
}

void TestParser::NullUnnamedStrm()
{
#ifdef HAVE_SSTREAM
    std::istringstream strm( "" );
#else
    std::istrstream strm( "" );
#endif
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    try
    {
        CPPUNIT_ASSERT( parser.parse( strm ) );
    }
    catch(...)
    {
        CPPUNIT_ASSERT( false );
    }
}

void TestParser::NullNamedStrm()
{
#ifdef HAVE_SSTREAM
    std::istringstream strm( "" );
#else
    std::istrstream strm( "" );
#endif
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    try
    {
        CPPUNIT_ASSERT( parser.parse( strm, "test" ) );
    }
    catch(...)
    {
        CPPUNIT_ASSERT( false );
    }
}

void TestParser::ValidArg()
{
    const char* argv[] = { "parsertest", "test::test=2" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
	int argc = 2;
    CPPUNIT_ASSERT( parser.parse( argc, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::ValidSubArg()
{
    const char* argv[] = { "parsertest", "test::subtest::test=2" };
    MyBuilder tester;
	MySubBuilder subtester( &tester );
    rcss::conf::Parser parser( tester );
	int argc = 2;
    CPPUNIT_ASSERT( parser.parse( argc, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( subtester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::ValidArg2()
{
    const char* argv[] = { "parsertest", "test::test_2=2" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test_2", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::ValidArg3()
{
    const char* argv[] = { "parsertest", "test::3%te-st_2=2" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "3%te-st_2", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::ValidArgMinus()
{
    const char* argv[] = { "parsertest", "-test::test=2" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::ValidArgMinusMinus()
{
    const char* argv[] = { "parsertest", "--test::test=2" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::InvalidArg()
{
    const char* argv[] = { "parsertest", "test::foo=2" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( !parser.parse( 2, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 5, tmp );
}

void TestParser::WrongNamespaceArg()
{
    const char* argv[] = { "parsertest", "foo::bar=2" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 5, tmp );
}

void TestParser::ValidWSArg()
{
    const char* argv[] = { "parsertest", "test::test", "  =", "  \n2\t  " };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 4, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::ValidStrm()
{
#ifdef HAVE_SSTREAM
    std::istringstream strm( "test::test=2" );
#else
    std::istrstream strm( "test::test=2" );
#endif
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( strm, "test" ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::InvalidStrm()
{
#ifdef HAVE_SSTREAM
    std::istringstream strm( "test::foo=2" );
#else
    std::istrstream strm( "test::foo=2" );
#endif
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( !parser.parse( strm ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 5, tmp );
}

void TestParser::WrongnamespaceStrm()
{
#ifdef HAVE_SSTREAM
    std::istringstream strm( "foo::bar=2" );
#else
    std::istrstream strm( "foo::bar=2" );
#endif
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( strm, "test" ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 5, tmp );
}

void TestParser::ValidWSStrm()
{
#ifdef HAVE_SSTREAM
    std::istringstream strm( "test::test   = \n2\t" );
#else
    std::istrstream strm( "test::test   = \n2\t" );
#endif
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( strm, "test" ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::ValidCommentStrm()
{
#ifdef HAVE_SSTREAM
    std::istringstream
#else
    std::istrstream
#endif
        strm( "/* thi#s //is a/* *comment */ test::test   = // this is// #an*ot/he/*r comm*/*ent\n # */an/*d ano//th#er\n2\t" );
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( strm, "test" ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
}

void TestParser::ConfFile()
{
    boost::filesystem::path filename = "rcssbase/conf/tmp.conf";
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    parser.parseCreateConf( filename, "test" );
    tester.setTest( 2 );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );
    CPPUNIT_ASSERT( parser.parse( filename ) );
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 5, tmp );
}

void TestParser::Help()
{
    const char* argv[] = { "parsertest", "--help" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    CPPUNIT_ASSERT( tester.genericHelpRequested() );
    tester.displayHelp();
}


void TestParser::TestHelp()
{

    const char* argv[] = { "parsertest", "--test::help" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    CPPUNIT_ASSERT( tester.detailedHelpRequested() );
    tester.displayHelp();
}

void TestParser::TestSubHelp()
{
    const char* argv[] = { "parsertest", "--test::subtest::help" };
    MyBuilder tester;
    MySubBuilder subtester( &tester );
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    CPPUNIT_ASSERT( subtester.detailedHelpRequested() );
    tester.displayHelp();
}

void TestParser::TestSetPath()
{
    const char* argv[] = { "parsertest", "--setpath=rcssbase/lib/loadertest" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ),
						  rcss::lib::Loader::getPath().size() );
    CPPUNIT_ASSERT_EQUAL( std::string( "rcssbase/lib/loadertest" ),
						  rcss::lib::Loader::getPath().front().string() );
}

void TestParser::TestAddPath()
{
    const char* argv[] = { "parsertest", "--addpath=rcssbase/conf" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    CPPUNIT_ASSERT_EQUAL( size_t( 2u ),
						  rcss::lib::Loader::getPath().size() );
    CPPUNIT_ASSERT_EQUAL( std::string( "rcssbase/lib/loadertest" ),
						  rcss::lib::Loader::getPath()[ 0 ].string() );
    CPPUNIT_ASSERT_EQUAL( std::string( "rcssbase/conf" ),
						  rcss::lib::Loader::getPath()[ 1 ].string() );
}

void TestParser::TestInclude()
{
    const char* argv[] = { "parsertest", "--include=rcssbase/conf/test.conf" };
    MyBuilder tester;
    MySubBuilder subtester( &tester );
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 2, (const char* const *)argv ) );
    int tmp = 0;
    CPPUNIT_ASSERT( tester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 2, tmp );

    CPPUNIT_ASSERT( subtester.get( "test", tmp ) );
    CPPUNIT_ASSERT_EQUAL( 3, tmp );
}

void TestParser::TestLoad()
{
    const char* argv[] = { "parsertest",
			   "--setpath=rcssbase/lib/loadertest",
			   "--load=libderived1",
			   "--help" };
    MyBuilder tester;
    rcss::conf::Parser parser( tester );
    CPPUNIT_ASSERT( parser.parse( 4, (const char* const *)argv ) );
    tester.displayHelp();
}
