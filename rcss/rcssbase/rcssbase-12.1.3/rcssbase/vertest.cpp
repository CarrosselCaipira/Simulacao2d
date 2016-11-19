// -*-c++-*-

/***************************************************************************
                                vertest.cpp
                             -------------------
                   Test the rssbase version reporting 
    begin                : 2003-04-23
    copyright            : (C) 2003 by The RoboCup Soccer Simulator 
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


#include "rcssbase/version.hpp"
#include <cppunit/extensions/HelperMacros.h>
#include <string>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

class TestVersion : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TestVersion);
    CPPUNIT_TEST(testVersion);
    CPPUNIT_TEST_SUITE_END();
protected:
    //--------------------------------------------------------------------------
    /// XXX: Document what the test is for here.
    //--------------------------------------------------------------------------
    void testVersion();
	
public:
    TestVersion();
	
    ~TestVersion();
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestVersion );

TestVersion::TestVersion()
{}
	
TestVersion::~TestVersion()
{}

void
TestVersion::testVersion() 
{
    const char* ver = rcss::base::version();
    CPPUNIT_ASSERT_EQUAL( std::string( ver ), std::string( VERSION ) );
}

