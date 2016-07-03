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
 * This file is part of xnetmaj
 * 
 * sfx.c -- 
 * 
 * $Id: x11sfx.c,v 1.2 1996/07/31 22:46:59 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Sat May 13 18:55:19 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Fri Sep 29 19:37:44 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "gt/dev/x11gdev.h"
#include "util/util.h"

extern GC gc;
void
gdev_flush_sfx_blind (gdev, xx, yy, ww, hh, pitch, n_div)
     gdev_t gdev;
     integer_t xx, yy, ww, hh;
     integer_t pitch;
     integer_t n_div;
{
  Display *display = gdev->x11.g.display;
  Window window = gdev->x11.g.window;
  Window hidden = gdev->x11.hidden;
  integer_t width = ww;
  integer_t height = hh;
  integer_t n_points, i, j;

  n_points = (height / pitch) / n_div;
  if (n_points == 0)
    {
      n_points = 1;
    }
  for (i = 0; i < n_div; i++)
    {
      for (j = 0; j < pitch; j++)
	{
	  integer_t y = (height / pitch) * j + (n_points * i);
	  XCopyArea (display, hidden, window, gc,
		     xx, yy + y, width, n_points, xx, yy + y);
	}
      XFlush (display);
      XSync (display, 1);
    }
  gdev_flush (gdev);

}

void
gdev_flush_sfx_shatter (gdev, xx, yy, ww, hh, pitch, n_div)
     gdev_t gdev;
     integer_t xx, yy, ww, hh;
     integer_t pitch;
     integer_t n_div;
{
  Display *display = gdev->x11.g.display;
  Window window = gdev->x11.g.window;
  Window hidden = gdev->x11.hidden;
  integer_t width = ww;
  integer_t height = hh;

  integer_t n_points, i, j;
  n_points = (height / pitch) / n_div;
  if (n_points == 0) {
    n_points = 1;
  }
  for (i = 0; i < n_div; i++)
    {
      for (j = 0; j < pitch; j++)
	{
	  integer_t x = (height / pitch) * j + (n_points * i);
	  XCopyArea (display, hidden, window, gc,
		     xx + x, yy, n_points, height, xx + x, yy);
	}
      XFlush (display);
      XSync (display, 1);

    }
  gdev_flush (gdev);

}

void
gdev_flush_sfx_zoom (gdev, xx, yy, ww, hh, pitch, n_div)
     gdev_t gdev;
     integer_t xx, yy, ww, hh, pitch, n_div;
{
}



static void
shaffle (array, n)
     integer_t *array;
     integer_t n;
{
#if 0
  static char data1[] =
  {3, 4, 5, 6, 7, 4, 3};
  static char data2[] =
  {5, 3, 4, 5, 6, 7, 5, 7};
  int i, j, k, l, m;
  i = time (0);
  i = i % n;
  j = data1[i % sizeof (data1)];
  k = data1[i % sizeof (data2)];

#define swap(a,b)   { int t; t = a; a = b; b = t; }
  for (i = 0; i < n; i++)
    {
      if (i + j < n && i - k >= 0)
	{
	  swap (array[i + j], array[i - k]);
	}
      if (i - j >= 0 && i + k < n)
	{
	  swap (array[i - j], array[i + k]);
	}
    }
  for (i = 0; i < n - 1; i++)
    {
      swap (array[i], array[i + 1]);
    }
#else
  integer_t i;
#define swap(a,b)   { int t; t = a; a = b; b = t; }
  for (i = 0; i < (n - 1) / 2; i++)
    {
      if (i % 2)
	{
	  swap (array[i], array[n - i - 1]);
	}
    }
#endif
}

void
gdev_flush_sfx_shaffle (gdev, xx, yy, ww, hh, pitch, n_div)
     gdev_t gdev;
     integer_t xx, yy, ww, hh;
     integer_t pitch;
     integer_t n_div;
{
  extern GC gcsfx;
  Display *display = gdev->x11.g.display;
  Window window = gdev->x11.g.window;
  Window hidden = gdev->x11.hidden;
  integer_t width = ww;
  integer_t height = hh;
  integer_t pitch_x = width / pitch;
  integer_t pitch_y = height / n_div;
  integer_t count;
  integer_t *randarray, i;

  if (pitch_x == 0)
    pitch_x = 1;
  if (pitch_y == 0)
    pitch_y = 1;

  count = pitch * n_div;
  randarray = (integer_t *) xmalloc (count * sizeof (integer_t));
  for (i = 0; i < count; i++)
    {
      randarray[i] = i;
    }
  shaffle (randarray, count);

  for (i = 0; i < count; i++)
    {
      integer_t x, y, val;
      val = randarray[i],

	x = val / n_div;
      y = (val - (x * n_div)) % pitch;
      x = x * pitch_x;
      y = y * pitch_y;

      XCopyArea (display, hidden, window, gcsfx,
		 xx + x, yy + y, pitch_x, pitch_y, xx + x, yy + y);
      if (i % 5 == 0)
	{
	  XFlush (display);
	}
    }
  xfree ((void *)randarray);

  XSync (display, 1);
  gdev_flush (gdev);
}

void
gdev_flush_sfx_doubleblind (gdev, xx, yy, ww, hh, pitch, n_div)
     gdev_t gdev;
     integer_t xx, yy, ww, hh;
     integer_t pitch;
     integer_t n_div;
{
}
