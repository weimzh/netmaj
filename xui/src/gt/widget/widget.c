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
 * widget.c -- 
 * 
 * $Id: widget.c,v 1.2 1996/07/31 22:48:01 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 10:59:57 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Apr 11 02:15:13 1996
 * 
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"


integer_t widget_call_callback ();

static widget_t root_widget;


void
widget_display (w)
     widget_t w;
{
  widget_t child;

  if (!w || !w->map || w->invisible)
    return;

  (*(w->type->proc.display)) (w);

  for (child = w->child; child; child = child->next)
    {
      widget_display (child);
    }
}



static widget_t grabed;
void
widget_grab (w)
     widget_t w;
{
  grabed = w;
}
void
widget_ungrab ()
{
  grabed = 0;
}

widget_t
widget_pointed (parent, x, y, px, py)
     widget_t parent;
     integer_t x, y;
     integer_t *px, *py;
{
  widget_t child = parent->child;
  integer_t xx, yy, width, height;

  if (grabed)
    {
      return grabed;
    }
  for (; child; child = child->next)
    {
      if (!child->map)
	{
	  continue;
	}
      width = child->width;
      height = child->height;

      if ((child->left <= x && child->left + width >= x)
	  && (child->top <= y && child->top + height >= y))
	{
	  ;
	}
      else
	{
	  continue;
	}
      xx = x - child->left;
      yy = y - child->top;
      if (child->child)
	{
	  widget_t p;
	  integer_t ppx, ppy;

	  p = widget_pointed (child, xx, yy, &ppx, &ppy);
	  if (p)
	    {
	      *px = ppx, *py = ppy;
	      return p;
	    }
	}
      *px = xx, *py = yy;
      return child;
    }
  return 0;
}

integer_t
widget_map_p (parent)
     widget_t parent;
{
  return parent->map;
}


void
widget_map (parent)
     widget_t parent;
{
  widget_t child;
  parent->map = 1;
  child = parent->child;
  while (child)
    {
      widget_map (child);
      child = child->next;
    }
}

void
widget_unmap (parent)
     widget_t parent;
{
  widget_t child;
  if (!parent) return;

  parent->map = 0;
  child = parent->child;
  while (child)
    {
      widget_unmap (child);
      child = child->next;
    }
}

void
widget_xunmap (parent)
     widget_t parent;
{
  widget_t pp = parent->parent;
  widget_unmap (parent);
  if (pp)
    {
      widget_display (pp);
    }
}





widget_t
widget_init (w, h)
     integer_t w, h;
{
  gdev_init (w, h);
  item_init ();

  widget_palette_init ();
  root_widget = widget_create ("toplevel", 0);
  widget_set_geometry (root_widget, w, h);
  return root_widget;
}


void
widget_event_discard (w)
     widget_t w;
{
  gdev_event_discard (w->dev);
  gdev_sync (w->dev, 1);
}

#if X11
integer_t
widget_eventsqueued (w)
     widget_t w;
{
  return gdev_have_event (w->dev);
}
#endif

void
widget_event_loop ()
{
  for (;;)
    {
      _widget_handle_event ();
    }
}


typedef struct callbackproc
{
  struct callbackproc *next;
  integer_t type;
    integer_t (*func) ();
  void *arg;
}
 *callbackproc_t;


static callbackproc_t
create_callbackproc ()
{
  return (callbackproc_t) xcalloc (sizeof (struct callbackproc));
}

void
_widget_add_callback (widget, resource, func, arg)
     widget_t widget;
     integer_t resource;
integer_t (*func) ();
     void *arg;
{
  callbackproc_t list = widget->callbackproc;
  callbackproc_t p = create_callbackproc ();
  switch (resource)
    {
    case resource_buttonpress_callback:
    case resource_motion_callback:
    case resource_buttonrelease_callback:
    case resource_keypress_callback:
      break;
    default:
      error ("add callback: unknown resource");
    }
  p->func = func;
  p->type = resource;
  p->arg = arg;
  p->next = list;
  widget->callbackproc = p;
}


integer_t
widget_call_callback (widget, resource, callbackarg)
     widget_t widget;
     integer_t resource;
     void *callbackarg;
{
  callbackproc_t list;

next:
  list = widget->callbackproc;
  while (list)
    {
      if (list->type == resource)
	{
	  return (*(list->func)) (widget, list->arg, callbackarg);
	}
      list = list->next;
    }
  if (widget->transparent)
    {
      widget = widget->parent;
      if (!widget)
	{
	  return 0;
	}
      goto next;
    }
  return 0;
}

void
widget_force_flush (o)
     widget_t o;
{
  integer_t left, top;
/*  if (!o->map || o->invisible) return; */

  widget_abs_xy (o, 0, 0, &left, &top);
  gdev_flush_area (o->dev, left, top,
		   widget_width (o), widget_height (o));
}

void
widget_flush (o)
     widget_t o;
{
  if (!o->map || o->invisible) return;
  widget_force_flush (o);
}

void
widget_root_flush (o)
     widget_t o;
{
  extern widget_t root_widget;
  widget_force_flush (root_widget);
}

void
widget_sync (o)
     widget_t o;
{
  gdev_sync (o->dev, 0);
}

void
widget_set_location(w, x, y)
     widget_t w;
     integer_t x, y;
{
  w->left = x;
  w->top = y;
}



void
widget_set_geometry (w, width, height)
     widget_t w;
     integer_t width, height;
{
  w->width = width;
  w->height = height;
}

void
widget_get_geometry (w, width, height)
     widget_t w;
     integer_t *width, *height;
{
  *width = w->width;
  *height = w->height;
}


void
wigdet_set_pack_pixmap (item, pixmap)
     widget_t item;
     gpixmap_t pixmap;
{
  integer_t x, y;

  widget_configure (item, resource_pixmap, pixmap);
  gdev_drawable_get_geometry (widget_gdev (item), pixmap, &x, &y);
  widget_set_geometry (item, x, y);

  widget_pack (item->parent);
}


void
widget_abs_xy (o, x, y, l, t)
     widget_t o;
     integer_t x, y;
     integer_t *l, *t;
{
  widget_t parent;
  integer_t val1;
  integer_t val2;

  if (widget_fixed (o))
    {
      *l = widget_abs_x (o);
      *t = widget_abs_y (o);
      return;
    }
  parent = o->parent;
  val1 = o->left;
  val2 = o->top;
  while (parent)
    {
      val1 += parent->left;
      val2 += parent->top;
      parent = parent->parent;
    }
  *l = val1 + x;
  *t = val2 + y;
}



void
widget_sleep (w)
     widget_t w;
{
  sleep (1);
}

void
widget_set_color_change (w, fg, bg, inactivefg, inactivebg)
     widget_t w;
     integer_t fg, bg, inactivefg, inactivebg;
{
  model_change (w->dev, &w->active, fg, bg);
  model_change (w->dev, &w->inactive, inactivefg, inactivebg);
}

void
widget_set_background (w, pixmap)
     widget_t w;
     gpixmap_t pixmap;
{
  gdev_set_background (w->dev, pixmap);
}

void
widget_dev_map (w)
     widget_t w;
{
  gdev_map (w->dev);
}

void
widget_change_models (root, fg)
     widget_t root;
     integer_t fg;
{
  extern integer_t black;
  while (root)
    {
      if (!widget_cannot_change_model (root))
	widget_model_change (root, fg, black, black, black);
      if (root->child)
	{
	  widget_change_models (root->child, fg);
	}
      root = root->next;
    }
}
