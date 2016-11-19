# AC_LIB_RCSSNET([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ---------------------------------------------------------
# Checks for the rcssnet library
AC_DEFUN([AC_LIB_RCSSNET],
[AS_VAR_PUSHDEF([ac_lib_rcssnet], [ac_cv_lib_rcssnet])dnl
AC_CACHE_CHECK(whether the rcssnet library is available, ac_cv_lib_rcssnet,
               [AC_LANG_PUSH(C++)
                OLD_LDFLAGS="$LDFLAGS"
                LDFLAGS="$LDFLAGS -lrcssnet"
                AC_LINK_IFELSE([@%:@include <rcssbase/net/udpsocket.hpp>
                                int main()
                                {
                                    rcss::net::UDPSocket udps();
                                    return 0;
                                }],
                                [AS_VAR_SET(ac_lib_rcssnet, yes)], 
                                [AS_VAR_SET(ac_lib_rcssnet, no) 
                                 LDFLAGS="$OLD_LDFLAGS"
                                ])
                AC_LANG_POP(C++)
                ])
AS_IF([test AS_VAR_GET(ac_lib_rcssnet) = yes], [$1], [$2])
AS_VAR_POPDEF([ac_lib_rcssnet])dnl
])# AC_LIB_RCSSNET
