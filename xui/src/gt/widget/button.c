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
 * button.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:01:57 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/button.h"

#define button_text(p)        ((struct widget_button *)p)->text
#define button_textcolor(p)   ((struct widget_button *)p)->textcolor
#define button_textfont(p)    ((struct widget_button *)p)->textfont
#define button_pixmap(p)      ((struct widget_button *)p)->pixmap
#define button_pixmap_mask(p) ((struct widget_button *)p)->pixmap_mask

static widget_t
_button_create (p)
     widget_t p;
{
  extern integer_t white;
  widget_set_geometry (p, 32, 50);
  widget_configure (p, resource_border_width, 2);
  widget_configure (p, resource_textcolor, white);
  return p;
}

static void
_button_destroy (o)
     widget_t o;
{
}

#define xofs(t)   (_offsetof(struct widget_button, t))

static struct configure button_configspec[] =
{
  {resource_textcolor, datatype_int, xofs (textcolor), 0},
  {resource_textfont, datatype_int, xofs (textfont), 0},
  {resource_pixmap, datatype_pixmap, xofs (pixmap), 0},
  {resource_pixmap_mask, datatype_pixmap, xofs (pixmap_mask), 0},
  {0,},
};

static void
_button_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  int len;
  if (resourceid == resource_text)
    {
      if (button_text (item))
	{
	  xfree ((void *) button_text (item));
	}
      len = strlen (value) + 1;
      button_text (item) = memcpy (xmalloc (len), value, len);
    }
  configure_proc (item, button_configspec, resourceid, value);
}

static void
_button_display (o, relief)
     widget_t o;
     integer_t relief;
{
  integer_t x, y;
  integer_t ofs;
  widget_abs_xy (o, 0, 0, &x, &y);

  if (relief != relief_sunken)
    {				/* model_init */
      _3D_Draw3DRectangle (widget_gdev (o),
			   x, y,
			   widget_width (o),
			   widget_height (o),
			   widget_border_width (o),
			   widget_active_gc (o),
			   widget_active_light_gc (o),
			   widget_active_dark_gc (o));
      ofs = 0;

    }
  else
    {
      _3D_Draw3DRectangle (widget_gdev (o),
			   x, y,
			   widget_width (o),
			   widget_height (o),
			   widget_border_width (o),
			   widget_active_dark_gc (o),
			   widget_active_dark_gc (o),
			   widget_active_light_gc (o));
      ofs = 1;
    }
  if (button_text (o))
    {
      gdev_drawtext (widget_gdev (o),
		     x + widget_border_width (o) + ofs,
		     y + widget_border_width (o) + ofs,
		     button_textcolor (o), button_textfont (o),
		     button_text (o), strlen (button_text (o)));
    }
  if (button_pixmap (o))
    {
      integer_t w, h;
      gdev_gpixmap_get_geometry (widget_gdev (o), button_pixmap (o), &w, &h);

      if (button_pixmap_mask (o))
	{
	  gdev_renderpixmap (widget_gdev (o),
			     button_pixmap (o),
			     button_pixmap_mask (o),
			     widget_active_gc (o),
			     0, 0,
			     w,
			     h,
			     x + widget_border_width (o) + ofs,
			     y + widget_border_width (o) + ofs);

	}
      else
	{
	  gdev_drawpixmap (widget_gdev (o),
			   button_pixmap (o),
			   widget_active_gc (o),
			   0, 0,
			   w,
			   h,
			   x + widget_border_width (o) + ofs,
			   y + widget_border_width (o) + ofs);
	}
    }
}

static void
button_display (o)
     widget_t o;
{
  _button_display (o, relief_raised);
}

static void
_button_action (o)
     void *o;
{

}


static void
_button_invoke (o, arg)
     widget_t o;
     struct callbackarg *arg;
{
  _button_display (o, relief_sunken);
  widget_flush (o);
  widget_sync (o);
#ifndef X11
  sleep (2);
#endif

  widget_call_callback (o, resource_buttonpress_callback, arg);

  if (o->map)
    {
      _button_display (o, relief_raised);
      widget_flush (o);
      widget_sync (o);
    }
}

static void
_button_clear (o)
     widget_t o;
{
}

struct item_type item_type_button =
{
  "button",
  widget_type_button,
  sizeof (struct widget_button),
  (void *) _button_create,
  (void *) _button_destroy,
  (void *) _button_configure,
  (void *) button_display,
  (void *) _button_action,
  (void *) _button_invoke,
  (void *) _button_clear,
  0,
};
