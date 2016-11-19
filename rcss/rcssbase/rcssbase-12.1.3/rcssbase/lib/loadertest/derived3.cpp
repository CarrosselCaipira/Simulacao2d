// -*-c++-*-

/***************************************************************************
                  derived3.cpp  - Part of the rcss::LibLoader testing program
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
#define LOADERTESTDERIVED3_API __declspec(dllexport)
#else
#define LOADERTESTDERIVED3_API
#endif

#include "base.hpp"
#include "../loader.hpp"
#include <iostream>

class Derived3
    : public Base
{
public:
	LOADERTESTDERIVED3_API
	virtual ~Derived3() {}
	LOADERTESTDERIVED3_API
    virtual int method() const { return 1; };

	LOADERTESTDERIVED3_API
    static
    void
    destroy( Derived3* c )
    {
		delete c; 
	}

	LOADERTESTDERIVED3_API
    static
    Ptr 
    create()
    {
		return Ptr( new Derived3, 
                    &destroy, 
                    rcss::lib::Loader::loadFromCache( "libderived3" ) );
    }
 

};


RCSSLIB_INIT( libderived3 )
{
    Base::factory().reg( &Derived3::create, "derived3" );
    return true;
}

RCSSLIB_FIN( libderived3 )
{
    Base::factory().dereg( "derived3" );
}

