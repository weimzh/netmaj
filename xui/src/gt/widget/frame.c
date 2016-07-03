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
 * frame.c --   
 *   
 * $Id: frame.c,v 1.2 1996/07/31 22:47:34 suz Exp $  
 * Author          : NBC02365@niftyserve.or.jp  
 * Created On      : Mon Apr 17 12:28:40 1995  
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Wed Apr 10 23:57:46 1996
 *
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/frame.h"


widget_t
frame_create (p)
     widget_t p;
{
  frame_packed (p) = resource_virtical;
  widget_relief (p) = relief_frame;
  p->wrap = 1;
  return p;
}

void
frame_destroy (p)
     widget_t p;
{
}

#define xofs(t)   (_offsetof(struct widget_frame, t))

static struct configure frame_configspec[] =
{
  {resource_packed, datatype_char, xofs (packed), 0},
  {resource_frame_start, datatype_int, xofs (start), 0},
  {0,},
};

void
frame_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  configure_proc (item, frame_configspec, resourceid, value);
}


void
frame_outline_draw (o)
     widget_t o;
{

  integer_t x, y;
  if (widget_border_width (o))
    {
      widget_abs_xy (o, 0, 0, &x, &y);
    }
}

void
rectanglewidget_display (o)
     widget_t o;
{
  integer_t x, y;
  widget_abs_xy (o, 0, 0, &x, &y);

  if (widget_border_width (o))
    {
      if (widget_relief (o) == relief_raised)
	{
	  _3D_Draw3DRectangle (widget_gdev (o),
			       x, y,
			       widget_width (o),
			       widget_height (o),
			       widget_border_width (o),
			       widget_active_gc (o),
			       widget_active_light_gc (o),
			       widget_active_dark_gc (o));
	}
      else if (widget_relief (o) == relief_sunken)
	{
	  _3D_Draw3DRectangle (widget_gdev (o),
			       x, y,
			       widget_width (o),
			       widget_height (o),
			       widget_border_width (o),
			       widget_active_dark_gc (o),
			       widget_active_dark_gc (o),
			       widget_active_light_gc (o));
	}
      else if (widget_relief (o) == relief_frame)
	{
	  _3D_Draw3DFrame (widget_gdev (o),
			   x, y,
			   widget_width (o),
			   widget_height (o),
			   widget_border_width (o),
			   widget_active_gc (o),
			   widget_active_light_gc (o),
			   widget_active_dark_gc (o));
	}
      else
	{
	  _3D_DrawFrame (widget_gdev (o),
			 x, y,
			 widget_width (o),
			 widget_height (o),
			 widget_border_width (o),
			 widget_active_gc (o),
			 widget_active_light_gc (o),
			 widget_active_dark_gc (o));
	}
    }
  else
    {
      gdev_fill_rectangle (widget_gdev (o),
			   widget_active_gc (o),
			   x, y,
			   widget_width (o), widget_height (o));

    }
}

void
frame_display (p)
     widget_t p;
{
  rectanglewidget_display (p);
}

void
frame_action (p)
     widget_t p;
{
}

void
frame_invoke (p)
     widget_t p;
{
}

void
frame_clear (p)
     widget_t p;
{
}




struct item_type item_type_frame =
{
  "frame",
  widget_type_frame,
  sizeof (struct widget_frame),
  frame_create,
  frame_destroy,
  frame_configure,
  frame_display,
  frame_action,
  frame_invoke,
  frame_clear,
  0,
};


void
widget_pack (o)
     widget_t o;
{
  integer_t height;
  integer_t width;
  widget_t child;
  frame_t w = (frame_t) o;
  widget_t np;
  integer_t border_width;
  if (!w)
    {
      return;
    }
  if (!w->packed)
    {
      return;
    }
  for (np = w->child; np; np = np->next)
    {
      widget_pack (np);
    }
  border_width = widget_border_width (o);
  switch (w->packed)
    {
    case resource_virtical:
      height = border_width + frame_start (o);
      width = border_width;
      for (child = w->child; child; child = child->next)
	{
	  child->left = border_width;
	  child->top = height;
	  height += child->height;
	  if (width < (child->width + border_width))
	    {
	      width = (child->width + border_width);
	    }
	}
      break;
    case resource_horizontal:
      width = border_width + frame_start (o);
      height = border_width;
      for (child = w->child; child; child = child->next)
	{
	  child->left = width;
	  child->top = border_width;
	  width += child->width;
	  if (height < (child->height + border_width))
	    {
	      height = (child->height + border_width);
	    }
	}
      break;
    default:
      xabort ();
    }
  height += border_width;
  width += border_width;
  if (w->wrap)
    {
      w->height = height;
      w->width = width;
    }
}


void
widget_fix (o)
     widget_t o;
{
  widget_t np;
  integer_t l, t;

  widget_abs_xy (o, 0, 0, &l, &t);
  widget_abs_x (o) = l;
  widget_abs_y (o) = t;
  for (np = o->child; np; np = np->next)
    {
      widget_fix (np);
    }
  widget_fixed (o) = 1;
}


#include <stdio.h>

void
widget_print_geometry (o, indent)
     widget_t o;
     int indent;
{
  widget_t child;
  int i;

  for (i = 0; i < indent; i++)
    {
      fprintf (stdout, "  ");
    }
  fprintf (stdout, "#%x #%x %d %d %d %d %d %s\n",
	   (unsigned) o, (unsigned) o->parent,
   o->left, o->top, o->width, o->height, o->packed, widget_type_string (o));

  for (child = o->child; child; child = child->next)
    {
      widget_print_geometry (child, indent + 1);
    }
  fflush (stdout);
}

void
pixmapframe_free (p)
     widget_t p;
{
  ;
}
