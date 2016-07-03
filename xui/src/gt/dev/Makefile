CC=cc
CCFLAGS=-ansi
G=-g 
RANLIB=ranlib
X11INC=/usr/X11R6/include
OBJ=x11gdev.o x11rend.o x11sfx.o x11gc.o x11jpstr.o
CFLAGS=-I$(X11INC) -I.. -I../.. -I. -I../include -DX11 $(G) $(CCFLAGS)

all: byteord libdev.a

byteord:
	$(CC) -o byteord byteord.c 
	./byteord > byteord.h

libdev.a: $(OBJ)
	-rm libdev.a
	ar ru libdev.a $(OBJ)
	$(RANLIB) libdev.a

$(OBJ):


clean:
	-rm *.o *.core core *.a *~ byteord byteord.h 2> /dev/null

byteord.o: byteord.c
x11gc.o: x11gc.c ../../gt/dev/x11gdev.h ../../gt/include/deftype.h \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../util/util.h
x11gdev.o: x11gdev.c   \
   ../../gt/dev/x11gdev.h ../../gt/include/deftype.h \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../util/util.h ../../gt/dev/x11font.h
x11jpstr.o: x11jpstr.c   \
    byteord.h \
  ../../gt/dev/x11gdev.h ../../gt/include/deftype.h \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h
x11rend.o: x11rend.c ../../gt/dev/x11gdev.h ../../gt/include/deftype.h \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h
x11sfx.o: x11sfx.c ../../gt/dev/x11gdev.h ../../gt/include/deftype.h \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h \
  ../../util/util.h
x11spr.o: x11spr.c ../../gt/dev/x11gdev.h ../../gt/include/deftype.h \
   ../../gt/dev/x11spr.h ../../gt/dev/x11gdevd.h
