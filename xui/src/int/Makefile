SRC=readexpr.c expr.c interror.c intinit.c int.c name.c numalloc.c
RANLIB=ranlib

OBJ=$(SRC:.c=.o)

CFLAGS=$(G) $(CCFLAGS) -DPOSIX

all: libnu.a

libnu.a: $(OBJ)
	-rm libnu.a
	ar ru libnu.a $(OBJ)
	$(RANLIB) libnu.a

$(OBJ): 

clean:
	-rm -rf *.o *~ *.core core *.a a.out 2> /dev/null

expr.o: expr.c nu.h config.h autoconf.h os.h \
  def.h \
  misc.h var.h exprdef.h hashdef.h declare.h
int.o: int.c nu.h config.h autoconf.h os.h \
  def.h \
  misc.h var.h exprdef.h hashdef.h declare.h
interror.o: interror.c nu.h config.h autoconf.h os.h \
  def.h \
  misc.h var.h exprdef.h hashdef.h declare.h
intinit.o: intinit.c
name.o: name.c nu.h config.h autoconf.h os.h \
  def.h \
  misc.h var.h exprdef.h hashdef.h declare.h
numalloc.o: numalloc.c \
  nu.h \
  config.h autoconf.h os.h \
  def.h \
  misc.h var.h exprdef.h hashdef.h declare.h
readexpr.o: readexpr.c nu.h config.h autoconf.h os.h \
  def.h \
  misc.h var.h exprdef.h hashdef.h declare.h
