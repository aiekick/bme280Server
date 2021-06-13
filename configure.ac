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

PKG_PROG_PKG_CONFIG
AC_ARG_WITH([systemdsystemunitdir],
     [AS_HELP_STRING([--with-systemdsystemunitdir=DIR], [Directory for systemd service files])],,
     [with_systemdsystemunitdir=auto])
AS_IF([test "x$with_systemdsystemunitdir" = "xyes" -o "x$with_systemdsystemunitdir" = "xauto"], [
     def_systemdsystemunitdir=$($PKG_CONFIG --variable=systemdsystemunitdir systemd)

     AS_IF([test "x$def_systemdsystemunitdir" = "x"],
   [AS_IF([test "x$with_systemdsystemunitdir" = "xyes"],
    [AC_MSG_ERROR([systemd support requested but pkg-config unable to query systemd package])])
    with_systemdsystemunitdir=no],
   [with_systemdsystemunitdir="$def_systemdsystemunitdir"])])
AS_IF([test "x$with_systemdsystemunitdir" != "xno"],
      [AC_SUBST([systemdsystemunitdir], [$with_systemdsystemunitdir])])
AM_CONDITIONAL([HAVE_SYSTEMD], [test "x$with_systemdsystemunitdir" != "xno"])

SERVICE_FILE=bme280Server.service
SYSTEMD_DIR=`pkg-config systemd --variable=systemdsystemunitdir`

# Use default if SYSTEMD_DIR is empty
if test x"$SYSTEMD_DIR" = "x"; then
    SYSTEMD_DIR=/etc/systemd/system
fi

AC_CONFIG_COMMANDS_POST([cp "$SERVICE_FILE" "$SYSTEMD_DIR"])
AC_CONFIG_COMMANDS_POST([systemctl enable "$SYSTEMD_DIR/$SERVICE_FILE"])
AC_CONFIG_COMMANDS_POST([systemctl start "$SERVICE_FILE"])



