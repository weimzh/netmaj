VERSION=2
REVISION=0
INTERNAL_REVISION=7

SHELL=/bin/sh
# Install Directory
PREFIX=/usr
LIBDIR=$(PREFIX)/lib/games/netmaj
BINDIR=$(PREFIX)/games
PERL=perl

STD_CFLAGS=-I. -Ilib -DNETMAJ_LIB=\"$(LIBDIR)\" 

# for BSD
#CC=gcc
#OS_CFLAGS=-O -g -DUSE_SGTTY -DUSE_FWOPEN -DUSE_TERMCAP -DHAVE_GETTIMEOFDAY
#RANLIB=ranlib
#LIBS=
#LDFLAGS=

# for GNU/Linux
CC=gcc
OS_CFLAGS=-O -DUSE_TERMIOS -DUSE_TERMCAP -DDEF_OPTARG -DHAVE_GETTIMEOFDAY
RANLIB=ranlib
LIBS=
LDFLAGS=

CFLAGS=$(STD_CFLAGS) $(OS_CFLAGS)

STR_DEF=pai_str.o 
OBJS_LIB=ss.o global.o mt.o rv.o hand.o ahand.o result.o log.o cvt.o pf.o \
		game.o analize.o ruledef.o xsleep.o $(STR_DEF)
OBJS_SERVER=server.o
OBJS_AUTO  =auto.o 

all: auto server server_kill netmaj z x
z: proxyserver proxyclient

libnetmaj.a: $(OBJS_LIB)
	ar ruc libnetmaj.a $(OBJS_LIB)
	$(RANLIB) libnetmaj.a

server_kill: server_kill.o ss.o
	$(CC) -o server_kill server_kill.o ss.o $(LDFLAGS) $(LIBS)

server: $(OBJS_SERVER) libnetmaj.a
	$(CC) -o server $(OBJS_SERVER) libnetmaj.a $(LDFLAGS) $(LIBS)

auto: $(OBJS_AUTO) libnetmaj.a
	$(CC) -o auto $(OBJS_AUTO) libnetmaj.a $(LDFLAGS) $(LIBS)

proxyclient: proxyclient.o ss.o
	$(CC) -o proxyclient proxyclient.o ss.o $(LDFLAGS) $(LIBS)

proxyserver:  proxyserver.o ss.o
	$(CC) -o proxyserver proxyserver.o ss.o $(LDFLAGS) $(LIBS)

x:
	-if test -d xui; then \
	(cd xui; make CC="$(CC)" OS_CFLAGS="$(OS_CFLAGS)" \
	RANLIB="$(RANLIB)" LIBS="$(LIBS)" LIBDIR="$(LIBDIR) \
	BINDIR="$(BINDIR)" \
	"); fi

install: all
	-if test ! -d ${DESTDIR}${BINDIR}; then \
		mkdir -p ${DESTDIR}${BINDIR}; fi
	-if test ! -d ${DESTDIR}${LIBDIR}; then \
		mkdir -p ${DESTDIR}${LIBDIR}; fi
	-./server_kill
	cp netmaj ${DESTDIR}$(BINDIR)
	cp server auto server_kill ${DESTDIR}$(LIBDIR)
	cp proxyserver proxyclient ${DESTDIR}$(LIBDIR)
	-if test -d xui; then \
	(cd xui; make install CC="$(CC)" OS_CFLAGS="$(OS_CFLAGS)" \
	RANLIB="$(RANLIB)" LIBS="$(LIBS)" LIBDIR="$(LIBDIR)" \
	BINDIR="$(BINDIR)" \
	DESTDIR="$(DESTDIR)" \
	); fi
	cp pf/*.pf ${DESTDIR}$(LIBDIR)
	-if test -d xui; then \
	(cd xui; make install-pf CC="$(CC)" OS_CFLAGS="$(OS_CFLAGS)" \
	RANLIB="$(RANLIB)" LIBS="$(LIBS)" LIBDIR="$(LIBDIR)" \
	BINDIR="$(BINDIR)" \
	DESTDIR="$(DESTDIR)" \
	); fi

netmaj: netmaj.in
	sed -e "s|LIBDIR|$(LIBDIR)|" < netmaj.in | \
	sed -e "s|BINDIR|$(BINDIR)|" > netmaj
	chmod 755 netmaj

clean:
	rm -f *.o core* a.out server auto server_kill \
		netmaj player_info libnetmaj.a \
		proxyserver proxyclient
	-if test -d xui; then \
	(cd xui; make clean); fi

distclean: clean

global.o : global.c pai.h global.h comm.h ss.h
mt.o : mt.c pai.h global.h comm.h ss.h
rv.o : rv.c pai.h global.h ss.h
hand.o : hand.c pai.h global.h ss.h
ahand.o : ahand.c pai.h ahand.h global.h ss.h
result.o : result.c pai.h global.h ruledef.h ss.h
log.o : log.c pai.h global.h ss.h
cvt.o : cvt.c pai.h global.h ss.h
pf.o : pf.c pai.h global.h ss.h
game.o : game.c pai.h global.h ruledef.h ss.h
analize.o : analize.c pai.h ahand.h global.h ss.h
pai_str.o : pai_str.c ss.h
server.o : server.c pai.h global.h ss.h
auto.o : auto.c pai.h global.h ss.h
ruledef.o : ruledef.h pai.h global.h ss.h
xsleep.o : xsleep.c global.h ss.h
proxyclient.o : proxyclient.c pai.h global.h comm.h ss.h
proxyserver.o : proxyserver.c pai.h global.h comm.h ss.h
ss.o : ss.c ss.h
