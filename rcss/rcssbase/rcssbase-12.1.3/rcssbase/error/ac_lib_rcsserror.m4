# AC_LIB_RCSSERROR([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ---------------------------------------------------------
# Checks for the rcsserror library
AC_DEFUN([AC_LIB_RCSSERROR],
[AS_VAR_PUSHDEF([ac_lib_rcsserror], [ac_cv_lib_rcsserror])dnl
AC_CACHE_CHECK(whether the rcsserror library is available, ac_cv_lib_rcsserror,
               [AC_LANG_PUSH(C++)
                OLD_LDFLAGS="$LDFLAGS"
                LDFLAGS="$LDFLAGS -lrcsserror"
                AC_LINK_IFELSE([@%:@include <rcssbase/error/errror.hpp>
                                int main()
                                {
                                    rcss::error::strerror( 0 );
                                    return 0;
                                }],
                                [AS_VAR_SET(ac_lib_rcsserror, yes)], 
                                [AS_VAR_SET(ac_lib_rcsserror, no) 
                                 LDFLAGS="$OLD_LDFLAGS"
                                ])
                AC_LANG_POP(C++)
                ])
AS_IF([test AS_VAR_GET(ac_lib_rcsserror) = yes], [$1], [$2])
AS_VAR_POPDEF([ac_lib_rcsserror])dnl
])# AC_LIB_RCSSERROR
