# @synopsis AX_ADD_MK_MACRO([RULE])
# ---------------------------------------------------------
# Author: Tom Howard <tomhoward@users.sf.net>
# Version: 1.0
# Copyright (C) 2005, Tom Howard
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without
# royalty provided the copyright notice and this notice
# are preserved.
#
# Desc: Adds the specified rule to $INCLUDE_MK
#
# See Also: AX_MK_MACROS, AX_ADD_RECURSIVE_MK_MACRO
#
AC_DEFUN([AX_ADD_MK_MACRO],[
  AC_REQUIRE([AX_MK_MACROS])
  AX_APPEND_TO_FILE([$INCLUDE_MK],[$1])
])
