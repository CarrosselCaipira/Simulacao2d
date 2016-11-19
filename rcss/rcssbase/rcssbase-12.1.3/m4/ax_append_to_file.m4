# @synopsis AX_APPEND_TO_FILE([FILE],[DATA])
# ---------------------------------------------------------
# @author Tom Howard <tomhoward@users.sf.net>
# @version $Id: ax_append_to_file.m4,v 1.1.1.1 2005/02/23 18:05:42 tomhoward Exp $
# @copyright 2005 Tom Howard
#
# Appends the specified data to the specified file
#
AC_DEFUN([AX_APPEND_TO_FILE],[
AC_REQUIRE([AX_FILE_ESCAPES])
printf "$2" >> "$1"
])
