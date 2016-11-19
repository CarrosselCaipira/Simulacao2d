// -*-c++-*-

/***************************************************************************
                                loaderimpl.hpp
                             -------------------
                base class for implementing a dynamically library loader
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


#ifndef RCSS_LIB_LOADERIMPL_HPP
#define RCSS_LIB_LOADERIMPL_HPP

#include "../rcssbaseconfig.hpp"

#include "factory.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <vector>

namespace rcss {
namespace lib {

class LoaderImpl {
public:
    enum AutoExt {
        NO_AUTO_EXT,
        AUTO_EXT
    };

    enum Error {
        LIB_OK, NOT_FOUND,
        INIT_ERROR,
        SYSTEM_ERROR
    };

    typedef boost::shared_ptr< LoaderImpl > Ptr;
    typedef Ptr (*Creator)( const boost::filesystem::path&,
                            AutoExt,
                            const std::vector< boost::filesystem::path >& path );
    typedef rcss::lib::Factory< Creator > Factory;

    RCSSBASE_API
    static
    Factory & factory();

    RCSSBASE_API
    virtual
    ~LoaderImpl();

    RCSSBASE_API
    const
    boost::filesystem::path & name() const
      {
          return M_name;
      }

    RCSSBASE_API
    std::string strippedName() const
      {
          return M_stripped_name;
      }

    RCSSBASE_API
    Error error() const;

    RCSSBASE_API
    std::string errorStr() const
			{
          return M_system_err_str;
      }

    RCSSBASE_API
    bool valid() const;

protected:
    typedef bool (*Initialize)();
    typedef void (*Finalize)();

    RCSSBASE_API
    LoaderImpl( const boost::filesystem::path &,
                AutoExt,
                const std::vector< boost::filesystem::path > & path );

    RCSSBASE_API
    void load();

    RCSSBASE_API
    void close();

    RCSSBASE_API
    bool autoExt()
      {
          return M_auto_ext == AUTO_EXT;
      }

    RCSSBASE_API
    void error( Error e );

    RCSSBASE_API
    void error( const std::string & err_str );

    RCSSBASE_API
    const
    std::vector< boost::filesystem::path > & getPath() const
			{
          return M_path;
			}
private:

    virtual
    void doLoad() = 0;

    virtual
    void doClose() = 0;

    virtual
    Initialize doGetInitialize() const = 0;

    virtual
    Finalize doGetFinalize() const = 0;

    void initialize();

    void finalize();

    friend class Loader;
private:
    boost::filesystem::path M_name;
    std::string M_stripped_name;
    AutoExt M_auto_ext;
    Error M_error;
    std::string M_system_err_str;
    std::vector< boost::filesystem::path > M_path;
};

/// Used to provide a static interface for the LoaderImpl subclasses
class LoaderStaticImpl {
public:
    typedef boost::shared_ptr< LoaderStaticImpl > Ptr;
    typedef Ptr (*Creator)();
    typedef rcss::lib::Factory< Creator > Factory;

    RCSSBASE_API
    static
    Factory & factory();

    RCSSBASE_API
    virtual
    ~LoaderStaticImpl();

    RCSSBASE_API
    virtual
    std::vector< boost::filesystem::path > listAvailableModules() = 0;

protected:
    RCSSBASE_API
    LoaderStaticImpl();
};

}
}

#endif
