// -*-c++-*-

/***************************************************************************
                              preloader.hpp
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


#ifndef RCSS_LIB_PRELOADER_HPP
#define RCSS_LIB_PRELOADER_HPP

#include "../rcssbaseconfig.hpp"

#include <vector>
#include <string>

namespace rcss {
namespace lib {
class Preloader;
}
}

namespace rcss {
namespace lib {

class Preloader {
public:
    typedef bool (*Initialize)();
    typedef void (*Finalize)();

    RCSSBASE_API
    Preloader( const std::string& name,
               Initialize fin,
               Finalize init );

    RCSSBASE_API
    ~Preloader();

    RCSSBASE_API
    const
    std::string & name() const;

    RCSSBASE_API
    Initialize init() const;

    RCSSBASE_API
    Finalize fin() const;

private:
    typedef std::vector< const Preloader * > Holder;
    friend class PreloadLoader;

    static
    void addLib( const Preloader & pre );

    static
    void remLib( const Preloader & pre );

    static
    Holder & preloads();

    static Holder S_preloads;

    std::string M_name;
    Initialize M_init;
    Finalize M_fin;

};

}
}

#endif
