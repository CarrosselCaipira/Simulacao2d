// -*-c++-*-

/***************************************************************************
                                version.cpp
                             -------------------
                   provides version information for rcssbase
    begin                : 2003-04-23
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "version.hpp"

namespace rcss {
namespace base {
const char *
version()
{
    return VERSION;
}
}
}
