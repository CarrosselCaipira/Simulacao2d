CLEANFILES = \
*~ \
core

AM_CXXFLAGS = -Wall -I$(top_srcdir)
AM_LDFLAGS = -L../

if BUILD_TEST

check_PROGRAMS = addrtest

addrtest_SOURCES = addrtest.cpp
addrtest_LDADD = @TEST_LIBS@ -lrcssnet

TESTS = addrtest

endif


#@INCLUDE_MK@
