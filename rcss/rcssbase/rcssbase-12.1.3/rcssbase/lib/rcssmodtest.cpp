// -*-c++-*-

/***************************************************************************
                                rcssmodtest.cpp
                             -------------------
                   Tests is a file can be opened by the rcsslib library
    begin                : 2005-01-06
    copyright            : (C) 2005 by The RoboCup Soccer Simulator
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

#include "loader.hpp"

#include <iostream>
#include <cstdlib>

void
usage( int status = EXIT_FAILURE )
{
    std::ostream* out = &std::cerr;
    if( status == EXIT_SUCCESS )
    {
        std::cout << "exit_suc\n";
        out = &std::cout;
    }

    *out << "Usage:\n"
         << "\trcssmodtest [-h|--help] [-q|--quiet] <library>\n"
         << "\n"
         << "\trcssmodtest will check if the file specified is\n"
         << "\tloadable as a module.  The exit code is "
         << EXIT_SUCCESS << " on success\n"
         << "\tand " << EXIT_FAILURE << " otherwise.\n";
    exit( status );
}


int
main( int argc, char ** argv )
{
    bool verbose = true;
    for ( int i = 1; i < argc; ++i )
    {
        if( strcmp( argv[ i ], "-q" ) == 0
            || strcmp( argv[ i ], "--help" ) == 0 )
            verbose = false;
    }

    for ( int i = 1; i < argc; ++i )
    {
        if ( strcmp( argv[ i ], "-h" ) == 0
            || strcmp( argv[ i ], "--help" ) == 0 )
        {
            usage( EXIT_SUCCESS );
        }
        else if ( strcmp( argv[ i ], "-q" ) == 0
                  || strcmp( argv[ i ], "--quiet" ) == 0 )
        {
            // do nothing
        }
        else
        {
            rcss::lib::Loader loader;

            try
            {
                boost::filesystem::path path( argv[ i ],
                                              &boost::filesystem::native );
                if ( loader.open( path ) )
                {
                    exit( EXIT_SUCCESS );
                }
                else
                {
                    if ( verbose )
                    {
                        std::cerr << "error: could not load "
                                  << argv[ i ] << std::endl;
                        std::cerr << "error: " << loader.errorStr() << std::endl;
                    }
                    exit( EXIT_FAILURE );
                }
            }
            catch ( const std::exception & e )
            {
                if ( verbose )
                {
                    std::cerr << "error: could not load "
                              << argv[ i ] << std::endl;
                    std::cerr << "error: " << e.what() << std::endl;
                }
                exit( EXIT_FAILURE );
            }
        }
    }

    if ( verbose )
    {
        usage( EXIT_SUCCESS );
    }
}
