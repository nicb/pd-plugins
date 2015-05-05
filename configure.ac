#
# $Id: configure.ac 40 2006-03-23 08:52:44Z nicb $
#
# Copyright (C) 2006 Nicola Bernardini, Marco Braggion, Amalia De Goetzen
#                    <studenti-nerds@lists.sme-ccppd.org>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([nerdz-pd-plugins],
        [0.0.0],
        [studenti-nerds@lists.sme-ccppd.org],
        [nerdz-pd-plugins}])
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

AC_PROG_CC
AC_PROG_INSTALL
AC_AIX
AC_ISC_POSIX
AC_MINIX
AC_STDC_HEADERS
AC_PROG_RANLIB

AC_CONFIG_FILES([
   Makefile
   README
   doc/Makefile
   m4/Makefile
   plugins/Makefile
])

AC_OUTPUT
