CC=cc
CCFLAGS=-ansi
G=-g 
X11INC=/usr/X11R6/include
X11LIB=/usr/X11R6/lib
RANLIB=ranlib

OBJ=
CFLAGS=-I$(X11INC) -I.. -Iwidget -I. -Idev -DX11

all: libmytk.a

libmytk.a: $(OBJ)
	(cd widget; make CC="$(CC)" G="$G" CCFLAGS="$(CCFLAGS)" X11INC="$(X11INC)" X11LIB="$(X11LIB)"  RANLIB="$(RANLIB)")
	(cd dev; make CC="$(CC)" G="$G" CCFLAGS="$(CCFLAGS)"  X11INC="$(X11INC)" X11LIB="$(X11LIB)" RANLIB="$(RANLIB)" )
	-rm libmytk.a
	ar ru libmytk.a   ./widget/*.o ./dev/*.o 
	$(RANLIB) libmytk.a

clean:
	-rm *.a *~ 2> /dev/null
	(cd widget; make -k clean)
	(cd dev; make -k clean )
	-rm *.o *.core core *.a *~ *.core 2> /dev/null
