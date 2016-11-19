# @synopsis AX_INSTALL_FILES()
# ---------------------------------------------------------
# Author: Tom Howard <tomhoward@users.sf.net>
# Version: 1.2
# Copyright (C) 2004, 2005, Tom Howard
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without
# royalty provided the copyright notice and this notice
# are preserved.
#
# Desc: Adds a target to yopu Makefile for creating an install_files file,
#       which contains the list of files that will be installed.
#
# See Also: AX_MK_MACROS
#
AC_DEFUN([AX_INSTALL_FILES],
[
AC_MSG_NOTICE([adding install_files support])
AX_INSTALL_FILES_LIST="\$(top_builddir)/install_files"
AC_SUBST(AX_INSTALL_FILES_LIST)
AX_MFSTAMP="\$(top_builddir)/mfstamp"
AC_SUBST(AX_MFSTAMP)
AX_STAGING="\$(top_builddir)/staging"
AC_SUBST(AX_STAGING)
AX_ADD_MK_MACRO([[

ifdef CLEANFILES
  CLEANFILES += $AX_INSTALL_FILES_LIST
else
  CLEANFILES = $AX_INSTALL_FILES_LIST
endif

clean_staging:
	@echo \"removing $AX_STAGING\"
	rm -rf \"$AX_STAGING\"

$AX_INSTALL_FILES_LIST: do-mfstamp-recursive
	@if test \"$AX_MFSTAMP\" -nt \"$AX_INSTALL_FILES_LIST\"; then \\
          \$(MAKE) clean_staging; \\
	  cd \$(top_builddir) && STAGING=\"\$(PWD)/staging\"; \\
	  mkdir -p \"\$\$STAGING\"; \\
	  \$(MAKE) DESTDIR=\"\$\$STAGING\" install; \\
	  cd \"\$\$STAGING\" && find . ! -type d -print > ../install_files; \\
	else \\
	    echo \"\\\`$AX_INSTALL_FILES_LIST\' is up to date.\"; \\
	fi

]])
    AX_ADD_RECURSIVE_MK_MACRO([do-mfstamp],[[
$AX_MFSTAMP:  do-mfstamp-recursive

do-mfstamp-mk do-mfstamp: Makefile.in
	@echo \"timestamp for all Makefile.in files\" > \"$AX_MFSTAMP\"
	@touch \"${AX_DOLLAR}@\" 

]])
])# AX_INSTALL_FILES
