# -*- Mode: Fundamental -*- 
# This file is part of xnetmaj

CC=cc
CCFLAGS=-ansi
G=-g 
X11INC=/usr/X11R6/include

SRC = 	pixbox.c pixmap4.c
OBJ=$(SRC:.c=.o)
CFLAGS=-I$(X11INC) -I../.. -I../../include -I../../.. \
       -I../../../.. -I../../dev -DX11 $(G) $(CCFLAGS) 

all: libwid4.a

libwid4.a: $(OBJ)
	- /bin/rm libwid4.a
	ar rc libwid4.a $(OBJ) 
	$(RANLIB) libwid4.a

$(OBJ):

clean:
	-rm *.o *.core *.a *~ 2> /dev/null
