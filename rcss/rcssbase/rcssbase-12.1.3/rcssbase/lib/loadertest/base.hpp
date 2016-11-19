// -*-c++-*-

/***************************************************************************
                  base.hpp  - Part of the rcss::LibLoader testing program
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

#include "../factory.hpp"
#include "../shared_ptr.hpp"
#include "../loader.hpp"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(CYGWIN)
  #ifdef BASE_EXPORTS
    #define BASE_API __declspec(dllexport)
    #define BASE_EXTERN
  #else
    #define BASE_API __declspec(dllimport)
    #define BASE_EXTERN extern
  #endif
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
  #define BASE_API
  #define BASE_EXTERN extern
#else
  #define BASE_API
  #define BASE_EXTERN
#endif


class Base
{
public:
    typedef rcss::lib::shared_ptr< Base > Ptr;
    typedef Ptr(*Creator)();
    typedef rcss::lib::Factory< Creator > Factory;
	
    BASE_API
	virtual ~Base() {}

    BASE_API
	virtual int method() const = 0;

    BASE_API
	static
    Factory&
    factory();
};

