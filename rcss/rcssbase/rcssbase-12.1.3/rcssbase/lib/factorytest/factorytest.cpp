// -*-c++-*-

/***************************************************************************
                  factorytest.cpp  - A rcss::Factory testing program
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


class TestFactory : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestFactory);
    CPPUNIT_TEST(emptyFactory);
    CPPUNIT_TEST(Registration);
    CPPUNIT_TEST(PersitantRegistration);
    CPPUNIT_TEST(DoubleRegistration);
    CPPUNIT_TEST(DoubleDeRegistration);
    CPPUNIT_TEST(DeRegistration);
    CPPUNIT_TEST(ReRegistration);
    CPPUNIT_TEST(GetCreator);
    CPPUNIT_TEST(GetCreatorFail);
    CPPUNIT_TEST(Create);
    CPPUNIT_TEST(Overwite);
    CPPUNIT_TEST(UnOverwite);
    CPPUNIT_TEST_SUITE_END();
protected:
    //--------------------------------------------------------------------------
    /// XXX: Document what the test is for here.
    //--------------------------------------------------------------------------

    void emptyFactory();
    void Registration();
    void PersitantRegistration();
    void DoubleRegistration();
    void DoubleDeRegistration();
    void DeRegistration();
    void ReRegistration();
    void GetCreator();
    void GetCreatorFail();
    void Create();
    void Overwite();
    void UnOverwite();

public:
    TestFactory()
	{}
	
    ~TestFactory()
	{}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestFactory );

#include "../factory.hpp"
#include <boost/shared_ptr.hpp>

using namespace rcss;

class Base
{
public:
    typedef boost::shared_ptr< Base > Ptr;
    typedef Ptr(*Creator)();
    typedef rcss::lib::Factory< Creator > Factory;


    virtual ~Base() {}

    virtual int method() const = 0;

    static
    Factory&
    factory()
    { return s_fact; }

private:
    static Factory s_fact;
};
 
Base::Factory Base::s_fact;


void
TestFactory::emptyFactory() 
{
  CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
}

class Derived
    : public Base
{
public:
    virtual int method() const { return 23; }

    static
    Ptr 
    create()
    {
        return Ptr( new Derived ); 
    }
};

class Derived2
    : public Base
{
public:
    virtual int method() const { return 230; }

    static
    Ptr 
    create()
    {
        return Ptr( new Derived2 ); 
    }
};


void
TestFactory::Registration()
{
    Base::factory().reg( &Derived::create, "derived" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size( "derived" ) );
}

void
TestFactory::PersitantRegistration()
{
  CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
  CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size( "derived" ) );
}

void
TestFactory::DoubleRegistration()
{
    Base::factory().reg( &Derived::create, "derived" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
    CPPUNIT_ASSERT_EQUAL( size_t( 2u ), Base::factory().size( "derived" ) );
}

void
TestFactory::DoubleDeRegistration()
{
    Base::factory().dereg( "derived" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size( "derived" ) );
}

void
TestFactory::DeRegistration()
{
    Base::factory().dereg( "derived" );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size() );
    CPPUNIT_ASSERT_EQUAL( size_t( 0u ), Base::factory().size( "derived" ) );
}

void
TestFactory::ReRegistration()
{
    Base::factory().reg( &Derived::create, "derived" );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size() );
    CPPUNIT_ASSERT_EQUAL( size_t( 1u ), Base::factory().size( "derived" ) );
}

void
TestFactory::GetCreator()
{
    Base::Creator c;
    CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived" ) );
    CPPUNIT_ASSERT( &Derived::create == c );
}

void
TestFactory::GetCreatorFail()
{
    Base::Creator c;
    CPPUNIT_ASSERT( !Base::factory().getCreator( c, "foobar" ) );
}

void
TestFactory::Create()
{
    Base::Creator c;
    CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived" ) );
    CPPUNIT_ASSERT_EQUAL( 23, c()->method() );
}

void
TestFactory::Overwite()
{
    Base::factory().reg( &Derived2::create, "derived" );
    Base::Creator c;
    CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived" ) );
    CPPUNIT_ASSERT_EQUAL( 230, c()->method() );
}

void
TestFactory::UnOverwite()
{
    Base::factory().dereg( "derived" );
    Base::Creator c;
    CPPUNIT_ASSERT( Base::factory().getCreator( c, "derived" ) );
    CPPUNIT_ASSERT_EQUAL( 23, c()->method() );
}

