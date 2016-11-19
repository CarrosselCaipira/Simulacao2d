// -*-c++-*-

/***************************************************************************
                              preloader.cpp
                             -------------------
                        Used to preload libs
    begin                : 2003-Sep-01
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

#include "preloader.hpp"
#include "loader.hpp"

namespace rcss {
namespace lib {

Preloader::Preloader( const std::string & name,
                      Initialize init,
                      Finalize fin )
    : M_name( name )
    , M_init( init )
    , M_fin( fin )
{
    addLib( *this );
}

Preloader::~Preloader()
{
    remLib( *this );
}

const
std::string &
Preloader::name() const
{
    return M_name;
}

Preloader::Initialize
Preloader::init() const
{
    return M_init;
}

Preloader::Finalize
Preloader::fin() const
{
    return M_fin;
}


void
Preloader::addLib( const Preloader & preload )
{
    S_preloads.push_back( &preload );
}

void
Preloader::remLib( const Preloader & preload )
{
    Holder::iterator i = S_preloads.begin();
    while ( i != S_preloads.end() )
    {
        if ( *i == &preload )
        {
            i = S_preloads.erase( i );
        }
        else
        {
            ++i;
        }
    }
}

Preloader::Holder &
Preloader::preloads()
{
    return S_preloads;
}

Preloader::Holder Preloader::S_preloads;

}
}
