# AC_LIB_RCSSCONFPARSER([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ---------------------------------------------------------
# Checks for the rcssconfparser library
AC_DEFUN([AC_LIB_RCSSCONFPARSER],
[AS_VAR_PUSHDEF([ac_lib_rcssconfparser], [ac_cv_lib_rcssconfparser])dnl
AC_CACHE_CHECK(whether the rcssconfparser library is available, ac_cv_lib_rcssconfparser,
               [AC_LANG_PUSH(C++)
                OLD_LDFLAGS="$LDFLAGS"
                LDFLAGS="$LDFLAGS -lrcssconfparser"
                AC_LINK_IFELSE([@%:@include <rcssbase/conf/builder.hpp>
                                @%:@include <rcssbase/conf/parser.hpp>
				int main(int argc, char *argv[])
                                {
                                    rcss::conf::Builder builder( argv[ 0 ], "test" );
                                    rcss::conf::Parser parser( builder );
                                    return 0;
                                }],
                                [AS_VAR_SET(ac_lib_rcssconfparser, yes)], 
                                [AS_VAR_SET(ac_lib_rcssconfparser, no) 
                                 LDFLAGS="$OLD_LDFLAGS"
                                ])
                AC_LANG_POP(C++)
                ])
AS_IF([test AS_VAR_GET(ac_lib_rcssconfparser) = yes], [$1], [$2])
AS_VAR_POPDEF([ac_lib_rcssconfparser])dnl
])# AC_LIB_RCSSCONFPARSER
