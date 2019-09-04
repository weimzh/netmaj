# -*- Mode: Fundamental -*- 
# 
# Makefile -- 
# 
# Copyright (C) 1995,1996 Todo Software All rights reserved.
#

CC=cc
CCFLAGS=-ansi
G=-g 
X11=/opt/X11
RANLIB=ranlib

OBJ=error.o xmalloc.o
CFLAGS=-I$(X11)/include -I.. -I. -I../.. -DX11 $(G) $(CCFLAGS)

all: libutil.a

libutil.a: $(OBJ)
	ar ru libutil.a $(OBJ)
	$(RANLIB) libutil.a

$(OBJ):


clean:
	-rm *.o *.core *.a *~ 2> /dev/null
