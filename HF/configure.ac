#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(HF, 0.1, bug-report@soixantecircuits.fr)
AM_INIT_AUTOMAKE([-Wall -Werror foreign dist-bzip2])
AC_CONFIG_SRCDIR([HF/main.cpp])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

# Checks for libraries.


# Check system type
AC_CANONICAL_HOST
AC_DEFINE_UNQUOTED(R_PLATFORM, "${host}",
[Define this to be the canonical name (cpu-vendor-os) of your system.])
AC_DEFINE_UNQUOTED(R_CPU, "${host_cpu}",
[Define this to be the name of the CPU of your system.])
AC_DEFINE_UNQUOTED(R_VENDOR, "${host_vendor}",
[Define this to be the name of the vendor of your system.])
AC_DEFINE_UNQUOTED(R_OS, "${host_os}",
[Define this to be the name of the OS of your system.])

R_PLATFORM="${host}"
AC_SUBST(R_PLATFORM)
R_OS="${host_os}"
AC_SUBST(R_OS)


case "$host_os" in
*darwin* | *rhapsody* | *macosx*)
	AC_MSG_RESULT(Recognised OS $host_os)
	AC_DEFINE_UNQUOTED(OSX,1,[This is an OS X system])
#	LDFLAGS="-framework AGL -framework coreaudio -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib"
	OPENGL_LIBS="-lGL"
	AC_SUBST(OPENGL_LIBS)
	;;
*linux-gnu*)
	AC_MSG_RESULT(Recognised OS $host_os)
	;;
*)
	AC_MSG_RESULT(Unrecognised OS $host_os)
;;
esac





# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_HEADER_TIME
AC_TYPE_UINT8_T
AC_PROG_OBJC

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([memset pow])

#Generate outputs
AC_CONFIG_FILES([Makefile HF/Makefile])
AC_OUTPUT
