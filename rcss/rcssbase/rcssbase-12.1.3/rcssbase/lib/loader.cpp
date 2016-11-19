// -*-c++-*-

/***************************************************************************
                                loader.cpp
                             -------------------
                             dlopen a library
    begin                : 2002-10-10
    copyright            : (C) 2002-2005 by The RoboCup Soccer Simulator
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "loader.hpp"
#include <stdio.h>

#include <boost/filesystem/convenience.hpp>

namespace rcss {
namespace lib {

const char Loader::S_OK_ERR_STR[] = "no error";
const char Loader::S_NOT_FOUND_ERR_STR[] = "module not found";
const char Loader::S_INIT_ERR_STR[] = "module could not initialise";

Loader::CacheMap Loader::S_cached_libs;
Loader::DepMap Loader::S_deps;
std::vector< boost::filesystem::path > Loader::S_path;

std::vector< boost::filesystem::path > Loader::S_available;
bool Loader::S_available_valid = false;

void
Loader::addPath( const boost::filesystem::path & path )
{
    S_available_valid = false;
    S_path.push_back( path );
}

void
Loader::addPath( const std::vector< boost::filesystem::path > & path )
{
    S_available_valid = false;
    S_path.insert( S_path.end(), path.begin(), path.end() );
}

void
Loader::setPath( const boost::filesystem::path & path )
{
    S_available_valid = false;
    S_path.clear();
    S_path.push_back( path );
}

void
Loader::setPath( const std::vector< boost::filesystem::path > & path )
{
    S_available_valid = false;
    S_path = path;
}

void
Loader::clearPath()
{
    S_available_valid = false;
    S_path.clear();
}

const
std::vector< boost::filesystem::path > &
Loader::getPath()
{
    return S_path;
}

const
std::vector< boost::filesystem::path > &
Loader::listAvailableModules( ForceRecalc force )
{
    if ( S_available_valid && force == TRY_CACHE )
    {
				return S_available;
    }
    S_available.clear();

    std::list< LoaderStaticImpl::Factory::Index > sloaders = LoaderStaticImpl::factory().list();

    for ( std::list< LoaderStaticImpl::Factory::Index >::iterator iter = sloaders.begin();
          iter != sloaders.end();
          ++iter )
    {
        LoaderStaticImpl::Factory::Creator creator;
        if ( LoaderStaticImpl::factory().getCreator( creator, *iter ) )
        {
            LoaderStaticImpl::Ptr impl = creator();
            std::vector< boost::filesystem::path > curr = impl->listAvailableModules();
            S_available.insert( S_available.end(), curr.begin(), curr.end() );
				}
    }
    S_available_valid = true;
    return S_available;
}

Loader::Impl
Loader::loadFromCache( const std::string & lib )
{
    CacheMap::iterator i = S_cached_libs.find( lib );
    if ( i != S_cached_libs.end() )
		{
		    if ( i->second.expired() )
        {
            S_cached_libs.erase( i );
            return Impl();
        }
		    else
        {
            return i->second.lock();
        }
		}
    return Impl();
}

size_t
Loader::libsLoaded()
{
    return S_cached_libs.size();
}

std::string
Loader::strip( const boost::filesystem::path & filename )
{
    return stripExt( stripDirName( filename ) );
}

std::string
Loader::stripExt( const boost::filesystem::path & filename )
{
    std::string rval = boost::filesystem::basename( filename );
    return rval;
}


boost::filesystem::path
Loader::stripDirName( const boost::filesystem::path & filename )
{
    std::string rval = filename.leaf();
    return rval;
}

Loader::Loader()
    : M_error( LIB_OK )
{

}

Loader::Loader( WeakLoader & loader )
{
    if ( ! loader.M_impl.expired() )
    {
        M_impl = Impl( loader.M_impl );
    }
}

bool
Loader::open( const boost::filesystem::path & libname,
              AutoExt auto_ext )
{
    if ( libname.empty() )
    {
        return false;
    }

    // try loading from the cache
    M_impl = loadFromCache( strip( libname ) );
    if ( M_impl )
    {
        return true;
    }

    std::list< LoaderImpl::Factory::Index > loaders = LoaderImpl::factory().list();
    for ( std::list< LoaderImpl::Factory::Index >::iterator iter = loaders.begin();
          iter != loaders.end();
          ++iter )
    {
				LoaderImpl::Factory::Creator creator;
				if ( LoaderImpl::factory().getCreator( creator, *iter ) )
				{
            M_impl = creator( libname,
                              (LoaderImpl::AutoExt)auto_ext,
                              S_path );
            if ( M_impl->valid() )
            {
                addToCache( libname, M_impl );
                addDep( M_impl );
                // the library we just loaded may make a new
                // loader available, so we can no longer reley
                // on the currently cached value (if there is
                // one) of available modules.
                S_available_valid = false;
                return true;
            }
            else
            {
                M_error = static_cast< Error >( M_impl->error() );
                if ( M_error == SYSTEM_ERROR )
                {
                    M_system_err_str = M_impl->errorStr();
                }
                M_impl.reset();
            }
				}
    }
    return false;
}

bool
Loader::isOpen() const
{
    return M_impl.get() != NULL;
}

void
Loader::close()
{
    M_impl.reset();
}

void
Loader::addToCache( const boost::filesystem::path & lib_name,
                    const boost::shared_ptr< LoaderImpl > & lib )
{
    S_cached_libs[ strip( lib_name ) ] = lib;
}

void
Loader::addDep( const Loader::Impl & lib )
{
    if ( S_deps.empty() )
    {
				S_deps.push_back( std::make_pair( Impl(), lib ) );
    }
    else
    {
				Impl dep;
				do
				{
            dep = S_deps.back().second.lock();
            if ( ! dep ) S_deps.pop_back();
				}
        while( ! dep && ! S_deps.empty() );

				if ( S_deps.empty() )
        {
            S_deps.push_back( std::make_pair( Impl(), lib ) );
        }
				else
        {
            S_deps.push_back( std::make_pair( dep,
                                              WeakImpl( lib ) ) );
        }
    }
}

boost::filesystem::path
Loader::name() const
{
    return ( M_impl
             ? M_impl->name()
             : boost::filesystem::path() );
}

std::string
Loader::strippedName() const
{
    return ( M_impl
             ? M_impl->strippedName()
             : std::string() );
}

Loader::Error
Loader::error() const
{
    return M_error;
}

const char *
Loader::errorStr() const
{
    switch ( error() ) {
    case LIB_OK:
        return S_OK_ERR_STR;
    case NOT_FOUND:
        return S_NOT_FOUND_ERR_STR;
    case INIT_ERROR:
        return S_INIT_ERR_STR;
    case SYSTEM_ERROR:
        return M_system_err_str.c_str();
    }
    return "unknown error";
}

}
}
