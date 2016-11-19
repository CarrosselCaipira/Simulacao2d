lib_LTLIBRARIES = librcsserror.la

CLEANFILES = \
*~ \
core \
install_files

librcsserror_la_SOURCES = \
error.hpp \
error.cpp

AM_CXXFLAGS = -Wall -I$(top_srcdir)

librcsserror_la_LDFLAGS = -version-info 0:0:0

errordir = $(pkgincludedir)/error

error_HEADERS = \
error.hpp

EXTRA_DIST = ac_lib_rcsserror.m4 SConscript

pkgdata_DATA = ac_lib_rcsserror.m4


#@INCLUDE_MK@
