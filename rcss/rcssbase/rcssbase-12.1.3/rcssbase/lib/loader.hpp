// -*-c++-*-

/***************************************************************************
                                loader.hpp
                             -------------------
                         dynamically open a library
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

#ifndef RCSS_LIB_LOADER_HPP
#define RCSS_LIB_LOADER_HPP

#include "../rcssbaseconfig.hpp"

#include "loaderimpl.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <map>
#include <iostream>

namespace rcss {
namespace lib {

class WeakLoader;

class Loader {
public:
    enum AutoExt {
        NO_AUTO_EXT = LoaderImpl::NO_AUTO_EXT,
        AUTO_EXT = LoaderImpl::AUTO_EXT
    };

    enum Error {
        LIB_OK = LoaderImpl::LIB_OK,
        NOT_FOUND = LoaderImpl::NOT_FOUND,
        INIT_ERROR = LoaderImpl::INIT_ERROR,
        SYSTEM_ERROR = LoaderImpl::SYSTEM_ERROR
    };

    enum ForceRecalc {
        TRY_CACHE,
        FORCE_RECALC
    };

    typedef boost::shared_ptr< LoaderImpl > Impl;
    typedef boost::weak_ptr< LoaderImpl > WeakImpl;

private:
    static const char S_OK_ERR_STR[];
    static const char S_NOT_FOUND_ERR_STR[];
    static const char S_INIT_ERR_STR[];

    typedef std::map< std::string, WeakImpl > CacheMap;
    typedef std::vector< std::pair< Impl, WeakImpl > > DepMap;

public:
    RCSSBASE_API
    static
    void addPath( const boost::filesystem::path & path );

    RCSSBASE_API
    static
    void addPath( const std::vector< boost::filesystem::path > & path );

    RCSSBASE_API
    static
    void setPath( const boost::filesystem::path & path );

    RCSSBASE_API
    static
    void setPath( const std::vector< boost::filesystem::path > & path );

    RCSSBASE_API
    static
    void clearPath();

    RCSSBASE_API
    static
    const
    std::vector< boost::filesystem::path > & getPath();

    /// returns the list of available modules that can
    /// currently be loaded.  Getting the list can be time
    /// consuming, so we cache the list.  The list is
    /// invalidated whenever the path is changed or a library
    /// is loaded (because loading a library might make a new
    /// loader available).  A new list will be fetched
    /// whenever the cached value is invalidated or \param
    /// force is set to FORCE_RECALC
    RCSSBASE_API
    static
    const
    std::vector< boost::filesystem::path > & listAvailableModules( ForceRecalc force = TRY_CACHE );

    RCSSBASE_API
    static
    Impl loadFromCache( const std::string & lib );

    RCSSBASE_API
    static
    size_t libsLoaded();

    RCSSBASE_API
    static
    std::string strip( const boost::filesystem::path & filename );

    RCSSBASE_API
    static
    boost::filesystem::path stripDirName( const boost::filesystem::path & filename );

    RCSSBASE_API
    static
    std::string stripExt( const boost::filesystem::path & filename );


    RCSSBASE_API
    Loader();

    Loader( WeakLoader & loader );

    RCSSBASE_API
    bool open( const boost::filesystem::path & lib,
               AutoExt autoext = AUTO_EXT );

    RCSSBASE_API
    bool isOpen() const;

    RCSSBASE_API
    void close();

    RCSSBASE_API
    boost::filesystem::path name() const;

    RCSSBASE_API
    std::string strippedName() const;

    RCSSBASE_API
    Error error() const;

    RCSSBASE_API
    const char * errorStr() const;
private:

    static
    void addToCache( const boost::filesystem::path & lib_name,
                     const Impl & lib );

    static
    void addDep( const Impl & lib );

    friend class LoaderImpl;
    friend class StaticLoader;
    friend class WeakLoader;

private:
    static CacheMap S_cached_libs;
    static DepMap S_deps;

    Impl M_impl;
    Error M_error;
    std::string M_system_err_str;

    static std::vector< boost::filesystem::path > S_path;
    static std::vector< boost::filesystem::path > S_available;
    static bool S_available_valid;

};

class WeakLoader {
public:
    RCSSBASE_API
    WeakLoader( Loader& loader )
        : M_impl( loader.M_impl )
      { }

private:
    friend class Loader;
    Loader::WeakImpl M_impl;
};

}
}



#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
#  define RCSSLIBCLIENT_API __declspec(dllexport)
#else
#  define RCSSLIBCLIENT_API
#endif

#define RCSSLIB_INIT( modulename )                            \
    namespace { const char* RCSS_MODULE_NAME = #modulename; } \
    extern "C" RCSSLIBCLIENT_API                              \
    bool                                                      \
    modulename##_initialize()

#define RCSSLIB_FIN( modulename )               \
    extern "C" RCSSLIBCLIENT_API                \
    void                                        \
    modulename##_finalize()

#endif
