/***************************************************************************
               error.hpp  -  Provides a function to return descriptive
							 strings from error codes
                             -------------------
    begin                : 14-AUG-2003
    copyright            : (C) 2003 by The RoboCup Soccer Server
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

#include <string>
#include "../rcssbaseconfig.hpp"

namespace rcss
{
    namespace error
    {
		RCSSBASE_API
        std::string
        strerror( long err );
    }
}
