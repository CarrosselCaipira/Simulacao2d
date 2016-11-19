// -*-c++-*-

/***************************************************************************
                                libimporter.cpp 
                             -------------------
                   Creates export files for rcsslib libraries
    begin                : 2002-11-06
    copyright            : (C) 2002 by The RoboCup Soccer Simulator 
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
#include <iostream>
#include <fstream>
#include <set>
#include <cstring>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

boost::filesystem::path
stripDirName( const boost::filesystem::path& filename )
{
	return filename.leaf();
}

std::string
stripExt( const boost::filesystem::path& filename )
{
    return boost::filesystem::basename( filename );
}

std::string
strip( const boost::filesystem::path& filename )
{ return stripExt( stripDirName( filename ) ); }

void
createExportFile( std::set< std::string > exports )
{
    std::cout << "#include <rcssbase/lib/preloader.hpp>\n";
    for( std::set< std::string >::iterator i = exports.begin();
         i != exports.end(); ++i )
    {
        std::cout << "\nextern \"C\"\n"
                  << "{\n"
                  << "    extern bool " << *i 
                  << "_initialize();\n"
                  << "    extern void " << *i
                  << "_finalize();\n"
                  << "}\n"
                  << "rcss::lib::Preloader " << *i << "_si( \"" 
				  << *i 
                  << "\", (bool(*)())&" 
				  << *i << "_initialize, (void(*)())&" 
				  << *i << "_finalize );\n";
    }
}

void
usage()
{
    std::cerr << "Usage:\n"
              << "\trcsslibimporter [-h|--help] library ...\n";
    exit( -1 );
}

int 
main( int argc, char ** argv )
{
    std::set< std::string > exports;
    for( int i = 1; i < argc; ++i )
    {
        if( strcmp( argv[ i ], "-h" ) == 0 || strcmp( argv[ i ], "--help" ) == 0 )
        {
            usage();
        }
        else
        {
			boost::filesystem::path path;
			try
			{
				path = boost::filesystem::path( argv[ i ],
												&boost::filesystem::native );
			}
			catch( const std::exception& e )
			{
				try
				{
					path = boost::filesystem::path( argv[ i ] );
				}
				catch( const std::exception& e )
				{
					std::cerr << "error: " << e.what() << std::endl; 
					continue;
				}
			}
            exports.insert( strip( path ) );
        }
    }

    createExportFile( exports );

    return 0;
}
