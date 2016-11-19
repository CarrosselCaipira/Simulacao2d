SUBDIRS = . factorytest loadertest

CLEANFILES = \
*~ \
core


pkgdata_DATA = ac_lib_rcsslib.m4
EXTRA_DIST = ac_lib_rcsslib.m4 SConscript


AM_CXXFLAGS = -W -I@srcdir@/src

INCLUDES= -I$(top_srcdir) @INCLTDL@

rcsslibdir = $(pkgincludedir)/lib

rcsslib_HEADERS = \
factory.hpp \
loader.hpp \
shared_ptr.hpp \
weak_ptr.hpp \
loaderimpl.hpp \
preloader.hpp

lib_LTLIBRARIES = librcsslib.la

librcsslib_la_SOURCES = \
factory.hpp \
loader.hpp \
loader.cpp \
loaderimpl.hpp \
loaderimpl.cpp \
preloader.hpp \
preloader.cpp \
preloadloader.cpp \
preloadloader.hpp \
systemloader.cpp \
systemloader.hpp

librcsslib_la_LDFLAGS = -version-info 1:0:1
#    1. Start with version information of `0:0:0' for each libtool library.
#
#    2. Update the version information only immediately before a public
#       release of your software.  More frequent updates are unnecessary,
#       and only guarantee that the current interface number gets larger
#       faster.
#
#    3. If the library source code has changed at all since the last
#       update, then increment REVISION (`C:R:A' becomes `C:r+1:A').
#
#    4. If any interfaces have been added, removed, or changed since the
#       last update, increment CURRENT, and set REVISION to 0.
#
#    5. If any interfaces have been added since the last public release,
#       then increment AGE.
#
#    6. If any interfaces have been removed since the last public release,
#       then set AGE to 0.

librcsslib_la_LIBADD = @LIBLTDL@

bin_PROGRAMS = rcsslibimporter rcssmodtest

rcsslibimporter_SOURCES = libimporter.cpp
rcsslibimporter_LDADD = $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)

rcssmodtest_SOURCES = rcssmodtest.cpp

rcssmodtest_LDADD = librcsslib.la $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)

#@INCLUDE_MK@
