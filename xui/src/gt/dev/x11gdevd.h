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
 *  See the gpl.text for more details.
 *
 *   If you would like to do something with NETMAJ that this copyright
 *  prohibits (such as distributing it with a commercial product,
 *  using portions of the source in some other program, etc.), please
 *  contact the author (preferably via email).  Arrangements can
 *  probably be worked out.
 *
 *   Todo Software : NBC02365@niftyserve.or.jp
 */
/*  -*- Mode: C -*- 
 * This file is part of xui
 * 
 * x11gdevd.h -- 
 * 
 * $Id: x11gdevd.h,v 1.2 1996/07/31 22:46:55 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:55:38 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:55:42 1996
 * description:
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#if ANSIC || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* x11gc.c */
gc_t gc_new P_((void));
void gc_set_foreground P_((gc_t gc, integer_t color));
void gc_set_background P_((gc_t gc, integer_t color));

#undef P_
#if ANSIC || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* x11gdev.c */
void gdev_clear P_((gdev_t gdev));
void gdev_cleararea P_((gdev_t gdev, integer_t left, integer_t top, integer_t w, integer_t h));
void gdev_fatal P_((gdev_t gdev, char *s));
gdev_t gdev_new P_((void));
void gdev_set_gc P_((gdev_t dev, gc_t gc));
void gdev_rel_gc P_((gdev_t dev, gc_t gc));
integer_t gdev_colornametopixel P_((gdev_t gdev, char *s));
integer_t colornametopixel P_((char *s));
void gdev_assign P_((gdev_t gdev));
void gdev_drawable_copy P_((gdev_t gdev, gpixmap_t src, gpixmap_t dest, integer_t src_x, integer_t src_y, integer_t width, integer_t height, integer_t dest_x, integer_t dest_y));
void gdev_drawable_xor_copy P_((gdev_t gdev, gpixmap_t src, gpixmap_t dest, integer_t src_x, integer_t src_y, integer_t width, integer_t height, integer_t dest_x, integer_t dest_y));
void gdev_drawable_and_copy P_((gdev_t gdev, gpixmap_t src, gpixmap_t dest, integer_t src_x, integer_t src_y, integer_t width, integer_t height, integer_t dest_x, integer_t dest_y));
void gdev_readfiletopixmap P_((gdev_t gdev, char *fname, gpixmap_t *pixmap, gpixmap_t *pixmapM));
void gdev_create_pixmapfromdata P_((gdev_t gdev, char *dataname, char **data, gpixmap_t *pixmap, gpixmap_t *pixmapM));
gpixmap_t gdev_create_bitmapfromdata P_((gdev_t gdev, char *data, integer_t w, integer_t h));
void gdev_drawable_copy_area P_((gdev_t gdev, gpixmap_t src, gpixmap_t dest, integer_t src_x, integer_t src_y, integer_t width, integer_t height, integer_t dest_x, integer_t dest_y));
void gdev_drawable_copy_plane P_((gdev_t gdev, gpixmap_t src, gpixmap_t dest, integer_t src_x, integer_t src_y, integer_t width, integer_t height, integer_t dest_x, integer_t dest_y));
void gdev_set_background P_((gdev_t gdev, gpixmap_t pixmap));
void gdev_drawable_get_geometry P_((gdev_t gdev, gpixmap_t gpixmap, integer_t *width, integer_t *height));
void gdev_map P_((gdev_t gdev));
void gdev_init P_((integer_t w, integer_t h));
void gdev_flush P_((gdev_t gdev));
void gdev_flush_area P_((gdev_t gdev, integer_t left, integer_t top, integer_t w, integer_t h));
void gdev_sync P_((gdev_t gdev, integer_t i));
void gdev_setwindowsizehints P_((gdev_t gdev, integer_t w, integer_t h));
void gdev_drawtext P_((gdev_t gdev, integer_t x, integer_t y, integer_t colour, integer_t fontid, char *text, integer_t len));
void gdev_drawshadowtext P_((gdev_t gdev, integer_t x, integer_t y, integer_t colour, integer_t fontid, char *text, integer_t len));
integer_t gdev_have_event P_((gdev_t gdev));
void gdev_done P_((void));
void gdev_polygon P_((gdev_t dev, gc_t gc, XPoint *pt, integer_t n));
void gdev_fill_rectangle P_((gdev_t dev, gc_t gc, integer_t left, integer_t top, integer_t width, integer_t height));
void gdev_copyarea P_((gdev_t dev, Drawable src, Drawable target, gc_t gc, integer_t x, integer_t y, integer_t width, integer_t height, integer_t dest_x, integer_t dest_y));
void gdev_drawpixmap P_((gdev_t dev, gpixmap_t pixmap, gc_t gc, integer_t x, integer_t y, integer_t width, integer_t height, integer_t dest_x, integer_t dest_y));
void gdev_gpixmap_get_geometry P_((gdev_t gdev, gpixmap_t gpixmap, integer_t *width, integer_t *height));
void whereabort P_((char *f, integer_t l));
void xabort P_((void));
integer_t gdev_string_n_pixels P_((gdev_t dev, integer_t fontid, char *str, integer_t len));
integer_t gdev_rgb2pixel P_((gdev_t dev, integer_t r, integer_t g, integer_t b));
void gdev_pixel2rgb P_((gdev_t dev, integer_t pixel, integer_t *r, integer_t *g, integer_t *b));

#undef P_
#if ANSIC || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

#undef P_
#if ANSIC || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* x11rend.c */
void gdev_renderpixmap P_((gdev_t dev, gpixmap_t p, gpixmap_t pM, gc_t gc, integer_t src_x, integer_t src_y, integer_t w, integer_t h, integer_t x, integer_t y));

#undef P_
#if ANSIC || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* x11sfx.c */
void gdev_flush_sfx_blind P_((gdev_t gdev, integer_t xx, integer_t yy, integer_t ww, integer_t hh, integer_t pitch, integer_t n_div));
void gdev_flush_sfx_shatter P_((gdev_t gdev, integer_t xx, integer_t yy, integer_t ww, integer_t hh, integer_t pitch, integer_t n_div));
void gdev_flush_sfx_zoom P_((gdev_t gdev, integer_t xx, integer_t yy, integer_t ww, integer_t hh, integer_t pitch, integer_t n_div));
void gdev_flush_sfx_shaffle P_((gdev_t gdev, integer_t xx, integer_t yy, integer_t ww, integer_t hh, integer_t pitch, integer_t n_div));
void gdev_flush_sfx_doubleblind P_((gdev_t gdev, integer_t xx, integer_t yy, integer_t ww, integer_t hh, integer_t pitch, integer_t n_div));

#undef P_
#if ANSIC || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* x11spr.c */
void gdev_draw_sprite P_((gdev_t gdev, integer_t x, integer_t y, gsprite_t sprite));

#undef P_
