# @synopsis AX_MK_MACROS()
# ---------------------------------------------------------
# Author: Tom Howard <tomhoward@users.sf.net>
# Version: 1.1
# Copyright (C) 2005, Tom Howard
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without
# royalty provided the copyright notice and this notice
# are preserved.
#
# Desc: Adds support for macros that create make targets.  You must append
#       your Makefile.in files with the file at $INCLUDE_MK.  The best way
#       to achive this is by adding @INCLUDE_MK@ at the end of your
#       Makefile.am or Makefile.in
#	If you are using automake, then you must also add @INCLUDE_MK_FILE@
#	to you DISTCLEANFILES
#
# See also: AX_ADD_MK_MACRO, AX_ADD_RECURSIVE_MK_MACRO
#
# ChangeLog:
#
# 2005-06-29	Tom Howard	<tomhoward@users.sf.net>
#
#	* Added INCLUDE_MK_FILE which is AC_SUBST'ed and updated Desc
#
AC_DEFUN([AX_MK_MACROS],
[
AC_MSG_NOTICE([adding make macro support])
INCLUDE_MK_FILE="$PWD/include.mk"
INCLUDE_MK="$PWD/include.mk"
AC_MSG_NOTICE([creating $INCLUDE_MK])
INCLUDE_MK_TIME=`date`
AX_PRINT_TO_FILE([$INCLUDE_MK],[[
# generated automatically by configure from AX_MK_MACROS
# on $INCLUDE_MK_TIME 

ifdef DISTCLEANFILES
   DISTCLEANFILES += \$(top_builddir)/include.mk
else
   DISTCLEANFILES = \$(top_builddir)/include.mk
endif
  
]])
AC_SUBST_FILE(INCLUDE_MK)
AC_SUBST(INCLUDE_MK_FILE)
])
