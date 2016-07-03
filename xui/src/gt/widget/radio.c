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
 * radio.c -- 
 * 
 * $Id: radio.c,v 1.2 1996/07/31 22:47:53 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:57:35 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:57:40 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/radio.h"

#define radio_value(w)  (((struct widget_radio *)(w))->value)
#define radio_gc(w)     (((struct widget_radio *)(w))->gc)
#define radiobutton_width(w)  (((struct widget_radio *)(w))->radiobutton_width)
#define radiobutton_height(w) (((struct widget_radio *)(w))->radiobutton_height)
#define radiobutton_border_width(w) (((struct widget_radio *)(w))->radiobutton_border_width)

extern integer_t white, black;

static widget_t
_radio_create (p)
     widget_t p;
{
  widget_set_geometry (p, 20, 20);
  radiobutton_width (p) = 16;
  radiobutton_height (p) = 16;
  radiobutton_border_width (p) = 2;

  radio_gc (p) = gc_new ();
  gc_set_foreground (radio_gc (p), white);
  gc_set_background (radio_gc (p), black);
  return p;
}

static void
_radio_destroy (o)
     widget_t o;
{
}

#define xofs(t)   (_offsetof(struct widget_radio, t))

static struct configure radio_configspec[] =
{
  {resource_radiobutton_value, datatype_int, xofs (value), 0},
  {resource_radiobutton_width, datatype_int, xofs (radiobutton_width), 0},
  {resource_radiobutton_height, datatype_int, xofs (radiobutton_height), 0},
  {resource_radiobutton_border_width, datatype_int, xofs (radiobutton_border_width), 0},
  {0,},
};

static void
_radio_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  configure_proc (item, radio_configspec, resourceid, value);
}

extern integer_t white, black, red;
static void
_radio_display (o)
     widget_t o;
{
  integer_t x, y, sx, sy;
  integer_t color;
  integer_t border_width = radiobutton_border_width (o);
  sx = (widget_width (o) - radiobutton_width (o)) / 2;
  sy = (widget_height (o) - radiobutton_height (o)) / 2;

  widget_abs_xy (o, 0, 0, &x, &y);

  if (sx != 0 || sy != 0
      || widget_width (o) != radiobutton_width (o)
      || widget_height (o) != radiobutton_height (o))
    {
      rectanglewidget_display (o);
    }
  _3D_Draw3DRectangle (widget_gdev (o),
		       x + sx, y + sy,
		       radiobutton_width (o),
		       radiobutton_height (o),
		       radiobutton_border_width (o),
		       widget_active_dark_gc (o),
		       widget_active_dark_gc (o),
		       widget_active_light_gc (o));
  if (radio_value (o) != 0)
    {
      color = red;
    }
  else
    {
      color = black;
    }
  gc_set_foreground (radio_gc (o), color);
  gdev_fill_rectangle (widget_gdev (o),
		       radio_gc (o),
		       x + sx + border_width,
		       y + sy + border_width,
		       radiobutton_width (o) - border_width * 2,
		       radiobutton_height (o) - border_width * 2);
}

static void
_radio_action (o)
     void *o;
{
}


static void
_radio_invoke (o, arg)
     widget_t o;
     struct callbackarg *arg;
{
  radio_value (o) = !radio_value (o);
  _radio_display (o);
  widget_flush (o);
}

static void
_radio_clear (o)
     widget_t o;
{
}

integer_t
radiobutton_get_value (o)
     widget_t o;
{
  return radio_value (o);
}

struct item_type item_type_radio =
{
  "radiobutton",
  widget_type_radio,
  sizeof (struct widget_radio),
  (void *) _radio_create,
  (void *) _radio_destroy,
  (void *) _radio_configure,
  (void *) _radio_display,
  (void *) _radio_action,
  (void *) _radio_invoke,
  (void *) _radio_clear,
  0,
};
