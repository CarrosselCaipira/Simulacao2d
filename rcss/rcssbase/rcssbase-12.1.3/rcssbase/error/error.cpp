/***************************************************************************
               error.cpp  -  Provides a function to return descriptive
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

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <windows.h>
#include <lmerr.h>
#else
#include <cerrno>
#include <cstring>
#endif

#include "error.hpp"

namespace rcss
{
	namespace error
	{
		std::string
		strerror( long err )
		{
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
			std::string rval = "could not determine error message";
			HMODULE hModule = NULL; // default to system source
			LPSTR MessageBuffer;
			DWORD dwBufferLength;

			DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_FROM_SYSTEM ;

		    //
			// If dwLastError is in the network range, 
			//  load the message source.
			//

			if( err >= NERR_BASE && err <= MAX_NERR)
			{
				hModule = LoadLibraryEx( TEXT("netmsg.dll"),
										 NULL,
										 LOAD_LIBRARY_AS_DATAFILE );

				if( hModule != NULL )
					dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
				else
					return rval;
			}

			//
			// Call FormatMessage() to allow for message 
			//  text to be acquired from the system 
			//  or from the supplied module handle.
			//

		
			if( dwBufferLength = FormatMessageA( dwFormatFlags,
												 hModule, // (NULL == system)
												 err,
												 MAKELANGID( LANG_NEUTRAL,
															 SUBLANG_DEFAULT), // default language
													 (LPSTR) &MessageBuffer,
												 0,
											 	 NULL ) )
			{
				rval = MessageBuffer; 
				LocalFree( MessageBuffer );
			}	

			//
			// If we loaded a message source, unload it.
			//
			if( hModule != NULL )
				FreeLibrary( hModule );
	
			return rval;
#else
			return std::strerror( err );
#endif
		}
	}
}
