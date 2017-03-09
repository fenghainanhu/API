/* -*-C-*- */
/*
    plConfig.h.in

    Maurice LeBrun
    IFS, University of Texas at Austin
    18-Jul-1994

    Contains macro definitions that determine miscellaneous PLplot library
    configuration defaults, such as macros for bin, font, lib, and tcl
    install directories, and various system dependencies.  On a Unix
    system, typically the configure script builds plConfig.h from
    plConfig.h.in.  Elsewhere, it's best to hand-configure a plConfig.h
    file and keep it with the system-specific files.
*/

#ifndef __PLCONFIG_H__
#define __PLCONFIG_H__

/* Define if on a POSIX.1 compliant system.  */
#undef _POSIX_SOURCE

/* Define HAVE_UNISTD_H if unistd.h is available. */
#undef HAVE_UNISTD_H

/* Define if you have vfork.h.  */
#undef HAVE_VFORK_H

/* Define to `int' if <sys/types.h> doesn't define.  */
#define pid_t int

/* Define to `char *' if <sys/types.h> doesn't define.  */
#undef caddr_t

/* Define as the return type of signal handlers (int or void).  */
#undef RETSIGTYPE

/* Define if you have the ANSI C header files.  */
#undef STDC_HEADERS

/* Define vfork as fork if vfork does not work.  */
#undef vfork

/* Define if popen is available.  */
#undef HAVE_POPEN

/* Define if we're using a debugging malloc */
#undef DEBUGGING_MALLOC

/* If you don't know what this is for, you shouldn't be using it */
#undef NOBRAINDEAD

/* Define if fgetpos/fsetpos is busted */
#undef USE_FSEEK

/* Define if [incr Tcl] is available */
#undef HAVE_ITCL

/* Define if [incr Tk] is available */
#undef HAVE_ITK

/* Define if you want PLplot's float type to be double */
#undef PL_DOUBLE

/* Install directories. */

#undef LIB_DIR
#undef BIN_DIR
#undef TCL_DIR

#endif	/* __PLCONFIG_H__ */
