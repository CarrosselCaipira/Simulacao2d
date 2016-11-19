CLEANFILES = \
*~ \
core

AM_CXXFLAGS = -Wall -I$(top_srcdir)

if BUILD_TEST

check_PROGRAMS = factorytest

factorytest_SOURCES = factorytest.cpp
factorytest_LDADD = @TEST_LIBS@ ../librcsslib.la

TESTS = factorytest

endif


#@INCLUDE_MK@
