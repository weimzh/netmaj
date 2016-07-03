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
 * This file is part of xnetmaj
 * 
 * _3d.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:02:06 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"

void
_3D_Draw3DRectangle (dev,
		     x, y,
		     width,
		     height,
		     border_width,
		     mainGC,
		     topGC,
		     bottomGC)
     gdev_t dev;
     integer_t x, y, width, height, border_width;
     gc_t mainGC, topGC, bottomGC;
{
  GDEVPOINT points[7];

  gdev_fill_rectangle (dev, mainGC, x, y, width, height);
#if X11

  gdev_fill_rectangle (dev, bottomGC, x, y + height - border_width,
		       (unsigned int) width, (unsigned int) border_width);
  gdev_fill_rectangle (dev, bottomGC, x + width - border_width, y,
		       (unsigned int) border_width, (unsigned int) height);

  points[0].x = points[1].x = points[6].x = x;
  points[0].y = points[6].y = y + height;
  points[1].y = points[2].y = y;
  points[2].x = x + width;
  points[3].x = x + width - border_width;
  points[3].y = points[4].y = y + border_width;
  points[4].x = points[5].x = x + border_width;
  points[5].y = y + height - border_width;
  gdev_polygon (dev, topGC, points, 7);

#else

  points[0].x = x + width - border_width;
  points[0].y = y + height - border_width;
  points[1].x = x + border_width;
  points[1].y = y + height - border_width;
  points[2].x = x;
  points[2].y = y + height;
  points[3].x = x + width;
  points[3].y = y + height;
  points[4].x = x + width;
  points[4].y = y;
  points[5].x = x + width - border_width;
  points[5].y = y + border_width;
  points[6].x = x + width - border_width;
  points[6].y = y + height - border_width;

  gdev_polygon (dev, bottomGC, points, 7);
  points[0].x = x;
  points[0].y = y;
  points[1].x = x;
  points[1].y = y + height;
  points[2].x = x + border_width;
  points[2].y = y + height - border_width;
  points[3].x = x + border_width;
  points[3].y = y + border_width;
  points[4].x = x + width - border_width;
  points[4].y = y + border_width;
  points[5].x = x + width - border_width;
  points[5].y = y + border_width;
  points[6].x = x + width;
  points[6].y = y;
  gdev_polygon (dev, topGC, points, 7);
#endif
}

void
_3D_Draw3DFrame (dev,
		 left, top,
		 width,
		 height,
		 border_width,
		 mainGC, topGC, bottomGC)
     gdev_t dev;
     integer_t left, top, width, height, border_width;
     gc_t mainGC, topGC, bottomGC;
{
  integer_t x, y;
  x = left, y = top;

  gdev_fill_rectangle (dev, bottomGC, x, y + height - border_width,
		       (unsigned int) width, (unsigned int) border_width);
  gdev_fill_rectangle (dev, bottomGC, x + width - border_width, y,
		       (unsigned int) border_width, (unsigned int) height);
  gdev_fill_rectangle (dev, bottomGC, x, y,
		       (unsigned int) width, (unsigned int) border_width);
  gdev_fill_rectangle (dev, bottomGC, x, y,
		       (unsigned int) border_width, (unsigned int) height);
  gdev_fill_rectangle (dev, topGC,
		       x + border_width - 1,
		       y + border_width - 1,
		       width - border_width * 2 + 2,
		       height - border_width * 2 + 2);
  gdev_fill_rectangle (dev, mainGC, x + border_width, y + border_width,
		       (unsigned int) width - border_width * 2,
		       (unsigned int) height - border_width * 2);

}

void
_3D_DrawFrame (dev,
	       x, y,
	       width,
	       height,
	       border_width,
	       mainGC,
	       topGC,
	       bottomGC)
     gdev_t dev;
     integer_t x, y, width, height, border_width;
     gc_t mainGC, topGC, bottomGC;
{
  gdev_fill_rectangle (dev, mainGC, x, y,
		       (unsigned int) width, (unsigned int) height);
  gdev_fill_rectangle (dev, bottomGC, x, y + height - border_width,
		       (unsigned int) width, (unsigned int) border_width);
  gdev_fill_rectangle (dev, bottomGC, x + width - border_width, y,
		       (unsigned int) border_width, (unsigned int) height);
  gdev_fill_rectangle (dev, bottomGC, x, y,
		       (unsigned int) width, (unsigned int) border_width);
  gdev_fill_rectangle (dev, bottomGC, x, y,
		       (unsigned int) border_width, (unsigned int) height);
}
