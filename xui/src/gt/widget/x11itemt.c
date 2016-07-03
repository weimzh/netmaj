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
/*  -*- Mode: C -*- 
 * This file is part of xui
 * 
 * x11itemt.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:56:31 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"

#define MAX_INTENSITY 0xffff

static integer_t
rgb_trunc (i)
     integer_t i;
{
  if (i > MAX_INTENSITY)
    {
      return MAX_INTENSITY;
    }
  return i;
}


#define DARK(e)  rgb_trunc((60 * (int) (e) / 100))
#define LIGHT(e) rgb_trunc((14 * (int) (e) / 10))

static void
calc_dark_light (dev, mainfg, mainbg, darkp, lightp)
     gdev_t dev;
     integer_t mainfg, mainbg, *darkp, *lightp;
{
  static struct
    {
      integer_t fgpixel, bgpixel, darkpixel, lightpixel;
    }
  cache;
  integer_t dark, light;
  integer_t r, g, b;
  if (cache.fgpixel == mainfg)
    {
      dark = cache.darkpixel;
      light = cache.lightpixel;
    }
  else
    {
      gdev_pixel2rgb (dev, mainfg, &r, &g, &b);
      dark = gdev_rgb2pixel (dev, DARK (r), DARK (g), DARK (b));
      light = gdev_rgb2pixel (dev, LIGHT (r), LIGHT (g), LIGHT (b));
      cache.fgpixel = mainfg;
      cache.darkpixel = dark;
      cache.lightpixel = light;
    }
  if (cache.bgpixel == mainbg)
    {
      ;
    }
  else
    {
      cache.bgpixel = mainbg;
    }
  *darkp = dark;
  *lightp = light;
}

extern integer_t black;

void
model_color_alloc (dev, pixel)
     gdev_t dev;
     integer_t pixel;
{
  integer_t d, l;
  calc_dark_light (dev, pixel, black, &d, &l);
}


static void
_model_init (dev, m, mainfg, mainbg)
     gdev_t dev;
     model_t m;
     integer_t mainfg, mainbg;	/* pixel */
{
  integer_t dark, light;

  calc_dark_light (dev, mainfg, mainbg,
		   &dark, &light);
  m->main.gc->fg = mainfg;
  m->main.gc->bg = mainbg;
  m->light.gc->fg = light;
  m->light.gc->bg = black;
  m->dark.gc->fg = dark;
  m->dark.gc->bg = black;
}

void
model_init (dev, m, mainfg, mainbg)
     gdev_t dev;
     model_t m;
     integer_t mainfg;		/* pixel */
     integer_t mainbg;		/* pixel */
{
  m->main.gc = (gc_t) gc_new ();
  m->sub.gc = (gc_t) gc_new ();
  m->light.gc = (gc_t) gc_new ();
  m->dark.gc = (gc_t) gc_new ();

  _model_init (dev, m, mainfg, mainbg);
}

void
model_change (gdev, m, mainfg, mainbg)
     gdev_t gdev;
     model_t m;
     integer_t mainfg;
     integer_t mainbg;
{
  _model_init (gdev, m, mainfg, mainbg);
}

void
widget_model_change (w, mainfg, mainbg, fg, bg)
     widget_t w;
     integer_t mainfg;
     integer_t mainbg;
     integer_t fg, bg;
{
  _model_init (widget_gdev (w), &w->active, mainfg, mainbg);
  _model_init (widget_gdev (w), &w->inactive, mainfg, mainbg);
}
