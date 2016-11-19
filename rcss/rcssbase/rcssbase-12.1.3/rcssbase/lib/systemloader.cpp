// -*-c++-*-

/***************************************************************************
                              systemloader.cpp
                             -------------------
                           dynamically loads libraries
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

#include "systemloader.hpp"
#include <iostream>
#include <string>
#include <boost/weak_ptr.hpp>
#include "loader.hpp"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <boost/filesystem/convenience.hpp>

#include "../error/error.hpp"

using namespace rcss::lib;

#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
static
void
onModuleIteration( const boost::filesystem::path & lib,
                   std::vector< boost::filesystem::path > & data )
{
    if ( boost::filesystem::extension( lib ) != ".dll"  )
    {
        return;
    }

    std::string command = "rcssmodtest.exe -q \""
        + lib.native_file_string()
        + "\"";
    if ( system( command.c_str() ) )
    {
        return;
    }

    data.push_back( lib );
    return;
}
#else
static
int
onModuleIteration( const char * sfilename, lt_ptr data )
{
    boost::filesystem::path filename;
    try
    {
        filename = boost::filesystem::path( sfilename,
                                            boost::filesystem::native );
    }
    catch(...)
    {
        try
        {
            filename = boost::filesystem::path( sfilename );
        }
        catch(...)
        {
            return 0;
        }

    }

    // make sure the file is loadable
    std::string command = "rcssmodtest \"";
    command += filename.native_file_string();
    command += "\" > /dev/null 2>&1";

    if ( system( command.c_str() ) )
    {
        return 0;
    }

    std::vector< boost::filesystem::path >& rval
        = *reinterpret_cast< std::vector< boost::filesystem::path >* >( data );
    rval.push_back( boost::filesystem::path( filename ) );
    return 0;
}
#endif

namespace rcss {
namespace lib {

class SystemLoaderInit {
public:
    ~SystemLoaderInit()
	    {
#if !defined(_WIN32) && !defined(__WIN32__) && !defined (WIN32)
          lt_dlexit();
#endif
	    }

    static
    SystemLoaderInit&
    instance()
	    {
          static SystemLoaderInit rval;
          return rval;
	    }


private:
    SystemLoaderInit()
	    {
#if !defined(_WIN32) && !defined(__WIN32__) && !defined (WIN32)
          lt_dlinit();
#endif
	    }

};

}
}

SystemLoader::SystemLoader( const boost::filesystem::path& lib,
                            AutoExt auto_ext,
                            const std::vector< boost::filesystem::path >& path )
    : LoaderImpl( lib, auto_ext, path ),
      M_init( SystemLoaderInit::instance() )
{
    load();
}


SystemLoader::~SystemLoader()
{
    close();
}

std::string
SystemLoader::setPath( const std::string& path )
{
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
    const char* old_path = getenv( "PATH" );
    if ( ! old_path )
        old_path = "";
    std::string rval = old_path;
    std::string tmp_new_path = "PATH=" + path;
    _putenv( tmp_new_path.c_str() );
    return rval;
#else // not windows
    const char* old_path = lt_dlgetsearchpath();
    if( ! old_path )
        old_path = "";

    std::string rval = old_path;
    lt_dlsetsearchpath( path.c_str() );
    return rval;
#endif
}


std::string
SystemLoader::setPath( const std::vector< boost::filesystem::path >& path )
{
    std::string tmp_new_path;
    for( std::vector< boost::filesystem::path >::const_iterator i = path.begin();
         i != path.end();
         ++i )
    {
        if ( i != path.begin() )
        {
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
            tmp_new_path += ';';
#else // not windows
            tmp_new_path += ':';
#endif
        }
        tmp_new_path += i->native_directory_string();
    }
    return setPath( tmp_new_path );
}

void
SystemLoader::doLoad()
{
    bool path_set = false;
    std::string old_path;
    if( ! Loader::getPath().empty() )
    {
        path_set = true;
        old_path = setPath( Loader::getPath() );
    }
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
    unsigned int old_error_mode = SetErrorMode( SEM_NOOPENFILEERRORBOX );
    if ( autoExt() )
    {
        boost::filesystem::path libname = name();
        std::string native_name = libname.native_file_string();
        if( boost::filesystem::extension( libname ) != ".dll" )
        {
            libname = libname.string() + ".dll";
        }
        m_handle = LoadLibrary( libname.native_file_string().c_str() );
    }
    else
    {
        m_handle = LoadLibrary( name().native_file_string().c_str() );
    }
    SetErrorMode( old_error_mode );
#else // not windows
    if ( autoExt() )
    {
        M_handle = lt_dlopenext( name().native_file_string().c_str() );
    }
    else
    {
        M_handle = lt_dlopen( name().native_file_string().c_str() );
    }
#endif
    if ( path_set )
    {
        setPath( old_path );
    }

    if ( ! M_handle )
    {
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
        error( rcss::error::strerror( GetLastError() ) );
#else
        const char* err = lt_dlerror();
        error( err );
#endif
    }
}

void
SystemLoader::doClose()
{
    //std::cerr << "lib::SystemLoader::doClose" << std::endl;
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
    FreeLibrary( M_handle );
#else
    lt_dlclose( M_handle );
#endif
}

SystemLoader::Initialize
SystemLoader::doGetInitialize() const
{
    std::string sym_name = strippedName() + "_initialize";
    //std::cerr << "lib::SystemLoader::doGetInitialize " << sym_name << std::endl;
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
    return (Initialize)GetProcAddress( M_handle, sym_name.c_str() );
#else
    return (Initialize)lt_dlsym( M_handle, sym_name.c_str() );
#endif
}

SystemLoader::Finalize
SystemLoader::doGetFinalize() const
{
    std::string sym_name = strippedName() + "_finalize";
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
    return (Finalize)GetProcAddress( M_handle, sym_name.c_str() );
#else
    return (Finalize)lt_dlsym( M_handle, sym_name.c_str() );
#endif
}


SystemLoader::Ptr
SystemLoader::create( const boost::filesystem::path & lib,
                      AutoExt auto_ext,
                      const std::vector< boost::filesystem::path > & path )
{
    return Ptr( new SystemLoader( lib, auto_ext, path ) );
}


std::vector< boost::filesystem::path >
SystemLoader::listAvailableModules()
{
    std::vector< boost::filesystem::path > rval;
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
    for ( std::vector< boost::filesystem::path >::const_iterator i = Loader::getPath().begin();
          i != Loader::getPath().end();
          ++i )
    {
        if ( boost::filesystem::exists( *i )
             &&  boost::filesystem::is_directory( *i ) )
        {
            for ( boost::filesystem::directory_iterator j( *i );
                  j != boost::filesystem::directory_iterator();
                  ++j )
            {
                onModuleIteration( *j, rval );
            }
        }
    }
#else
    std::string old_path = setPath( Loader::getPath() );
    //int result =
    lt_dlforeachfile( NULL,
                      &onModuleIteration,
                      reinterpret_cast< lt_ptr >( &rval ) );

    setPath( old_path );
#endif
    return rval;
}

SystemLoaderStatic::SystemLoaderStatic()
    : M_init( SystemLoaderInit::instance() )
{
}

SystemLoaderStatic::~SystemLoaderStatic()
{

}

SystemLoaderStatic::Ptr
SystemLoaderStatic::create()
{
    return Ptr( new SystemLoaderStatic() );
}
