# -*- Mode: Fundamental -*- 
# This file is part of xnetmaj
# $Id: Makefile,v 1.1 1996/06/21 04:21:27 suz Exp $
# Last Modified By: NBC02365@niftyserve.or.jp
# Last Modified On: Wed Sep 27 00:52:46 1995
CC=cc
CCFLAGS=-ansi
G=-g 
RANLIB=ranlib
X11INC=/usr/X11R6/include

SRC = 	pixcont.c core.c 	_3d.c	conf.c \
	item.c	\
	menu.c	\
	button.c frame.c	 \
        widget.c toplevel.c pixmapfr.c \
	text.c \
	pixmapco.c widtype.c bind.c \
	widsfx.c canvas.c \
	radio.c \
	convini.c \
	pixbox.c pixmap4.c \
	x11evth.c x11itemt.c

OBJ=$(SRC:.c=.o)
CFLAGS=-I$(X11INC) -I.. -I../include -I../.. \
       -I../../.. -I../dev -DX11 $(G) $(CCFLAGS) 

all: libwid.a

libwid.a: $(OBJ)
	- /bin/rm libwid.a
	ar rc libwid.a $(OBJ) 
	$(RANLIB) libwid.a

$(OBJ):

clean:
	/bin/rm -f *.o *.core *.a *~ 4/*.o 4/*.a 4/*~ 2> /dev/null

_3d.o: _3d.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h
bind.o: bind.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h
button.o: button.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/button.h
canvas.o: canvas.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/canvas.h ../../gt/widget/frame.h
conf.o: conf.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h 
convini.o: convini.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h
core.o: core.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/core.h
frame.o: frame.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/frame.h 
item.o: item.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/core.h ../../gt/widget/button.h \
  ../../gt/widget/frame.h ../../gt/widget/pixmapfr.h ../../gt/widget/menu.h \
  ../../gt/widget/toplevel.h ../../gt/widget/text.h \
  ../../gt/widget/pixmapco.h ../../gt/widget/radio.h 
menu.o: menu.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/menu.h
pixmapco.o: pixmapco.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/pixmapco.h ../../gt/widget/frame.h
pixmapfr.o: pixmapfr.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/pixmapfr.h
radio.o: radio.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/radio.h
text.o: text.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/text.h
toplevel.o: toplevel.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h ../../gt/widget/toplevel.h ../../gt/widget/frame.h
widget.o: widget.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h
widsfx.o: widsfx.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h
widtype.o: widtype.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h
x11evth.o: x11evth.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h
x11itemt.o: x11itemt.c ../../gt/widget/widget.h ../../gt/include/deftype.h \
  ../../gt/dev/gdev.h ../../gt/dev/x11gdev.h  \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../gt/widget/itemtype.h  \
  ../../gt/widget/resource.h ../../gt/widget/event.h \
  ../../gt/widget/funcdecl.h ../../gt/widget/widgetde.h ../../util/util.h \
  ../../gt/widget/local.h
