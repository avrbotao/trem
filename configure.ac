#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(trem, 1.0.96, alexandre at botao dot org)
AC_MSG_NOTICE([trem 1.0.96 alexandre at botao dot org])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CANONICAL_HOST
OE_CFLAGS=""
AS_CASE([$build_os],
        [aix*],
        [
           OE_CFLAGS="$OE_CFLAGS -DAIX"
        ],
        [hpux*],
        [
           OE_CFLAGS="$OE_CFLAGS -DHPUX"
        ],
        [linux*],
        [
           OE_CFLAGS="$OE_CFLAGS -DLINUX"
        ],
        [cygwin*],
        [
           OE_CFLAGS="$OE_CFLAGS -DCYGWIN"
	],
        [darwin*],
        [
           OE_CFLAGS="$OE_CFLAGS -DOSX"
	],
        [mingw*],
        [
           OE_CFLAGS="$OE_CFLAGS -DMINGW"
	],
        [solaris*],
        [
           OE_CFLAGS="$OE_CFLAGS -DSOLARIS"
        ],
	[*],
	[
		AC_MSG_ERROR(["OS $build_os not supported"])
        ])
AS_CASE([$build_cpu],
	[hppa*],
	[
		OE_CFLAGS="$OE_CFLAGS -DHPPA"
	],
	[ia64],
	[
		OE_CFLAGS="$OE_CFLAGS -DIA64"
	],
	[i?86],
	[
		OE_CFLAGS="$OE_CFLAGS -DIX86"
	],
	[powerpc],
	[
		OE_CFLAGS="$OE_CFLAGS -DPOWERPC"
	],
	[sparc],
	[
		OE_CFLAGS="$OE_CFLAGS -DSPARC"
	],
	[x86_64],
	[
		OE_CFLAGS="$OE_CFLAGS -DAMD64"
	],
	[*],
	[
		AC_MSG_ERROR(["CPU $build_cpu not supported"])
	])
AC_SUBST([OE_CFLAGS])
AC_CONFIG_SRCDIR([include/absd.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_RANLIB
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB(crypt,crypt,[LIBS="$LIBS -lcrypt"])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_OFF_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([getcwd strdup strrchr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
