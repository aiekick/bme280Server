AC_INIT([bme280Server], [0.1], [aiekick@funparadigm.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CXX
CFLAGS="$CFLAGS -I/opt/local/include -std=c11 -Wall -Wextra -Wno-unused-parameter -fpermissive"
CXXFLAGS="$CXXFLAGS -I/opt/local/include -std=c++11 -Wall -Wextra -Wno-unused-parameter -fpermissive"
LDFLAGS="$LDFLAGS -L/opt/local/lib"
AC_CHECK_LIB([uv], [uv_loop_init], [], [AC_MSG_ERROR([libuv1-dev is not found])])
AC_CHECK_LIB([sqlite3], [sqlite3_exec], [], [AC_MSG_ERROR([libsqlite3-dev is not found])])
AX_PTHREAD([
    LIBS="$LIBS $PTHREAD_LIBS"
    CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
])
AX_CXX_COMPILE_STDCXX_11(noext, mandatory)
AC_CONFIG_FILES([Makefile bme280Server.service])
AC_OUTPUT




