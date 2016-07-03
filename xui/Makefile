#  NETMAJ-XUI -- X User Interface --  Copyright Notice
#
#      Copyright (c) 1995,1996 Todo Software All rights reserved.
#
#   You can redistribute it and/or modify this software under the terms
#  of the GNU General Public License version 2 as published by
#  the Free Software Foundation.
#
#   This software is provided 'as-is', without any express or implied
#  warranty.  In no event will the author be held liable for any damages
#  arising from the use of this software.
#

VERSION=2
REVISION=0
INTERNAL_REVISION=7

# C compiler; ANSI C compiler required 
CC=gcc
# Debugging option or Optimization Option
G=-g
# C Compiler options: Please compile ANSI C: -Xa or -ansi
CCFLAGS=-DSMALL -DXNETMAJ -DX11=1
# where X11
X11INC=/usr/X11R6/include
X11LIB=/usr/X11R6/lib
RANLIB=ranlib
INSTALL=install
LIBS=
CFLAGS=
PREFIX=/usr
BINDIR=$(PREFIX)/games
LIBDIR=$(PREFIX)/lib/games/netmaj
CP1=$(INSTALL) -s
CP2=$(INSTALL) -c

all:
	-(cd src/gt; make CC="$(CC)" G="$(G)" CCFLAGS="$(CCFLAGS)" X11INC="$(X11INC)" X11LIB="$(X11LIB)" RANLIB="$(RANLIB)")
	-(cd src/util; make CC="$(CC)" G="$(G)" CCFLAGS="$(CCFLAGS)" RANLIB="$(RANLIB)" )
	-(cd src/int; make CC="$(CC)" G="$(G)" CCFLAGS="$(CCFLAGS)"  RANLIB="$(RANLIB)" )
	-(cd src/xui; make CC="$(CC)" G="$(G)" CCFLAGS="$(CCFLAGS)" X11INC="$(X11INC)" X11LIB="$(X11LIB)" RANLIB="$(RANLIB)" LIBS="$(LIBS)" OS_CFLAGS="$(OS_CFLAGS)" BINDIR=$(BINDIR) LIBDIR=$(LIBDIR))

clean:
	-(cd src/gt; make -k clean)
	-(cd src/util; make -k clean)
	-(cd src/int; make -k clean)
	-(cd src/xui; make -k clean)
	-rm -f *~ doc/*~ pf/.*~ pf/.*.pf~ *.core core .*~ *.bak

install: all
	$(CP1) src/xui/xclient ${DESTDIR}$(LIBDIR)
	$(CP1) src/xui/xnetmaj1 ${DESTDIR}$(LIBDIR)
	$(CP1) src/xui/xplview ${DESTDIR}$(LIBDIR)

install-pf:
	-if test -d ${DESTDIR}$(LIBDIR); then \
		$(CP2) ./misc/pf/.*.pf ${DESTDIR}$(LIBDIR); fi

