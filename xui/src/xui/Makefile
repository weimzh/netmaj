.SUFFIXES: .c .o 

PREFIX=/usr
CC=cc
CCFLAGS=-DSMALL -DXNETMAJ -DX11=1 -DNETMAJ_LIB=\"$(LIBDIR)\" 
G=-O -g
X11INC=/opt/X11/include
X11LIB=/opt/X11/lib
BINDIR=${PREFIX}/bin
LIBDIR=${PREFIX}/lib/games/netmaj
LIBS=

GT=../gt
UT=../util
INT=../int
NM=../../..
NMOBJS=client.o $(NM)/libnetmaj.a 
PLVOBJS=plview.o $(NM)/libnetmaj.a 
NM1OBJS=netmaj1.o $(NM)/libnetmaj.a 
NLIBS=$(GT)/libmytk.a $(UT)/libutil.a $(INT)/libnu.a

SRC=pixdec.c pixmisc.c pixread.c pixyaku.c pixbdig.c pixattr.c \
    res.c bg.c handler.c itime.c  attr.c msg.c auto.c \
    x11intin.c pfdir.c pfinfo.c getline.c bdcolor.c advice2.c advice.c \
    talkcmd.c lang.c flags.c rule.c \
    about.c hora.c playinfo.c \
    rank.c testpai.c title.c yaku.c \
    init.c pixpai2.c \
    xui2.c board2.c config.c

OBJ=$(SRC:.c=.o) 

.c.o : 
	$(CC) $(CFLAGS) $(G) -c $*.c

CFLAGS= $(XCFLAGS) $(CCFLAGS) $(G) $(OS_CFLAGS) \
	-I$(X11INC) -L$(X11LIB) -DX11 -I.. \
	-I../misc \
	-I$(NM) -I$(NM)/lib \
	-DNETMAJ_LIB=\"$(LIBDIR)\"

all: xclient xnetmaj1 xplview 

xclient: $(OBJ) $(NMOBJS) $(NLIBS)
	$(CC) $(G) -o xclient $(OBJ) $(NMOBJS) $(NLIBS) -L$(NM)/lib -L$(X11LIB) -lXpm -lX11 $(LIBS)

xnetmaj1: $(OBJ) $(NM1OBJS) $(NLIBS)
	$(CC) $(G) -o xnetmaj1 $(OBJ) $(NM1OBJS) $(NLIBS) -L$(NM)/lib -L$(X11LIB) -lXpm -lX11 $(LIBS)

xplview: $(OBJ) $(PLVOBJS) $(NLIBS)
	$(CC) $(G) -o xplview $(OBJ) $(PLVOBJS) $(NLIBS) -L$(NM)/lib -L$(X11LIB) -lXpm -lX11 $(LIBS)

clean:
	/bin/rm -f *.core *~ *.o *.a 2> /dev/null
	/bin/rm -f xclient xnetmaj1 xplview 2> /dev/null
	/bin/rm -f bitmap/misc/*~ bitmap/pai/*~ bitmap/pai2/*~ bitmap/pai3/*~ bitmap/yaku/*~ bitmap/attr/402/*~ bitmap/attr/252/*~ 2> /dev/null
	/bin/rm -f bitmap/pai/*~ bitmap/pai/*.core  bitmap/pai/a.out 2> /dev/null
	/bin/rm -f bitmap/pai2/*~ bitmap/pai2/*.core  bitmap/pai2/a.out 2> /dev/null
	/bin/rm -f bitmap/pai3/*~ bitmap/pai3/*.core  bitmap/pai3/a.out 2> /dev/null

about.o: about.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h

advice.o: advice.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    ../../..//ahand.h \
  bitmap/misc/adv.xpm
advice2.o: advice2.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    bitmap/attr/252/tiny0.xpm \
  bitmap/attr/252/tiny1.xpm bitmap/attr/252/tiny2.xpm \
  bitmap/attr/252/tiny3.xpm bitmap/attr/252/tiny4.xpm \
  bitmap/attr/252/tinyn.xpm bitmap/attr/252/tinys.xpm \
  bitmap/attr/252/tinyw.xpm bitmap/attr/252/tinye.xpm \
  bitmap/attr/252/tiny_sp.xpm
attr.o: attr.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
auto.o: auto.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
bdcolor.o: bdcolor.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
bg.o: bg.c

flags.o: flags.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
getline.o: getline.c   \
   
handler.o: handler.c ../misc/systemio.h ../gt/include/deftype.h \
  
hora.o: hora.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
init.o: init.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
itime.o: itime.c   \
    
lang.o: lang.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
msg.o: msg.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    bitmap/misc/tting.xpm \
  bitmap/misc/failconn.xpm bitmap/misc/connect.xpm bitmap/misc/disconne.xpm \
  bitmap/misc/msgtumo.xpm bitmap/misc/msgron.xpm bitmap/misc/msgreach.xpm
pfdir.o: pfdir.c   \
    ../misc/mydir.h \
   flags.h ../gt/widget/widget.h \
  ../gt/include/deftype.h ../gt/dev/gdev.h ../gt/dev/x11gdev.h \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h lang.h
pfinfo.o: pfinfo.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    pfinfo.h \
   
pixattr.o: pixattr.c pixread.h ../gt/widget/widget.h ../gt/include/deftype.h \
  ../gt/dev/gdev.h ../gt/dev/x11gdev.h  \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h geom.h pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h \
  bitmap/attr/252/attrreac.xpm bitmap/attr/252/attrron.xpm \
  bitmap/attr/252/attrkan.xpm bitmap/attr/252/attrpon.xpm \
  bitmap/attr/252/attrtie.xpm bitmap/attr/252/attrtg.xpm \
  bitmap/attr/252/attr4.xpm bitmap/attr/252/attrdot.xpm \
  bitmap/attr/252/attrn.xpm bitmap/attr/252/attrnon.xpm \
  bitmap/attr/252/attrt.xpm
pixbdig.o: pixbdig.c pixread.h ../gt/widget/widget.h ../gt/include/deftype.h \
  ../gt/dev/gdev.h ../gt/dev/x11gdev.h  \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h geom.h pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h \
  bitmap/misc/bdigit0.xpm bitmap/misc/bdigit1.xpm bitmap/misc/bdigit2.xpm \
  bitmap/misc/bdigit3.xpm bitmap/misc/bdigit4.xpm bitmap/misc/bdigit5.xpm \
  bitmap/misc/bdigit6.xpm bitmap/misc/bdigit7.xpm bitmap/misc/bdigit8.xpm \
  bitmap/misc/bdigit9.xpm bitmap/misc/bdigitm.xpm bitmap/misc/adigit0.xpm \
  bitmap/misc/adigit1.xpm bitmap/misc/adigit2.xpm bitmap/misc/adigit3.xpm \
  bitmap/misc/adigit4.xpm bitmap/misc/adigit5.xpm bitmap/misc/adigit6.xpm \
  bitmap/misc/adigit7.xpm bitmap/misc/adigit8.xpm bitmap/misc/adigit9.xpm \
  bitmap/misc/adigitm.xpm
pixdec.o: pixdec.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    bitmap/misc/digit0.xpm \
  bitmap/misc/digit1.xpm bitmap/misc/digit2.xpm bitmap/misc/digit3.xpm \
  bitmap/misc/digit4.xpm bitmap/misc/digit5.xpm bitmap/misc/digit6.xpm \
  bitmap/misc/digit7.xpm bitmap/misc/digit8.xpm bitmap/misc/digit9.xpm \
  bitmap/misc/bigton.xpm bitmap/misc/bignan.xpm \
  bitmap/misc/bigsya.xpm bitmap/misc/bigpei.xpm bitmap/misc/kyoku.xpm \
  bitmap/misc/honba.xpm bitmap/misc/reststr.xpm bitmap/misc/dorastr.xpm \
  bitmap/misc/uradoras.xpm bitmap/misc/ie.xpm bitmap/misc/n1hon.xpm \
  bitmap/misc/n2hon.xpm bitmap/misc/n3hon.xpm bitmap/misc/n4hon.xpm \
  bitmap/misc/n5hon.xpm bitmap/misc/userstat.xpm bitmap/misc/ch1.xpm \
  bitmap/misc/ch2.xpm bitmap/misc/ch0.xpm bitmap/misc/mainbg.xpm \
  bitmap/misc/logo.xpm \
  bitmap/misc/asia.xpm bitmap/misc/presskey.xpm bitmap/misc/current.xpm \
  bitmap/misc/btkan.xpm bitmap/misc/btreach.xpm bitmap/misc/bttie.xpm \
  bitmap/misc/btpon.xpm bitmap/misc/bttumo.xpm bitmap/misc/btron.xpm \
  bitmap/misc/mt_rest_.xpm bitmap/misc/num1.xpm \
  bitmap/misc/num2.xpm bitmap/misc/num3.xpm bitmap/misc/num4.xpm
pixmisc.o: pixmisc.c pixread.h ../gt/widget/widget.h ../gt/include/deftype.h \
  ../gt/dev/gdev.h ../gt/dev/x11gdev.h  \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h geom.h pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h \
  bitmap/misc/hometon.xpm bitmap/misc/homenan.xpm bitmap/misc/homesya.xpm \
  bitmap/misc/homepei.xpm bitmap/misc/statis.xpm
pixpai2.o: pixpai2.c xnet.h   \
  bitmap/pai3/paibase.h bitmap/pai3/paidata.h \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h 

pixread.o: pixread.c pixread.h ../gt/widget/widget.h ../gt/include/deftype.h \
  ../gt/dev/gdev.h ../gt/dev/x11gdev.h  \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h geom.h pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h
pixyaku.o: pixyaku.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
  bitmap/misc/rryukyok.xpm bitmap/misc/rtumo.xpm bitmap/misc/rron.xpm \
  bitmap/misc/rchonbo.xpm bitmap/misc/pointh.xpm bitmap/misc/gameendh.xpm \
  bitmap/misc/fusfx.xpm bitmap/misc/fansfx.xpm

playinfo.o: playinfo.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    ../gt/widget/text.h \
  pfinfo.h
rank.o: rank.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    pfinfo.h
rc.o: rc.c xnetmaj.h ../../..//pai.h ../../..//global.h  \
   ../../..//comm.h  \
   ../../..//result.h xnet.h  \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h  \
  ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h ../gt/widget/itemtype.h \
   ../gt/widget/resource.h ../gt/widget/event.h \
  ../gt/widget/funcdecl.h ../gt/widget/widgetde.h ../util/util.h flags.h \
  geom.h decl.h pixread.h pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h \
  ../misc/mydir.h   \
  ../int/export.h
res.o: res.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    ../../..//ahand.h
rule.o: rule.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    ../../..//ruledef.h
talkcmd.o: talkcmd.c   \
   xnet.h  ../gt/widget/widget.h \
  ../gt/include/deftype.h ../gt/dev/gdev.h ../gt/dev/x11gdev.h \
  ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h ../gt/widget/itemtype.h \
   ../gt/widget/resource.h ../gt/widget/event.h \
  ../gt/widget/funcdecl.h ../gt/widget/widgetde.h ../util/util.h flags.h \
  geom.h decl.h xnetmaj.h ../../..//pai.h ../../..//global.h \
   ../../..//comm.h  \
  ../../..//result.h pixread.h pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h \
  ../misc/mydir.h  
testpai.o: testpai.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
title.o: title.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
x11intin.o: x11intin.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    rc.c ../int/export.h
yaku.o: yaku.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \

xui2.o: xui2.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    misc.h
config.o: config.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
    lang.h
board2.o: board2.c xnet.h   \
  ../gt/widget/widget.h ../gt/include/deftype.h ../gt/dev/gdev.h \
  ../gt/dev/x11gdev.h   \
   ../gt/dev/x11spr.h ../gt/dev/x11gdevd.h \
  ../gt/widget/itemtype.h  ../gt/widget/resource.h \
  ../gt/widget/event.h ../gt/widget/funcdecl.h ../gt/widget/widgetde.h \
  ../util/util.h flags.h geom.h decl.h xnetmaj.h ../../..//pai.h \
  ../../..//global.h  ../../..//comm.h  \
    ../../..//result.h pixread.h \
  pixdec.h pixbdig.h pixmisc.h pixyaku.h pmutil.h ../misc/mydir.h \
   
