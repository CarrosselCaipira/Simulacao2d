AM_CXXFLAGS = -W -Wall -I$(top_srcdir)

CLEANFILES = \
*~ \
core

EXTRA_DIST = ac_lib_rcssconfparser.m4 \
SConscript

lib_LTLIBRARIES = librcssconfparser.la

librcssconfparser_la_SOURCES = \
	parser.cpp \
	builder.cpp \
	statushandler.cpp \
	streamstatushandler.cpp

librcssconfparser_la_LDFLAGS = -version-info 2:0:0
librcssconfparser_la_LIBADD = ../lib/librcsslib.la $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)

../lib/librcsslib.la:
	cd ../lib && $(MAKE) $(AM_MAKEFLAGS)

confdir = $(pkgincludedir)/conf

conf_HEADERS = \
	builder.hpp \
	parser.hpp \
	paramgetter.hpp \
	paramsetter.hpp \
	statushandler.hpp \
	streamstatushandler.hpp

if BUILD_TEST

check_PROGRAMS = buildertest parsertest

buildertest_SOURCES = buildertest.cpp
buildertest_LDADD = @TEST_LIBS@ librcssconfparser.la $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)

parsertest_SOURCES = parsertest.cpp
parsertest_LDADD = @TEST_LIBS@ librcssconfparser.la  $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)

TESTS = buildertest parsertest

endif

pkgdata_DATA = ac_lib_rcssconfparser.m4


#@INCLUDE_MK@
