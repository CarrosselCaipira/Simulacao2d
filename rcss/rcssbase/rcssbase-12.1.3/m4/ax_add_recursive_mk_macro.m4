# @synopsis AX_ADD_RECURSIVE_MK_MACRO([TARGET],[RULE])
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
# Desc: Adds the specified rule to $INCLUDE_MK along with a TARGET-recursive
#       rule that will call TARGET for the current directory and TARGET-mk
#       recursively for each subdirectory
#
# See Also: AX_MK_MACROS, AX_ADD_MK_MACRO
AC_DEFUN([AX_ADD_RECURSIVE_MK_MACRO],[
  AX_ADD_MK_MACRO([
$1-recursive:
	@set fnord ${AX_DOLLAR}${AX_DOLLAR}MAKEFLAGS; amf=${AX_DOLLAR}${AX_DOLLAR}2; \\
	dot_seen=no; \\
	list='${AX_DOLLAR}(SUBDIRS)'; for subdir in ${AX_DOLLAR}${AX_DOLLAR}list; do \\
	  echo \"Making $1 in ${AX_DOLLAR}${AX_DOLLAR}subdir\"; \\
	  if test \"${AX_DOLLAR}${AX_DOLLAR}subdir\" = \".\"; then \\
	    dot_seen=yes; \\
	    local_target=\"$1-mk\"; \\
	  else \\
	    local_target=\"$1\"; \\
	  fi; \\
	  (cd ${AX_DOLLAR}${AX_DOLLAR}subdir && ${AX_DOLLAR}(MAKE) ${AX_DOLLAR}${AX_DOLLAR}local_target) \\
	   || case \"${AX_DOLLAR}${AX_DOLLAR}amf\" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \\
	done; \\
	if test \"${AX_DOLLAR}${AX_DOLLAR}dot_seen\" = \"no\"; then \\
	  ${AX_DOLLAR}(MAKE) \"$1-mk\" || exit 1; \\
	fi; test -z \"${AX_DOLLAR}${AX_DOLLAR}fail\"

$2
])
])
