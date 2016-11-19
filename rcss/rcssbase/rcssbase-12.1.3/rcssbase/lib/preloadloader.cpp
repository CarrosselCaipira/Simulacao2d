// -*-c++-*-

/***************************************************************************
                              preloadloader.cpp
                             -------------------
                        Does fake dynamic loading for static libraries
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


#include "preloadloader.hpp"
#include "preloader.hpp"
#include <string>

#include <boost/filesystem/operations.hpp>

namespace rcss {
namespace lib {

PreloadLoader::PreloadLoader( const boost::filesystem::path & lib,
                              AutoExt auto_ext,
                              const std::vector< boost::filesystem::path > & path )
    : LoaderImpl( lib, auto_ext, path )
{
    load();
}

PreloadLoader::~PreloadLoader()
{
    close();
}

void
PreloadLoader::doLoad()
{
    for ( Preloader::Holder::iterator i = Preloader::preloads().begin();
          i != Preloader::preloads().end();
          ++i )
    {
				if ( strippedName() == (*i)->name() )
        {
            M_handle = *i;
            return;
        }
    }
    error( NOT_FOUND );
}

void
PreloadLoader::doClose()
{

}

PreloadLoader::Initialize
PreloadLoader::doGetInitialize() const
{
    return M_handle->init();
}

PreloadLoader::Finalize
PreloadLoader::doGetFinalize() const
{
    return M_handle->fin();
}

std::vector< boost::filesystem::path >
PreloadLoader::listAvailableModules()
{
    std::vector< boost::filesystem::path > rval;
    for ( Preloader::Holder::iterator i = Preloader::preloads().begin();
          i != Preloader::preloads().end();
          ++i )
    {
				rval.push_back( (*i)->name() );
    }
    return rval;
}

PreloadLoader::Ptr
PreloadLoader::create( const boost::filesystem::path & lib,
                       AutoExt auto_ext,
                       const std::vector< boost::filesystem::path > & path )
{
    return Ptr( new PreloadLoader( lib, auto_ext, path ) );
}


PreloadLoaderStatic::PreloadLoaderStatic()
{

}

PreloadLoaderStatic::~PreloadLoaderStatic()
{

}

PreloadLoaderStatic::Ptr
PreloadLoaderStatic::create()
{
    return Ptr( new PreloadLoaderStatic() );
}

}
}
