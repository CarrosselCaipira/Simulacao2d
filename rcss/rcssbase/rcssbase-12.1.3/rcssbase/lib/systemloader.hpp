// -*-c++-*-

/***************************************************************************
                              systemloader.hpp
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


#ifndef RCSS_LIB_SYSTEM_LOADER_HPP
#define RCSS_LIB_SYSTEM_LOADER_HPP

#include "loaderimpl.hpp"

#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
#  include <windows.h>
#else
#  include <ltdl.h>
#endif

#include <boost/filesystem/path.hpp>

namespace rcss {
namespace lib {

class SystemLoaderInit;
class LibHandle;

class SystemLoader
    : public LoaderImpl {
public:
    SystemLoader( const boost::filesystem::path & lib,
                  AutoExt auto_ext,
                  const std::vector< boost::filesystem::path > & path );

    ~SystemLoader();

    static
    std::vector< boost::filesystem::path > listAvailableModules();

    static
    Ptr create( const boost::filesystem::path & lib,
                AutoExt auto_ext,
                const std::vector< boost::filesystem::path > & path );

private:
    static
    std::string setPath( const std::vector< boost::filesystem::path > & path );

    static
    std::string setPath( const std::string & path );

    void doLoad();

    void doClose();

    Initialize doGetInitialize() const;

    Finalize doGetFinalize() const;

private:
#if defined(_WIN32) || defined(__WIN32__) || defined (WIN32)
    HMODULE M_handle;
#else
    lt_dlhandle M_handle;
#endif
    friend class Loader;

    SystemLoaderInit & M_init;
};


class SystemLoaderStatic
    : public LoaderStaticImpl {
public:
    SystemLoaderStatic();

    ~SystemLoaderStatic();

    static
    Ptr create();

    /// we could just get the list of available modules
    /// without referencing system loader, but the
    /// LoaderStaticImpl concept is to provide access to the
    /// static interface of LoaderImpl subclasses.  By
    /// refefencing a static member function of SystemLoader,
    /// we adhear to this concept
    std::vector< boost::filesystem::path > listAvailableModules()
			{
          return SystemLoader::listAvailableModules();
			}

private:
    SystemLoaderInit & M_init;
};

}
}

#endif
