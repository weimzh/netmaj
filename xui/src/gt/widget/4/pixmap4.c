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
 * pixmapcont.c --    
 *    
 * Author          : NBC02365@niftyserve.or.jp   
 * Created On      : Thu Apr 13 19:48:50 1995   
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/pixmapco.h"
#include "gt/widget/4/pixmap4.h"
#include "gt/widget/private.h"

#define D(a)

widget_t
pmcontainer4_create (p)
     widget_t p;
{
  pmcontainer_create(p);
  pmcontainer4_direction(p) = 0;
  pmcontainer4_drawdirection(p) = 0;
  return p;
}


#define xofs(t)   (_offsetof(struct widget_pmcontainer4, t))

static struct configure pmcontainer4_configspec[] =
{
  {resource_direction, datatype_char, xofs (direction), 0},
  {resource_drawdirection, datatype_char, xofs (drawdirection), 0},
  {0},
};

void
pmcontainer4_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  if (!configure_proc (item, pmcontainer4_configspec, resourceid, value)) {
    pmcontainer_configure(item, resourceid, value);
  }
}


void
pmcontainer4_display (o)
     widget_t o;
{
  integer_t left, top, orgleft, orgtop, start, stop, inc;

  widget_abs_xy (o, 0, 0, &left, &top);
  gdev_fill_rectangle (widget_gdev (o),
		       widget_active_gc (o),
		       left, top, widget_width (o), widget_height (o));
  orgleft = left, orgtop = top;
  switch (pmcontainer4_direction (o))
    {
    case 0:
      orgleft = orgleft + pmcontainer_margin (o);
      break;
    case 1:
      orgtop = orgtop + widget_height(o) - pmcontainer_margin (o);
      break;
    case 2:
      orgleft = orgleft + widget_width(o) - pmcontainer_margin (o);
      orgtop = top + widget_height(o);
      break;
    case 3:
      orgleft = orgleft + widget_width(o);
      orgtop = orgtop + pmcontainer_margin (o);
      break;
    }

  left = orgleft, top = orgtop;
  {
    integer_t i;
    pmcontainer_t cp = (pmcontainer_t) o;
    content_t p;
    if (pmcontainer4_drawdirection (o) == 0) {
      start = 0;
      stop =  cp->n_contents;
      inc = 1;
    } else {
      stop = -1;
      start =  cp->n_contents-1;
      inc = -1;
    }
    for (i = start; i != stop; i+=(inc))
      {
	integer_t x, y;
	content_t np;
	np = cp->content;
	p = &np[i];

	switch (pmcontainer4_direction (o))
	  {
	  case 0:
	    x = orgleft + p->ofs;
	    y = orgtop + widget_height(o) - p->height;
	    break;
	  case 1:
	    x = orgleft + widget_width(o) - p->width;
	    y = orgtop - p->ofs - p->height;
	    break;
	  case 2:
	    x = orgleft - p->ofs - p->width;
	    y = orgtop - widget_height(o) ;
	    break;
	  case 3:
	    x = orgleft - widget_width(o);
	    y = orgtop + p->ofs;
	    break;
	  }
	pixmap_draw_content(o, p, x, y);
      }
  }
  D (widget_sync (o));
}


struct item_type item_type_pmcontainer4 =
{
  "pmcontainer4",
  widget_type_pmcontainer4,
  sizeof (struct widget_pmcontainer4),
  pmcontainer4_create,
  pmcontainer_destroy,
  pmcontainer4_configure,
  pmcontainer4_display,
  pmcontainer_action,
  pmcontainer_invoke,
  pmcontainer_clear,
};
