# AC_LIB_RCSSLIB([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ---------------------------------------------------------
# Checks for the rcsslib library
AC_DEFUN([AC_LIB_RCSSLIB],
[AS_VAR_PUSHDEF([ac_lib_rcsslib], [ac_cv_lib_rcsslib])dnl
AC_CACHE_CHECK(whether the rcsslib library is available, ac_cv_lib_rcsslib,
               [AC_LANG_PUSH(C++)
                OLD_LDFLAGS="$LDFLAGS"
                LDFLAGS="$LDFLAGS -lrcsslib"
                AC_LINK_IFELSE([@%:@include <rcssbase/lib/loader.hpp>
                                int main()
                                {
                                    rcss::lib::Loader::libsLoaded();
                                    return 0;
                                }],
                                [AS_VAR_SET(ac_lib_rcsslib, yes)], 
                                [AS_VAR_SET(ac_lib_rcsslib, no) 
                                 LDFLAGS="$OLD_LDFLAGS"
                                ])
                AC_LANG_POP(C++)
                ])
AS_IF([test AS_VAR_GET(ac_lib_rcsslib) = yes], [$1], [$2])
AS_VAR_POPDEF([ac_lib_rcsslib])dnl
])# AC_LIB_RCSSLIB
