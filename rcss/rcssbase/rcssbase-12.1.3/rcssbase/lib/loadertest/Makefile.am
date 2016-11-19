CLEANFILES = \
*~ \
core \
libimports.cpp

AM_CXXFLAGS = -Wall -I$(top_srcdir)

if BUILD_TEST

TESTS = runloadertest

check_SCRIPTS = runloadertest

runloadertest:	loadertest Makefile.am
	echo "#! /bin/sh" > runloadertest
	echo "cd .. && loadertest/loadertest" >> runloadertest
	chmod +x runloadertest

DYNAMIC_MODULES =
PRELOAD_MODULES =
if NODLOPEN
PRELOAD_MODULES += $(DYNAMIC_MODULES)
endif

check_PROGRAMS = loadertest
loadertest_SOURCES = loadertest.cpp
nodist_loadertest_SOURCES = libimports.cpp
BUILT_SOURCES = libimports.cpp
loadertest_LDADD = @TEST_LIBS@ ../librcsslib.la libbase.la $(PRELOAD_MODULES)

loadertest_DEPENDENCIES = $(PRELOAD_MODULES) ../rcsslibimporter
libimports.cpp:	Makefile
	../rcsslibimporter $(PRELOAD_MODULES) > $@

../rcsslibimporter:
	cd .. && $(MAKE) $(AM_MAKEFLAGS) rcsslibimporter

pkglib_LTLIBRARIES =  \
libbase.la \
libderived1.la

check_LTLIBRARIES = \
libderived3.la

libbase_la_SOURCES = \
base.cpp \
base.hpp

#install-exec-hook:
#	for lib in $(pkglib_LTLIBRARIES); do \
#		target=`echo $$lib | sed 's,\.la$$,,'`; \
#		echo $$target; \
#		rm -f $$DESTDIR$(pkglibdir)/$$target.*; \
#	done


libderived1_la_SOURCES = derived1.cpp
libderived1_la_LIBADD= libbase.la
libderived1_la_LDFLAGS = -module
DYNAMIC_MODULES += libderived1.la

libderived3_la_SOURCES = derived3.cpp
libderived3_la_LIBADD= libbase.la
PRELOAD_MODULES += libderived3.a

endif


#@INCLUDE_MK@
