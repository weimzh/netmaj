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
 * x11spr.c -- 
 * 
 * $Id: x11spr.c,v 1.2 1996/07/31 22:47:01 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:55:21 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:55:24 1996
 * description:
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/dev/x11gdev.h"

extern GC gc;

void
gdev_draw_sprite (gdev, x, y, sprite)
     gdev_t gdev;
     integer_t x, y;
     gsprite_t sprite;
{
  Display *display = gdev->x11.g.display;
  Window window = gdev->x11.g.window;
  Pixmap pixmap = sprite->p;
  Pixmap mask = sprite->pM;
  int w = sprite->width;
  int h = sprite->height;

  /* Set to dest x and y clip origin */
  XSetClipOrigin (display, gc, x, y);

  /* Set the clipping mask */
  XSetClipMask (display, gc, mask);

  XCopyArea (display, pixmap, window, gc, 0, 0, w, h, x, y);

  /* Unset clip (or add a clip gc) */
  XSetClipMask (display, gc, None);

  sprite->x = x;
  sprite->y = y;
}
