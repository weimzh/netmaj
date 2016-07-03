/*  NETMAJ-XUI -- X User Interface --  Copyright Notice
 *
 *      Copyright (c) 1995,1996 Todo Software All rights reserved.
 *
 *   You can redistribute it and/or modify this software under the terms
 *  of the GNU General Public License version 2 as published by
 *  the Free Software Foundation.
 *
 *   This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the author be held liable for any damages
 *  arising from the use of this software.
 *
 *   Todo Software : NBC02365@niftyserve.or.jp
 */

#ifndef _GDEV_H
#define _GDEV_H

#include "gt/include/deftype.h"
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/xpm.h>

typedef struct gscreen
  {
    Display *display;		/* display */
    Window window;
    integer_t height, width;
  }
 *gscreen_t;

typedef Pixmap gpixmap_t;

typedef struct gdev
  {
    struct
      {
	struct gscreen g;
	gpixmap_t hidden;
	GC gc;
	GC fg, bg;
	GC textgc;
	GC cursorgc;
	XFontStruct *normalfont;
	int screen;
	int depth;
	Colormap colormap;
	XFontStruct *fontstruct;
#define _GDEV_N_FONTS 15
	XFontStruct *fonts[_GDEV_N_FONTS+1];
	XFontStruct *jfonts[3]; /* Japanese */
      }
    x11;
  }
 *gdev_t;

typedef struct gc
{
  GC gc;
  XColor fgcolor, bgcolor;
  int fg;
  int bg;
  int setfg;
  int setbg;
} *gc_t;

#define GDEVPOINT XPoint

#include "gt/dev/x11spr.h"
#include "gt/dev/x11gdevd.h"

/* font alias definition */
#define FIXEDFONT      0
#define TIMESI         1 /* 12 */
#define TIMESIB        2 /* 12 */
#define HELV           3 /* 12 */
#define HELVB          4 /* 12 */
#define LUCIDA         5 /* 12 */
#define LUCIDAB        6 /* 18 */
#define LARGELUCIDAB   7 /* 18 */
#define LARGETIMESIB   8 /* 18 */
#define XLARGETIMESIB  9 /* 24 */
#define SMALLJP        10  /* 12 */
#define MEDIUMJP       11  /* 18 */
#define TINYJP         12  /* 10 */
#endif
