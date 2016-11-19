# @synopsis AX_PRINT_TO_FILE([FILE],[DATA])
# ---------------------------------------------------------
# @author Tom Howard <tomhoward@users.sf.net>
# @version $Id: ax_print_to_file.m4,v 1.1.1.1 2005/02/23 18:05:48 tomhoward Exp $
# @copyright 2005 Tom Howard
#
# Writes the specified data to the specified file
#
AC_DEFUN([AX_PRINT_TO_FILE],[
AC_REQUIRE([AX_FILE_ESCAPES])
printf "$2" > "$1"
])
