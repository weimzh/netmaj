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
 * This file is part of my project
 * 
 * pixmapfr.c -- 
 * 
 * $Id: pixmapfr.c,v 1.2 1996/07/31 22:47:50 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Aug 24 00:29:39 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Fri Aug 25 22:53:21 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/pixmapfr.h"


widget_t
pixmapframe_create (p)
     widget_t p;
{
  pixmapframe_pixmappacked (p) = resource_horizontal;
  p->wrap = 1;
  return p;
}

void
pixmapframe_destroy (p)
     widget_t p;
{
}

#define xofs(t)   (_offsetof(struct widget_pixmapframe, t))

static struct configure pixmapframe_configspec[] =
{
  {resource_contentpacked, datatype_char, xofs (pixmappacked), 0},
  {resource_pixmapstart, datatype_int, xofs (pixmapstart), 0},
  {0},
};

void
pixmapframe_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  configure_proc (item, pixmapframe_configspec, resourceid, value);
}

typedef struct pixmaplist
{
  struct pixmaplist *next;
  integer_t width, height;
  integer_t ofs;
  gpixmap_t pixmap;
  integer_t color;
}
 *pixmaplist_t;

static struct pixmaplist *
pixmaplist_new ()
{
  return (struct pixmaplist *) xcalloc (sizeof (struct pixmaplist));
}

static void
pixmaplist_add (w, pixmaplist)
     widget_t w;
     struct pixmaplist *pixmaplist;
{
  pixmapframe_t p = (pixmapframe_t) w;
  integer_t width = 0;
  integer_t height = 0;
  pixmaplist_t q, last = 0;

  if (!widget_pixmapframe_p ((widget_t) p))
    {
      xabort ();
    }
  q = (pixmaplist_t) p->pixmaps;
  while (q)
    {
      width += q->width;
      height += q->height;
      last = q;
      q = q->next;
    }
  if (last)
    {
      last->next = pixmaplist;
      if (pixmapframe_pixmappacked (p) == resource_horizontal)
	{
	  pixmaplist->ofs = width;
	}
      else
	{
	  pixmaplist->ofs = height;
	}
    }
  else
    {
      p->pixmaps = pixmaplist;
    }
}
void
pixmapframe_add_pixmap (w, pixmap, color)
     widget_t w;
     gpixmap_t pixmap;
     integer_t color;
{
  struct pixmaplist *p = pixmaplist_new ();
  if (!widget_pixmapframe_p (w))
    {
      xabort ();
    }

  gdev_gpixmap_get_geometry (w->dev, pixmap, &p->width, &p->height);
  p->pixmap = pixmap;
  p->color = color;
  pixmaplist_add (w, p);
}
void
pixmapframe_display (o)
     widget_t o;
{
  integer_t left, top, orgleft, orgtop;
  struct pixmaplist *p = ((struct widget_pixmapframe *) o)->pixmaps;

  {
    gc_t gc1, gc4;

    if (widget_border_width (o))
      {
	widget_abs_xy (o, 0, 0, &left, &top);
	gc1 = widget_active_dark_gc (o),
	  gc4 = widget_active_light_gc (o);

	_3D_Draw3DFrame (widget_gdev (o),
			 left, top,
			 widget_width (o),
			 widget_height (o),
			 widget_border_width (o),
			 gc1,
			 gc4,
			 gc4);
      }
  }

  widget_abs_xy (o, 0, 0, &orgleft, &orgtop);
  if (pixmapframe_pixmappacked (o) == resource_horizontal)
    {
      orgleft += pixmapframe_pixmapstart (o);
    }
  else
    {
      orgtop += pixmapframe_pixmapstart (o);
    }
  left = orgleft, top = orgtop;
  while (p)
    {
      if (pixmapframe_pixmappacked (o) == resource_horizontal)
	{
	  left = orgleft + p->ofs;
	}
      else
	{
	  top = orgtop + p->ofs;
	}
      gdev_drawpixmap (widget_gdev (o),
		       p->pixmap,
		       widget_active_gc (o),
		       0, 0,
		       widget_width (o),
		       widget_height (o), left, top);
      p = p->next;
    }
}

void
pixmapframe_action (p)
     widget_t p;
{
}

void
pixmapframe_invoke (p)
     widget_t p;
{
}

void
pixmapframe_clear (p)
     widget_t p;
{
}




struct item_type item_type_pixmapframe =
{
  "pixmapframe",
  widget_type_pixmapframe,
  sizeof (struct widget_pixmapframe),
  pixmapframe_create,
  pixmapframe_destroy,
  pixmapframe_configure,
  pixmapframe_display,
  pixmapframe_action,
  pixmapframe_invoke,
  pixmapframe_clear,
};
