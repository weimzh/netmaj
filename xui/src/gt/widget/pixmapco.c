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
 * pixmapcont.c --    
 *    
 * $Id: pixmapco.c,v 1.2 1996/07/31 22:47:48 suz Exp $   
 * Author          : NBC02365@niftyserve.or.jp   
 * Created On      : Thu Apr 13 19:48:50 1995   
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Apr 11 00:00:43 1996
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/pixmapco.h"
#include "gt/widget/private.h"

#define D(a)

widget_t
pmcontainer_create (p)
     widget_t p;
{
  pmcontainer_packdirect (p) = resource_horizontal;
  pmcontainer_content (p) = 0;
  pmcontainer_n_contents (p) = 0;
  widget_configure (p, resource_max_contents, 2);
  p->wrap = 0;
  return p;
}
void
pmcontainer_destroy (p)
     widget_t p;
{
}

#define xofs(t)   (_offsetof(struct widget_pmcontainer, t))

static struct configure pmcontainer_configspec[] =
{
  {resource_packdirect, datatype_char, xofs (packdirect), 0},
  {resource_margin, datatype_int, xofs (margin), 0},
  {resource_max_contents, datatype_int, xofs (max_contents), 0},
  {resource_pixmapstart, datatype_int, xofs (pixmapstart), 0},
  {0},
};


void
pmcontainer_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  if (resourceid == resource_max_contents)
    {
      pmcontainer_t cp = (pmcontainer_t) item;
      integer_t size = ((integer_t) value) * sizeof (struct content);
      cp->max_contents = ((integer_t) value);

      if (cp->content)
	{
	  cp->content = xrealloc (cp->content, size);
	}
      else
	{
	  cp->content = xcalloc (size);
	}
      return;
    }
  configure_proc (item, pmcontainer_configspec, resourceid, value);
}

void pmcontainer_rearrange_with_margin(widget_t w, int newmargin)
{
  pmcontainer_t p;
  content_t val;
  int i;
  p = (pmcontainer_t) w;
/*  assert (widget_pmcontainer_p (w)); */
  for (i = 0; i < p->n_contents; i++) {
    val = p->content;
    val[i].margin = newmargin;
  }
}

void
pmcontainer_add_withofs (w, content, mask, color, paramofs)
     widget_t w;
     gpixmap_t content;
     gpixmap_t mask;
     integer_t paramofs, color;
{
  pmcontainer_t p;
  content_t val;
  integer_t width;
  integer_t height;
  integer_t ofs;

retry:
  p = (pmcontainer_t) w;
  val = p->content;
/*  assert (widget_pmcontainer_p (w));*/

  if (p->max_contents <= p->n_contents + 1)
    {
      widget_configure (w, resource_max_contents, p->max_contents * 2);
      goto retry;
    }
  if (!val)
    {
      widget_configure (w, resource_max_contents, 2);
    }

  gdev_gpixmap_get_geometry (w->dev, content, &width, &height);
  if (paramofs == -1)
    {
      if (p->n_contents == 0)
	{
	  ofs = pmcontainer_pixmapstart (w);
	}
      else
	{
	  integer_t margin = val[p->n_contents - 1].margin;
	  val = p->content;
	  ofs = val[p->n_contents - 1].ofs + margin;
	  if (pmcontainer_packdirect (p) == resource_horizontal)
	    {
	      ofs += val[p->n_contents - 1].width;
	    }
	  else
	    {
	      ofs += val[p->n_contents - 1].height;
	    }
	}
    }
  else
    {
      ofs = paramofs;
    }
  val = p->content;
  val[p->n_contents].content = content;
  val[p->n_contents].ofs = ofs;
  val[p->n_contents].width = width;
  val[p->n_contents].height = height;
  val[p->n_contents].color = color;
  val[p->n_contents].mask = mask;
  val[p->n_contents].margin = 0;
  p->n_contents++;
}

void
pmcontainer_add (w, content, mask, color)
     widget_t w;
     gpixmap_t content;
     gpixmap_t mask;
     integer_t color;
{
  pmcontainer_add_withofs (w, content, mask, color, -1);
}

void
pmcontainer_add_withmargin (w, content, mask, color, margin)
     widget_t w;
     gpixmap_t content;
     gpixmap_t mask;
     integer_t margin, color;
{
  integer_t ofs;
  pmcontainer_t p;
  content_t val;

  p = (pmcontainer_t) w;
  val = p->content;

  if (p->n_contents == 0)
    {
      ofs = margin;
    }
  else if (pmcontainer_packdirect (p) == resource_horizontal) 
    {
      ofs = val[p->n_contents - 1].ofs + val[p->n_contents - 1].width + margin;
    }
  else
    {
      ofs = val[p->n_contents - 1].ofs + val[p->n_contents - 1].height + margin;
    }
  pmcontainer_add_withofs (w, content, mask, color, ofs);
}

void
pmcontainer_set_margin (w, margin)
     widget_t w;
     integer_t margin;
{
  pmcontainer_t p;
  content_t val;

  p = (pmcontainer_t) w;
  val = p->content;
  if (p->n_contents == 0)
    {
      return;
    }
  val[p->n_contents - 1].margin = margin;
}


void
pmcontainer_clear (w)
     widget_t w;
{
  pmcontainer_t p;
  content_t val;

  if (!w) return;
  p = (pmcontainer_t) w;
  val = p->content;
  memset ((void *) val, 0, sizeof (struct content) * p->max_contents);
  p->n_contents = 0;
  pmcontainer_pixmapstart (p) = 0;
}

void
pmcontainer_clear_frame (o)
     widget_t o;
{
  integer_t left, top;

  pmcontainer_clear (o);

  widget_abs_xy (o, 0, 0, &left, &top);
  gdev_fill_rectangle (widget_gdev (o),
		       widget_active_gc (o),
		       left, top, widget_width (o), widget_height (o));
}

void
pmcontainer_display (o)
     widget_t o;
{
  integer_t left, top, orgleft, orgtop;

  widget_abs_xy (o, 0, 0, &left, &top);

  gdev_fill_rectangle (widget_gdev (o),
		       widget_active_gc (o),
		       left, top, widget_width (o), widget_height (o));

  orgleft = left, orgtop = top;
  if (pmcontainer_packdirect (o) == resource_horizontal)
    {
      orgleft += pmcontainer_margin (o);
    }
  else
    {
      orgtop += pmcontainer_margin (o);
    }
  left = orgleft, top = orgtop;

  {
    integer_t i;
    pmcontainer_t cp = (pmcontainer_t) o;
    content_t p;
    for (i = 0; i < cp->n_contents; i++)
      {
	content_t np;
	np = cp->content;
	p = &np[i];

	if (pmcontainer_packdirect (o) == resource_horizontal)
	  {
	    left = orgleft + p->ofs;
	  }
	else
	  {
	    top = orgtop + p->ofs;
	  }
	if (p->mask)
	  {
	    gdev_renderpixmap (widget_gdev (o),
			       p->content,
			       p->mask,
			       widget_active_gc (o),
			       0, 0,
			       p->width,
			       p->height, left, top);
	  }
	else
	  {
	    gdev_drawpixmap (widget_gdev (o),
			     p->content,
			     widget_active_gc (o),
			     0, 0,
			     p->width,
			     p->height, left, top);

	  }
      }
  }
  D (widget_sync (o));
}
void
pmcontainer_action (p)
     widget_t p;
{
}
void
pmcontainer_invoke (p, arg)
     widget_t p;
     struct callbackarg *arg;

{
  widget_call_callback (p, resource_buttonpress_callback, arg);
  widget_sync (p);
}




struct item_type item_type_pmcontainer =
{
  "pmcontainer",
  widget_type_pmcontainer,
  sizeof (struct widget_pmcontainer),
  pmcontainer_create,
  pmcontainer_destroy,
  pmcontainer_configure,
  pmcontainer_display,
  pmcontainer_action,
  pmcontainer_invoke,
  pmcontainer_clear,
};
