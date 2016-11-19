SUBDIRS = error lib net conf gzip

CLEANFILES = \
*~ \
core

AM_CXXFLAGS = -W -I@srcdir@/src

INCLUDES= -I$(top_srcdir) @INCLTDL@

pkgdata_DATA = ac_lib_rcssbase.m4
EXTRA_DIST = ac_lib_rcssbase.m4 SConscript testmain.cpp

pkginclude_HEADERS = \
parser.h \
thread.h \
mutex.h \
cond.h \
messagequeue.h \
sharedvar.h \
FlexLexer.h \
rcssbaseconfig.hpp \
version.hpp

lib_LTLIBRARIES = librcssbase.la

librcssbase_la_SOURCES = version.hpp version.cpp

librcssbase_la_LDFLAGS = -version-info 0:0:0
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

if BUILD_TEST

check_PROGRAMS = vertest
vertest_SOURCES = vertest.cpp
vertest_LDADD = @TEST_LIBS@ librcssbase.la

TESTS = vertest

endif

##@INCLUDE_MK@
