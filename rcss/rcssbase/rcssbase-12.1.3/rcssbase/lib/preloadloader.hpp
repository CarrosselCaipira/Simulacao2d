// -*-c++-*-

/***************************************************************************
                              preloadloader.hpp
                             -------------------
                            loads preloaded libs
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


#ifndef RCSS_LIB_PRELOAD_LOADER_HPP
#define RCSS_LIB_PRELOAD_LOADER_HPP

#include "loaderimpl.hpp"

namespace rcss {
namespace lib {

class Preloader;

class PreloadLoader
    : public LoaderImpl {
public:
    PreloadLoader( const boost::filesystem::path & lib,
                   AutoExt auto_ext,
                   const std::vector< boost::filesystem::path > & path );
    ~PreloadLoader();

    static
    std::vector< boost::filesystem::path > listAvailableModules();

    static
    Ptr create( const boost::filesystem::path& lib,
                AutoExt auto_ext,
                const std::vector< boost::filesystem::path >& path );

private:
    void doLoad();

    void doClose();

    Initialize doGetInitialize() const;

    Finalize doGetFinalize() const;

private:
    const Preloader * M_handle;
};


class PreloadLoaderStatic
    : public LoaderStaticImpl {
public:
    PreloadLoaderStatic();

    ~PreloadLoaderStatic();

    static
    Ptr create();

    /// we could just get the list of available modules
    /// without referencing system loader, but the
    /// LoaderStaticImpl concept is to provide access to the
    /// static interface of LoaderImpl subclasses.  By
    /// refefencing a static member function of PreloadLoader,
    /// we adhear to this concept
    std::vector< boost::filesystem::path > listAvailableModules()
			{
          return PreloadLoader::listAvailableModules();
			}
};
}
}


#endif
