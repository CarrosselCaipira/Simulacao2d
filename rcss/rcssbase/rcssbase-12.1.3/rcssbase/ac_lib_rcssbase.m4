# AC_LIB_RCSSBASE([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ---------------------------------------------------------
# Checks for the rcssbase library
AC_DEFUN([AC_LIB_RCSSBASE],
[AS_VAR_PUSHDEF([ac_lib_rcssbase], [ac_cv_lib_rcssbase])dnl
AC_CACHE_CHECK(whether the rcssbase library is available, ac_cv_lib_rcssbase,
               [AC_LANG_PUSH(C++)
                OLD_LDFLAGS="$LDFLAGS"
                LDFLAGS="$LDFLAGS -lrcssbase"
                AC_LINK_IFELSE([@%:@include <rcssbase/version.hpp>
                                int main()
                                {
                                    rcss::base::version();
                                    return 0;
                                }],
                                [AS_VAR_SET(ac_lib_rcssbase, yes)], 
                                [AS_VAR_SET(ac_lib_rcssbase, no) 
                                 LDFLAGS="$OLD_LDFLAGS"
                                ])
                AC_LANG_POP(C++)
                ])
AS_IF([test AS_VAR_GET(ac_lib_rcssbase) = yes], [$1], [$2])
AS_VAR_POPDEF([ac_lib_rcssbase])dnl
])# AC_LIB_RCSSBASE
