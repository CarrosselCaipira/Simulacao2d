// -*-c++-*-

/***************************************************************************
                  loadertest.cpp  - A rcss::lib::Loader testing program
                             -------------------
    begin                : 28-AUG-2003
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


class TestLoader : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestLoader);
    CPPUNIT_TEST(Path);
    CPPUNIT_TEST(LoadingDynamic);
    CPPUNIT_TEST(DoubleLoadDynamic); 
    CPPUNIT_TEST(LoaderStaysOpen);
    CPPUNIT_TEST(LoadingDynamicWithExt);
    CPPUNIT_TEST(LoadingDynamicWithExtAndDir);
    CPPUNIT_TEST(LoadPreloadedStatic);
    CPPUNIT_TEST(LoadPreloadedDynamic);
    CPPUNIT_TEST(LoadError);
    CPPUNIT_TEST_SUITE_END();
protected:
    //--------------------------------------------------------------------------
    /// XXX: Document what the test is for here.
    //--------------------------------------------------------------------------

    void Path();
    void LoadingDynamic();
    void DoubleLoadDynamic(); 
    void LoaderStaysOpen();
    void LoadingDynamicWithExt();
    void LoadingDynamicWithExtAndDir();
    void LoadPreloadedStatic();
    void LoadPreloadedDynamic();
    void LoadError();

public:
    TestLoader()
	{}
	
    ~TestLoader()
	{}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestLoader );

#include "base.hpp"

using namespace rcss;
using namespace rcss::lib;

void TestLoader::Path()
{
    CPPUNIT_ASSERT( Loader::getPath().empty() );
    Loader::setPath( "rcssbase/lib/loadertest" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::getPath().size() );
    CPPUNIT_ASSERT_EQUAL( std::string("rcssbase/lib/loadertest"),
						  Loader::getPath().front().string() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
    const std::vector< boost::filesystem::path >& available = Loader::listAvailableModules();
    for( std::vector< boost::filesystem::path >::const_iterator i = available.begin();
		 i != available.end(); ++i )
    {
		std::cout << "module: " << i->native_file_string() << std::endl;
    }
	CPPUNIT_ASSERT( 3u <= Loader::listAvailableModules().size() );
}



void TestLoader::LoadingDynamic() 
{
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    {
        rcss::lib::Loader loader;
        bool result = loader.open( "libderived1" );
        if( !result )
			std::cout << "loader error: " <<loader.errorStr() << std::endl;
        CPPUNIT_ASSERT( result );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
        Base::Creator c;
        CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived1" ) );
        CPPUNIT_ASSERT_EQUAL( 12345, c()->method() );
    }
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}



void TestLoader::DoubleLoadDynamic() 
{
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    {
        rcss::lib::Loader loader;
        CPPUNIT_ASSERT( loader.open( "libderived1" ) );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
        rcss::lib::Loader lib2;
        CPPUNIT_ASSERT( lib2.open( "libderived1" ) );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
        Base::Creator c;
        CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived1" ) );
        CPPUNIT_ASSERT_EQUAL( 12345, c()->method() );
    }
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}

void TestLoader::LoaderStaysOpen() 
{
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    Base::Ptr ptr;
    {
        rcss::lib::Loader loader;
        CPPUNIT_ASSERT( loader.open( "libderived1" ) );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
        Base::Creator c;
        CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived1" ) );
        ptr = c();
        CPPUNIT_ASSERT_EQUAL( 12345, ptr->method() );
    }
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( 12345, ptr->method() );
    ptr.reset();
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}


void TestLoader::LoadingDynamicWithExt() 
{
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    {
        rcss::lib::Loader loader;
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
        CPPUNIT_ASSERT( loader.open( "libderived1.dll" ) );
#else
        CPPUNIT_ASSERT( loader.open( "libderived1.so" ) );
#endif       
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
        Base::Creator c;
        CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived1" ) );
        CPPUNIT_ASSERT_EQUAL( 12345, c()->method() );
    }
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}

void TestLoader::LoadingDynamicWithExtAndDir() 
{
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    {
        rcss::lib::Loader loader;
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
        bool result = loader.open( "rcssbase/lib/loadertest/libderived1.dll" );
#else
        bool result = loader.open( "rcssbase/lib/loadertest/libderived1.so" );
#endif       
        if( !result )
			std::cout << "loader error: " <<loader.errorStr() << std::endl;
        CPPUNIT_ASSERT( result );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
        Base::Creator c;
        CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived1" ) );
        CPPUNIT_ASSERT_EQUAL( 12345, c()->method() );
    }
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}

void TestLoader::LoadPreloadedStatic() 
{
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    {
        rcss::lib::Loader loader;
		bool result = loader.open( "libderived2" );
		if( !result )
			std::cout << "loader error: " <<loader.errorStr() << std::endl;
        CPPUNIT_ASSERT( result );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
        Base::Creator c;
        CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived2" ) );
        CPPUNIT_ASSERT_EQUAL( 2, c()->method() );
    }
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}

void TestLoader::LoadPreloadedDynamic() 
{
	CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
	CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    {
        rcss::lib::Loader loader;
        CPPUNIT_ASSERT( loader.open( "libderived3" ) );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Loader::libsLoaded() );
        CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
        Base::Creator c;
        CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived3" ) );
        CPPUNIT_ASSERT_EQUAL( 1, c()->method() );
    }
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}

void TestLoader::LoadError() 
{
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    {
        rcss::lib::Loader loader;
        CPPUNIT_ASSERT( !loader.open( "libderived3xxx" ) );
    }
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Loader::libsLoaded() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}
