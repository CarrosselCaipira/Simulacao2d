// -*-c++-*-

/***************************************************************************
                               sharedvar.h
                 Class storing shared variable to multiple threads
                             -------------------
    begin                : 22-APR-2002
    copyright            : (C) 2002 by The RoboCup Soccer Server
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

#ifndef _SHAREDVAR_H_
#define _SHAREDVAR_H_

#include <rcssbase/cond.h>
#include <iostream>

namespace rcss {
namespace thread {

template< typename DATA_TYPE >
class SharedVar
    : public Cond {
public:
    typedef DATA_TYPE DataType;
private:
    DataType M_data_var;

    SharedVar( const SharedVar & ); // not used

    void operator=( const SharedVar & ); // not used
public:
    SharedVar( const DataType & data )
        : M_data_var( data )
      { }

    DataType & operator=( const DataType & d )
      {
          return M_data_var = d;
      }

    DataType get( const Lock & )
      {
          return M_data_var;
      }
};

}
}

#endif
