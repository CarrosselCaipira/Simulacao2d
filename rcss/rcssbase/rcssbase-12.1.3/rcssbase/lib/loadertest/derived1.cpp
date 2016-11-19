// -*-c++-*-

/***************************************************************************
                  derived1.cpp  - Part of the rcss::LibLoader testing program
                             -------------------
    begin                : 28-Aug-2003
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
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(CYGWIN)
#define LOADERTESTDERIVED1_API __declspec(dllexport)
#else
#define LOADERTESTDERIVED1_API
#endif

#include "base.hpp"
#include "../loader.hpp"
#include <iostream>

class Derived1
    : public Base
{
public:
	LOADERTESTDERIVED1_API 
    virtual ~Derived1() {}
	LOADERTESTDERIVED1_API 
    virtual int method() const { return 12345; };

	LOADERTESTDERIVED1_API 
    static
    void
    destroy( Derived1* c )
    {
		delete c; 
	}

	LOADERTESTDERIVED1_API 
    static
    Ptr 
    create()
    {
		return Ptr( new Derived1, 
                    &destroy, 
                    rcss::lib::Loader::loadFromCache( "libderived1" ) );
    }
 

};


RCSSLIB_INIT( libderived1 )
{
    Base::factory().reg( &Derived1::create, "derived1" );
    return true;
}

RCSSLIB_FIN( libderived1 )
{
    Base::factory().dereg( "derived1" );
}

