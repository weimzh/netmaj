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
 * This file is part of my project
 * 
 * widsfx.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Sat May 13 19:18:01 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */
#include "gt/widget/widget.h"
#include "gt/widget/local.h"

void
widget_flush_sfx_shatter (w, a, b)
     widget_t w;
     integer_t a, b;
{
  integer_t left, top;
  widget_abs_xy (w, 0, 0, &left, &top);
  gdev_flush_sfx_shatter (w->dev, left, top, widget_width (w), widget_height (w), a, b);
}

void
widget_flush_sfx_blind (w, a, b)
     widget_t w;
     integer_t a, b;
{
  integer_t left, top;
  widget_abs_xy (w, 0, 0, &left, &top);
  gdev_flush_sfx_blind (w->dev, left, top, widget_width (w), widget_height (w), a, b);
}

void
widget_flush_sfx_zoom (w, a, b)
     widget_t w;
     integer_t a, b;
{
  integer_t left, top;
  widget_abs_xy (w, 0, 0, &left, &top);
  gdev_flush_sfx_zoom (w->dev, left, top, widget_width (w), widget_height (w), a, b);
}


void
widget_flush_sfx_shaffle (w, a, b)
     widget_t w;
     integer_t a, b;
{
  integer_t left, top;
  widget_abs_xy (w, 0, 0, &left, &top);
  gdev_flush_sfx_shaffle (w->dev, left, top, widget_width (w), widget_height (w), a, b);
}

void
widget_flush_sfx_doubleblind (w, a, b)
     widget_t w;
     integer_t a, b;
{
  integer_t left, top;
  widget_abs_xy (w, 0, 0, &left, &top);
  gdev_flush_sfx_doubleblind (w->dev, left, top, widget_width (w), widget_height (w), a, b);
}
