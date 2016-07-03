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
 * x11rend.c -- 
 * 
 * $Id: x11rend.c,v 1.2 1996/07/31 22:46:58 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:55:03 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:55:09 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/dev/x11gdev.h"

void
gdev_renderpixmap (dev, p, pM, gc, src_x, src_y, w, h, x, y)
     gdev_t dev;
     gpixmap_t p, pM;
     gc_t gc;
     integer_t x, y;
     integer_t w, h, src_x, src_y;
{
  Display *display = dev->x11.g.display;
  Pixmap pixmap = p;
  Pixmap mask = pM;
  GC x11gc = dev->x11.gc;
  Drawable dest = dev->x11.hidden;

  /* Set to dest x and y clip origin */
  XSetClipOrigin (display, x11gc, x, y);

  /* Set the clipping mask */
  XSetClipMask (display, x11gc, mask);

  XCopyArea (display, pixmap, dest, x11gc, src_x, src_y, w, h, x, y);

  /* Unset clip (or add a clip gc) */
  XSetClipMask (display, x11gc, None);
}
