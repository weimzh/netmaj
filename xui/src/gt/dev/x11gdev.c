/*  NETMAJ-XUI -- X User Interface --  Copyright Notice
 *
 *      Copyright (c) 1995,1996 Todo Software All rights reserved.
 *      Copyright (C) 1994, 1995, 1996 Koji Suzuki (suz@kt.rim.or.jp)
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
 *   Koji Suzuki   : suz@kt.rim.or.jp
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "gt/dev/x11gdev.h"
#include "util/util.h"
#include "gt/dev/x11font.h"

static integer_t DISP_W;
static integer_t DISP_H;

#define MAX_PIXMAPS 1000
static struct
  {
    gpixmap_t a;
    gpixmap_t Ma;
  }
loadedpixmaps[MAX_PIXMAPS];
static int n_loadedpixmaps;
#define check_pixmap() if (n_loadedpixmaps >= MAX_PIXMAPS) abort();

static void
gdev_load_font (gdev)
     gdev_t gdev;
{
  Display *display = gdev->x11.g.display;
#define font1 "-adobe-times-medium-i-normal--*-120-*-*-p-*-iso8859-1"
#define font2 "-adobe-times-bold-i-normal--*-120-*-*-p-*-iso8859-1"
#define font3 "-adobe-helvetica-medium-r-normal--*-120-*-*-p-*-iso8859-1"
#define font4 "-adobe-helvetica-bold-r-normal--*-120-*-*-p-*-iso8859-1"
#define font5 "-b&h-lucida-medium-i-normal-sans-*-120-*-*-p-*-iso8859-1"
#define font6 "-b&h-lucida-bold-i-normal-sans-*-180-*-*-p-*-iso8859-1"
#define font7 "-b&h-lucidabright-demibold-r-normal--*-180-*-*-p-*-iso8859-1"
#define font8 "-adobe-times-bold-i-normal--*-180-*-*-p-*-iso8859-1"
#define font9 "-adobe-times-bold-i-normal--*-240-*-*-p-*-iso8859-1"
#define font10 "-*-fixed-medium-r-normal--16-*-*-*-*-*-iso8859-1"
#define font11 "-*-fixed-medium-r-normal--16-*-*-*-*-*-iso8859-1"
#define font12 "-*-fixed-medium-r-normal--14-*-*-*-*-*-iso8859-1"
#define jfont0 "-*-fixed-medium-r-normal--24-*-*-*-*-*-jisx0208.1983-0"
#define jfont1 "-*-fixed-medium-r-normal--16-*-*-*-*-*-jisx0208.1983-0"
#define jfont2 "-*-fixed-medium-r-normal--14-*-*-*-*-*-jisx0208.1983-0"
#define font10alt "-adobe-courier-medium-r-normal--16-*-*-*-*-*-iso8859-1"
#define font11alt "-adobe-courier-medium-r-normal--16-*-*-*-*-*-iso8859-1"
#define font12alt "-adobe-courier-medium-r-normal--14-*-*-*-*-*-iso8859-1"

  int i;
  gdev->x11.fonts[0] = XLoadQueryFont (display, "fixed");
  gdev->x11.fonts[1] = XLoadQueryFont (display, font1);
  gdev->x11.fonts[2] = XLoadQueryFont (display, font2);
  gdev->x11.fonts[3] = XLoadQueryFont (display, font3);
  gdev->x11.fonts[4] = XLoadQueryFont (display, font4);
  gdev->x11.fonts[5] = XLoadQueryFont (display, font5);
  gdev->x11.fonts[6] = XLoadQueryFont (display, font6);
  gdev->x11.fonts[7] = XLoadQueryFont (display, font7);
  gdev->x11.fonts[8] = XLoadQueryFont (display, font8);
  gdev->x11.fonts[9] = XLoadQueryFont (display, font9);
  gdev->x11.fonts[10] = XLoadQueryFont (display, font10);
  gdev->x11.fonts[11] = XLoadQueryFont (display, font11);
  gdev->x11.fonts[12] = XLoadQueryFont (display, font12);
  if (!gdev->x11.fonts[10])
	gdev->x11.fonts[10] = XLoadQueryFont (display, font10alt);
  if (!gdev->x11.fonts[11])
	gdev->x11.fonts[11] = XLoadQueryFont (display, font11alt);
  if (!gdev->x11.fonts[12])
	gdev->x11.fonts[12] = XLoadQueryFont (display, font12alt);
  gdev->x11.jfonts[0] = XLoadQueryFont (display, jfont0);
  gdev->x11.jfonts[1] = XLoadQueryFont (display, jfont1);
  gdev->x11.jfonts[2] = XLoadQueryFont (display, jfont2);
}

void
gdev_clear (gdev)
     gdev_t gdev;
{
  XClearWindow (gdev->x11.g.display,
		gdev->x11.g.window);
}

void
gdev_cleararea (gdev, left, top, w, h)
     gdev_t gdev;
     integer_t left, top, w, h;
{
  XFillRectangle (gdev->x11.g.display,
		  gdev->x11.hidden,
		  gdev->x11.gc,
		  left, top, w, h);
}


void
gdev_fatal (gdev, s)
     gdev_t gdev;
     char *s;
{
  write (2, s, strlen (s));
  exit (2);
}

gdev_t
gdev_new ()
{
  return (gdev_t) xcalloc (sizeof (struct gdev));
}

static Display *xdisplay;
static Colormap xcolormap;

GC gc, gcxor, gcand, gcor, gcsfx, gccopy, gcplain;
integer_t red, tann, yellow, green, white, black, purple, blue, tomato,
  gray;
integer_t darkgreen, yellowgreen, forestgreen, seagreen, darkskyblue;
integer_t darkorange, indianred, maroon, deeppink;
integer_t khaki, chocolate, darkorchid;

gdev_t main_gdev;

void
gdev_set_gc (dev, gc)
     gdev_t dev;
     gc_t gc;
{
  Display *display = dev->x11.g.display;
/*  if (gc->setfg != gc->fg) */
  XSetForeground (display, gc->gc, gc->fg);

/*  if (gc->setbg != gc->bg) */
  XSetBackground (display, gc->gc, gc->bg);

  gc->setbg = gc->bg;
  gc->setfg = gc->fg;
}

void
gdev_rel_gc (dev, gc)
     gdev_t dev;
     gc_t gc;
{

}

static void
initialize_graphics (gdev)
     gdev_t gdev;
{
  Display *display = gdev->x11.g.display;
  Window window = gdev->x11.g.window;
  XGCValues gcv;

  /* Create a graphics context using copy mode */
  gcv.function = GXcopy;
  gcv.graphics_exposures = False;
  gcv.plane_mask = ~0;
  if (!(gccopy = XCreateGC (display, window,
		     GCPlaneMask | GCFunction | GCGraphicsExposures, &gcv)))
    {
      gdev_fatal (gdev, "Cannot create GXcopy1 graphics context.");
    }
  gcv.function = GXcopy;
  gcv.graphics_exposures = False;
  gcv.plane_mask = ~0;
  if (!(gcplain = XCreateGC (display, window,
		     GCPlaneMask | GCFunction | GCGraphicsExposures, &gcv)))
    {
      gdev_fatal (gdev, "Cannot create GXcopy1 graphics context.");
    }
  /* Create a graphics context using copy mode */
  gcv.function = GXcopy;
  gcv.fill_style = FillTiled;
  gcv.graphics_exposures = False;
  if (!(gc = XCreateGC (display, window,
		     GCFunction | GCFillStyle | GCGraphicsExposures, &gcv)))
    {
      gdev_fatal (gdev, "Cannot create GXcopy2 graphics context.");
    }
  /* Create a graphics context using xor mode */
  gcv.function = GXxor;
  gcv.graphics_exposures = False;
  if (!(gcxor = XCreateGC (display, window,
			   GCFunction | GCGraphicsExposures, &gcv)))
    {
      gdev_fatal (gdev, "Cannot create GXxor graphics context.");
    }

  /* Create a graphics context using and mode */
  gcv.function = GXand;
  gcv.graphics_exposures = False;
  if (!(gcand = XCreateGC (display, window,
			   GCFunction | GCGraphicsExposures, &gcv)))
    {
      gdev_fatal (gdev, "Cannot create GXand graphics context.");
    }
  XSetForeground (display, gcand, 0);
  XSetBackground (display, gcand, ~0);

  /* Create a graphics context using or mode */
  gcv.function = GXor;
  gcv.graphics_exposures = False;
  if (!(gcor = XCreateGC (display, window,
			  GCFunction | GCGraphicsExposures, &gcv)))
    {
      gdev_fatal (gdev, "Cannot create GXor graphics context.");
    }
  /* Create a graphics context for use by sfx */
  gcv.function = GXcopy;
  gcv.graphics_exposures = False;
  if (!(gcsfx = XCreateGC (display, window,
			   GCFunction | GCGraphicsExposures, &gcv)))
    {
      gdev_fatal (gdev, "Cannot create GXsfx graphics context.");
    }
}

static XVisualInfo visual_info;

static void
initialize_color ()
{
#if X11
  int screen_num;
  Display *display = xdisplay;

  /* Obtain the screen number for this display */
  screen_num = XDefaultScreen (display);

  /* Make sure that we are using a color visual */
  if (!XMatchVisualInfo (display, screen_num,
	     DefaultDepth (display, screen_num), PseudoColor, &visual_info))
    {
      if (!XMatchVisualInfo (display, screen_num,
	     DefaultDepth (display, screen_num), DirectColor, &visual_info))
	{
	  if (!XMatchVisualInfo (display, screen_num,
	       DefaultDepth (display, screen_num), TrueColor, &visual_info))
	    {
	      error ("color display expected.");
	    }
	}
    }

#if 0
  xcolormap = XCreateColormap (display, RootWindow (display, screen_num),
			       visual_info.visual, AllocNone);
  XInstallColormap (display, xcolormap);
#else
  xcolormap = XDefaultColormap (display, screen_num);
#endif
#endif
}

integer_t
gdev_colornametopixel (gdev, s)
     gdev_t gdev;
     char *s;
{
#if X11
  XColor xcolor;
  Display *display;
  Colormap colormap;

  display = gdev->x11.g.display;
  colormap = gdev->x11.colormap;

  XParseColor (display, colormap, s, &xcolor);
  XAllocColor (display, colormap, &xcolor);
  return xcolor.pixel;
#endif
}

integer_t
colornametopixel (s)
     char *s;
{
  extern gdev_t main_gdev;
  return gdev_colornametopixel (main_gdev, s);
}

static void
initialize_colorname (gdev)
     gdev_t gdev;
{
#if X11
  /* Obtain the colour index of several colours from colourmap */
  red = gdev_colornametopixel (gdev, "red");
  tann = gdev_colornametopixel (gdev, "tan");
  yellow = gdev_colornametopixel (gdev, "yellow");
  green = gdev_colornametopixel (gdev, "green");
  white = gdev_colornametopixel (gdev, "white");
  black = gdev_colornametopixel (gdev, "black");
  purple = gdev_colornametopixel (gdev, "purple");
  blue = gdev_colornametopixel (gdev, "blue");
  darkgreen = gdev_colornametopixel (gdev, "DarkGreen");
  yellowgreen = gdev_colornametopixel (gdev, "YellowGreen");
  forestgreen = gdev_colornametopixel (gdev, "ForestGreen");
  seagreen = gdev_colornametopixel (gdev, "SeaGreen");
  darkorange = gdev_colornametopixel (gdev, "DarkOrange");
  tomato = gdev_colornametopixel (gdev, "tomato");
  gray = gdev_colornametopixel (gdev, "gray");
  darkskyblue = gdev_colornametopixel (gdev, "DeepSkyBlue4");
  indianred = gdev_colornametopixel (gdev, "indianred");
  maroon = gdev_colornametopixel (gdev, "maroon3");
  deeppink = gdev_colornametopixel (gdev, "deeppink");
  khaki = gdev_colornametopixel (gdev, "khaki4");
  chocolate = gdev_colornametopixel (gdev, "chocolate");
  darkorchid = gdev_colornametopixel (gdev, "darkorchid");
#endif
}


#define	XEVENTS	(KeyPressMask | ExposureMask | ButtonPressMask |\
		 ButtonReleaseMask | StructureNotifyMask |\
		 EnterWindowMask | LeaveWindowMask | FocusChangeMask |\
		 KeyPressMask | ExposureMask )

void
gdev_assign (gdev)
     gdev_t gdev;
{
  char *argv[2];
  XTextProperty w_name_prop, i_name_prop;

  XSizeHints size_hints;
  XWMHints wm_hints;
  Display *display = xdisplay;
  Window win;
  GC gc, fg, bg, textgc, cursorgc;
  int screen;
  int fgpixel, bgpixel;
  XFontStruct *font_struct;
  int display_width, display_height;
#ifndef XNAME
#define XNAME "Network Mahjong for X"
#endif
  char *win_name = XNAME;
  char *icon_name = XNAME;

  argv[0] = XNAME, argv[1] = 0;
  screen = DefaultScreen (display);

  font_struct = XLoadQueryFont (display, "fixed");
  if (!font_struct)
    {
      gdev_fatal (0, "cannot load font");
    }

/* Normal Graphic Plane Size is 600 * 480 */
  display_width = DISP_W;
  display_height = DISP_H;

  win = XCreateSimpleWindow (display, DefaultRootWindow (display),
			     0, 0, display_width, display_height,
			     2,
			     BlackPixel (display, screen),
			     WhitePixel (display, screen));

  XStringListToTextProperty (&win_name, 1, &w_name_prop);
  XStringListToTextProperty (&icon_name, 1, &i_name_prop);

  size_hints.flags = PSize | PMinSize | PMaxSize;
  size_hints.width = size_hints.min_width = size_hints.max_width =
    display_width;
  size_hints.height = size_hints.min_height = size_hints.max_height =
    display_height;

  XSetWMProperties (display, win, &w_name_prop, &i_name_prop, argv, 1,
		    &size_hints, NULL, NULL);

  wm_hints.flags = InputHint;
  wm_hints.input = True;
  XSetWMHints (display, win, &wm_hints);

  XSelectInput (display, win, XEVENTS);

/*  gcv.graphics_exposures = False;
   gc = XCreateGC (display, win, GCGraphicsExposures, &gcv); */

  gc = XCreateGC (display, win, 0, 0);

  fgpixel = WhitePixel (display, screen);
  bgpixel = BlackPixel (display, screen);

  fg = XCreateGC (display, win, 0, 0);
  XSetForeground (display, fg, fgpixel);
  XSetBackground (display, fg, bgpixel);

  bg = XCreateGC (display, win, 0, 0);
  XSetForeground (display, bg, fgpixel);
  XSetBackground (display, bg, bgpixel);

  textgc = XCreateGC (display, win, 0, 0);
  XSetForeground (display, textgc, fgpixel);
  XSetBackground (display, textgc, bgpixel);

  cursorgc = XCreateGC (display, win, 0, 0);
  XSetForeground (display, cursorgc, bgpixel);
  XSetBackground (display, cursorgc, fgpixel);

  XSetFont (display, textgc, font_struct->fid);

  gdev->x11.depth = DefaultDepth (display, screen);
  gdev->x11.g.display = display;
  gdev->x11.g.window = win;
  gdev->x11.g.width = display_width;
  gdev->x11.g.height = display_height;
  gdev->x11.screen = screen;
  gdev->x11.gc = gc;
  gdev->x11.fg = fg;
  gdev->x11.bg = bg;
  gdev->x11.hidden = XCreatePixmap (gdev->x11.g.display, gdev->x11.g.window,
				    gdev->x11.g.width,
				    gdev->x11.g.height,
				    gdev->x11.depth);
  {
    XSetWindowAttributes winattr;
    int valuemask;
    valuemask = CWColormap;
    winattr.colormap = xcolormap;
    XChangeWindowAttributes (display, win, valuemask, &winattr);
    gdev->x11.colormap = xcolormap;
  }
  gdev->x11.fontstruct = font_struct;

  gdev_load_font (gdev);

  XFillRectangle (gdev->x11.g.display,
		  gdev->x11.hidden,
		  gdev->x11.gc,
		  0, 0, gdev->x11.g.width, gdev->x11.g.height);

}


void
gdev_drawable_copy (gdev, src, dest, src_x, src_y, width, height, dest_x, dest_y)
     gdev_t gdev;
     gpixmap_t src;
     gpixmap_t dest;
     integer_t src_x, src_y, width, height, dest_x, dest_y;
{
}

void
gdev_drawable_xor_copy (gdev, src, dest, src_x, src_y, width, height, dest_x, dest_y)
     gdev_t gdev;
     gpixmap_t src;
     gpixmap_t dest;
     integer_t src_x, src_y, width, height, dest_x, dest_y;
{
}

void
gdev_drawable_and_copy (gdev, src, dest, src_x, src_y, width, height, dest_x, dest_y)
     gdev_t gdev;
     gpixmap_t src;
     gpixmap_t dest;
     integer_t src_x, src_y, width, height, dest_x, dest_y;
{
}

static
void
xpmerror (display, ErrorStatus, tag, tag2)
     Display *display;
     int ErrorStatus;
     char *tag;
     char *tag2;
{
  char *error = NULL;
  char *warning = NULL;

  /* Switch on the type of error returned by xpm library */
  switch (ErrorStatus)
    {
    case XpmSuccess:
      return;

    case XpmColorError:
      /* The colour name passed was bung */
      warning = "Could not parse or alloc requested colour";
      break;

    case XpmNoMemory:
      /* Not enough memory for pixmap */
      error = "Not enough memory for pixmap creation";
      break;

    case XpmColorFailed:
      /* No more entries available in colourmap */
      error = "Colourmap is full - cannot allocate a colour";
      break;

    case XpmOpenFailed:
      /* Xpm could not open the pixmap file */
      error = "Unable to open pixmap file";
      break;

    case XpmFileInvalid:
      /* XPM file contains invalid or corrupt data */
      error = "XPM file contains invalid or corrupt data";
      break;

    default:
      /* Unexpected xpm error code */
      error = "Unexpected xpm error code";
      break;
    }

  /* If there is to be a warning then issue it */
  if (warning)
    fprintf (stdout, "%s %s - Warning: %s.\n", tag, tag2, warning);

  if (error)
    {
      /* Argg. An error so tell everyone */
      fprintf (stderr, "%s %s - Error: %s.\n", tag, tag2, error);
      gdev_fatal (main_gdev, "xpm error");
    }
}

void
gdev_readfiletopixmap (gdev, fname, pixmap, pixmapM)
     gdev_t gdev;
     char *fname;
     gpixmap_t *pixmap;
     gpixmap_t *pixmapM;
{
  integer_t XpmErrorStatus;
  XpmAttributes attributes;

  attributes.valuemask = XpmColormap;
  attributes.colormap = gdev->x11.colormap;

  XpmErrorStatus = XpmReadFileToPixmap (gdev->x11.g.display,
					gdev->x11.g.window,
					fname,
					pixmap, pixmapM,
					&attributes);

  loadedpixmaps[n_loadedpixmaps].a = *pixmap;
  loadedpixmaps[n_loadedpixmaps++].Ma = *pixmapM;
  check_pixmap ();

  xpmerror (gdev->x11.g.display, XpmErrorStatus, "XpmReadFileToPixmap", fname);
}

void
gdev_create_pixmapfromdata (gdev, dataname, data, pixmap, pixmapM)
     gdev_t gdev;
     char *dataname;
     char **data;
     gpixmap_t *pixmap;
     gpixmap_t *pixmapM;
{
  integer_t XpmErrorStatus;
  XpmAttributes attributes;

  attributes.valuemask = XpmColormap;
  attributes.colormap = gdev->x11.colormap;

  XpmErrorStatus = XpmCreatePixmapFromData (gdev->x11.g.display,
					    gdev->x11.g.window,
					    data,
					    pixmap, pixmapM,
					    &attributes);

  loadedpixmaps[n_loadedpixmaps].a = *pixmap;
  loadedpixmaps[n_loadedpixmaps++].Ma = *pixmapM;
  check_pixmap ();

  xpmerror (gdev->x11.g.display, XpmErrorStatus, "XpmCreatePixmapFromData", dataname);
}


gpixmap_t
gdev_create_bitmapfromdata (gdev, data, w, h)
     gdev_t gdev;
     char *data;
     integer_t w, h;
{
  Pixmap p;
  p = XCreateBitmapFromData (gdev->x11.g.display,
			     gdev->x11.g.window,
			     data,
			     w, h);

  loadedpixmaps[n_loadedpixmaps].a = p;
  loadedpixmaps[n_loadedpixmaps++].Ma = 0;
  check_pixmap ();

  return (gpixmap_t) p;
}



void
gdev_drawable_copy_area (gdev, src, dest, src_x, src_y, width, height,
			 dest_x, dest_y)
     gdev_t gdev;
     gpixmap_t src, dest;
     integer_t src_x, src_y, width, height, dest_x, dest_y;
{
  XCopyArea (gdev->x11.g.display,
	     src, dest, gc,
	     src_x, src_y, width, height,
	     dest_x, dest_y);
}

void
gdev_drawable_copy_plane (gdev, src, dest, src_x, src_y, width, height,
			  dest_x, dest_y)
     gdev_t gdev;
     gpixmap_t src, dest;
     integer_t src_x, src_y, width, height, dest_x, dest_y;
{
  XCopyPlane (gdev->x11.g.display,
	      src, dest, gdev->x11.fg,
	      src_x, src_y, width, height,
	      dest_x, dest_y, 1);
}

void
gdev_set_background (gdev, pixmap)
     gdev_t gdev;
     gpixmap_t pixmap;
{
  XSetWindowBackgroundPixmap (gdev->x11.g.display, gdev->x11.g.window,
			      pixmap);
}


void
gdev_drawable_get_geometry (gdev, gpixmap, width, height)
     gdev_t gdev;
     gpixmap_t gpixmap;
     integer_t *width, *height;
{
  Window win;
  int x, y;
  int border_width, depth;

  XGetGeometry (gdev->x11.g.display, gpixmap, &win, &x, &y, (unsigned int *) width,
		(unsigned int *) height,
		(unsigned int *) &border_width, (unsigned int *) &depth);
}


void
gdev_map (gdev)
     gdev_t gdev;
{
  XMapWindow (gdev->x11.g.display, gdev->x11.g.window);
}


void
gdev_init (w, h)
     integer_t w, h;
{
  xdisplay = XOpenDisplay (NULL);
  if (xdisplay == NULL)
    {
      gdev_fatal (0, "unable to open display: please specify DISPLAY\n");
    }
  {
    char *getenv ();
    char *locale = getenv ("LANG");
    if (!locale)
      locale = "C";
    setlocale (LC_ALL, locale);
  }

  DISP_W = w;
  DISP_H = h;

  initialize_color ();

  main_gdev = gdev_new ();
  gdev_assign (main_gdev);
  initialize_graphics (main_gdev);
  initialize_colorname (main_gdev);

  XSync (main_gdev->x11.g.display, 0);

  gdev_clear (main_gdev);
}

void
gdev_flush (gdev)
     gdev_t gdev;
{
  XCopyArea (gdev->x11.g.display,
	     gdev->x11.hidden,
	     gdev->x11.g.window,
	     gdev->x11.gc,
	     0, 0,
	     gdev->x11.g.width, gdev->x11.g.height,
	     0, 0);
  XFlush (gdev->x11.g.display);
}

void
gdev_flush_area (gdev, left, top, w, h)
     gdev_t gdev;
     integer_t left, top, w, h;
{
  XCopyArea (gdev->x11.g.display,
	     gdev->x11.hidden,
	     gdev->x11.g.window,
	     gdev->x11.gc,
	     left, top,
	     w, h,		/* gdev->x11.g.width, gdev->x11.g.height, */
	     left, top);
  XFlush (gdev->x11.g.display);
}

void
gdev_sync (gdev, i)
     gdev_t gdev;
     integer_t i;
{
  gdev_flush (gdev);
  XSync (gdev->x11.g.display, i);
}


void
gdev_setwindowsizehints (gdev, w, h)
     gdev_t gdev;
     integer_t w;
     integer_t h;
{
  XSizeHints sizehints;
  Display *display = gdev->x11.g.display;
  Window window = gdev->x11.g.window;

  /* Setup the max and minimum size that the window will be */
  sizehints.flags = PPosition | PSize | PMinSize | PMaxSize;
  sizehints.min_width = w;
  sizehints.min_height = h;
  sizehints.max_width = w;
  sizehints.max_height = h;

  /* Now set the window manager size hints properties */
  XSetWMNormalHints (display, window, &sizehints);
}

void
gdev_drawtext (gdev, x, y, colour, fontid, text, len)
     gdev_t gdev;
     integer_t x;
     integer_t y;
     integer_t colour;
     integer_t fontid;
     char *text;
     integer_t len;
{
  Display *display = gdev->x11.g.display;
  Window window;
  XFontStruct *font;
  XFontStruct *jfont;
  int d,jd;
  window = gdev->x11.hidden;

  jfont = 0;
  if (fontid > _GDEV_N_FONTS)
    {
      font = gdev->x11.fontstruct;
      jfont = gdev->x11.jfonts[2];
    }
  else if (fontid >= 10)
    {
      font = gdev->x11.fonts[fontid];
      jfont = gdev->x11.jfonts[fontid-10];
    }
  else
     {
      font = gdev->x11.fonts[fontid];
     }

    d = font->ascent;
    jd = jfont?(jfont->ascent + jfont->descent):0;
	if (jd>d) d = jd;

  /* Change the drawing function */
  XSetBackground (display, gcxor, colour);
  XSetForeground (display, gcxor, colour);

  if (!jfont)
    {
      /* Change to the new font */
      XSetFont (display, gcxor, font->fid);
      XSetFont (display, gcand, font->fid);

      /* Draw the string into the drawable */
      XDrawString (display, window, gcxor, x, y + d, text, len);
      XDrawString (display, window, gcand, x, y + d, text, len);
      XDrawString (display, window, gcxor, x, y + d, text, len);
    }
  else
    {
      DrawString16 (display, window, jfont, font, gcxor, x, y + d, text, len);
      DrawString16 (display, window, jfont, font, gcand, x, y + d, text, len);
      DrawString16 (display, window, jfont, font, gcxor, x, y + d, text, len);
    }
}

void
gdev_drawshadowtext (gdev, x, y, colour, fontid, text, len)
     gdev_t gdev;
     integer_t x;
     integer_t y;
     integer_t colour;
     integer_t fontid;
     char *text;
     integer_t len;

{
  extern integer_t black;
  /* Draw the text with a shadow */
  gdev_drawtext (gdev, x + 2, y + 2, black, fontid, text, len);
  gdev_drawtext (gdev, x, y, colour, fontid, text, len);
}

integer_t
gdev_have_event (gdev)
     gdev_t gdev;
{
  Display *display = gdev->x11.g.display;
  return XEventsQueued (display, QueuedAfterReading);
}

void
gdev_done ()
{
  int i;
/* font */

/* bitmap */
  for (i = 0; i < n_loadedpixmaps; i++)
    {
      loadedpixmaps[i].a;
      loadedpixmaps[i].Ma;
    }
/* color map */
/* memory */
  xfree ((void *)main_gdev);
  main_gdev = 0;
}


void
gdev_polygon (dev, gc, pt, n)
     gdev_t dev;
     gc_t gc;
     GDEVPOINT *pt;
     integer_t n;
{
  gdev_set_gc (dev, gc);
  XFillPolygon (dev->x11.g.display, dev->x11.hidden,
		gc->gc,
		pt, n, Nonconvex,
		CoordModeOrigin);
  gdev_rel_gc (dev, gc);
}

void
gdev_fill_rectangle (dev, gc, left, top, width, height)
     gdev_t dev;
     gc_t gc;
     integer_t left, top, width, height;
{
  gdev_set_gc (dev, gc);
  XFillRectangle (dev->x11.g.display, dev->x11.hidden, gc->gc,
		  left, top,
		  (unsigned int) width,
		  (unsigned int) height);
  gdev_rel_gc (dev, gc);
}

void
gdev_copyarea (dev, src, target, gc, x, y, width, height, dest_x, dest_y)
     gdev_t dev;
     Drawable src, target;
     gc_t gc;
     integer_t x, y, width, height;
     integer_t dest_x, dest_y;
{
  XCopyArea (dev->x11.g.display,
	     src,
	     target,
	     gccopy,
	     x, y,
	     width, height, dest_x, dest_y);
}

void
gdev_drawpixmap (dev,
		 pixmap,
		 gc,
		 x, y,
		 width,
		 height, dest_x, dest_y)
     gdev_t dev;
     gpixmap_t pixmap;
     gc_t gc;
     integer_t x, y, width, height, dest_x, dest_y;
{
  XCopyArea (dev->x11.g.display,
	     pixmap,
	     dev->x11.hidden,
	     gccopy,
	     x, y,
	     width, height, dest_x, dest_y);
}

void
gdev_gpixmap_get_geometry (gdev, gpixmap, width, height)
     gdev_t gdev;
     gpixmap_t gpixmap;
     integer_t *width, *height;
{
  Window root;
  int x, y;
  unsigned int border_width, depth, w, h;
  Status status;

  status = XGetGeometry (gdev->x11.g.display, gpixmap, &root,
			 &x, &y,
			 &w, &h,
			 &border_width, &depth);
  *width = w;
  *height = h;

}

void
whereabort (f, l)
     char *f;
     integer_t l;
{
  fprintf (stderr, "Illegal Operation: %s %ld", f, l);
  abort ();
}

void
xabort ()
{
  abort ();
}

integer_t
gdev_string_n_pixels (dev, fontid, str, len)
     gdev_t dev;
     integer_t fontid;
     char *str;
     integer_t len;
{
  return len * 8;
}



integer_t
gdev_rgb2pixel (dev, r, g, b)
     gdev_t dev;
     integer_t r, g, b;
{
  Display *display = dev->x11.g.display;
  Colormap colormap = dev->x11.colormap;
  XColor xc;

  xc.red = r;
  xc.green = g;
  xc.blue = b;
  XAllocColor (display, colormap, &xc);
  return xc.pixel;
}

void
gdev_pixel2rgb (dev, pixel, r, g, b)
     gdev_t dev;
     integer_t pixel;
     integer_t *r, *g, *b;
{
  Display *display = dev->x11.g.display;
  Colormap colormap = dev->x11.colormap;
  XColor xc;

  xc.pixel = pixel;
  XQueryColor (display, colormap, &xc);
  *r = xc.red;
  *g = xc.green;
  *b = xc.blue;
}

void gdev_event_discard(dev)
gdev_t dev;
{
  Display *display = dev->x11.g.display;
  XEvent ahead;

  while (XEventsQueued(display, QueuedAfterReading)>0) {
    XPeekEvent(display, &ahead);
    if (ahead.type != ButtonPress) {
      break;
    }
    XNextEvent(display, &ahead);
  }
}
