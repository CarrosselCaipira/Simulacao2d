// -*-c++-*-

/***************************************************************************
                  base.cpp  - Part of the rcss::LibLoader testing program
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

#include "base.hpp"

//Base::Factory Base::s_fact;

Base::Factory&
Base::factory()
{ static Factory fact; return fact; }

// #if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// template class LOADERTESTBASE_API std::allocator< Base::Factory::Creator >;
// template class LOADERTESTBASE_API std::vector< Base::Factory::Creator >;
// template class LOADERTESTBASE_API std::allocator< std::pair< Base::Factory::Index, std::vector< Base::Factory::Creator > > >;
// template class LOADERTESTBASE_API std::vector< std::pair< Base::Factory::Index, std::vector< Base::Factory::Creator > > >;
// template class LOADERTESTBASE_API rcss::lib::Factory< Base::Creator >;
// #endif
