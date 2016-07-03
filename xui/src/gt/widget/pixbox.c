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
 * $Id: pixbox.c,v 1.2 1996/07/31 22:47:43 suz Exp $   
 * Author          : NBC02365@niftyserve.or.jp   
 * Created On      : Thu Apr 13 19:48:50 1995   
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Apr 11 02:50:18 1996
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/4/pixbox.h"
#include "gt/widget/private.h"

#define D(a)

#ifdef __GNUC__
#define alloca	__builtin_alloca
#endif

#define xofs(t)   (_offsetof(struct widget_pixbox, t))

static struct configure pixbox_configspec[] =
{
  {resource_row, datatype_int, xofs (row), 0},
  {resource_col, datatype_int, xofs (col), 0},
  {resource_unit_w, datatype_int, xofs (unit_w), 0},
  {resource_unit_h, datatype_int, xofs (unit_h), 0},
  {0},
};

void
pixbox_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  if (!configure_proc (item, pixbox_configspec, resourceid, value)) {
    pmcontainer4_configure(item, resourceid, value);
  }
}


void
pixbox_display (o)
     widget_t o;
{
  integer_t left, top, orgleft, orgtop;

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
    integer_t i, idx, start, stop, inc;
    pmcontainer_t cp = (pmcontainer_t) o;
    content_t p;
    int n = (cp->n_contents/pixbox_col(o) + 1)* sizeof(int);
    int *ofs = (int *)alloca(n);
    memset(ofs, 0, n);

    if (pmcontainer4_drawdirection (o) == 0) {
      start = 0;
      stop =  cp->n_contents;
      inc = 1;
    } else {
      stop = -1;
      start =  cp->n_contents-1;
      inc = -1;
    }
    for (i = 0, idx = 0; i < cp->n_contents; i++) {
      if (i % pixbox_col(o) == 0) {
	ofs[idx++] = (((content_t)(cp->content))[i]).ofs;
      }
    }
    for (i = start, idx = 0; i != stop; i+=(inc), idx++)
      {
	integer_t x, y;
	content_t np;
	integer_t col, row, uh, uw;
	np = cp->content;
	p = &np[i];
	
	col = pixbox_col(o);
	row = pixbox_row(o);
	uw = pixbox_unit_w(o);
	uh = pixbox_unit_h(o);

	switch (pmcontainer4_direction (o))
	  {
	  case 0:
	    x = orgleft + p->ofs;
	    y = orgtop + pixbox_unit_h(o) - p->height;
	    x -= ofs[idx/col];
	    y += ((idx / col) * pixbox_unit_h(o));
	    break;
	  case 1:
	    x = orgleft + pixbox_unit_h(o) - p->width;
	    y = orgtop - p->ofs - p->height;
	    x += ((idx / col) * pixbox_unit_h(o));
	    y += ofs[idx/col];
	    break;
	  case 2:
	    x = orgleft - p->ofs - p->width;
	    y = orgtop - pixbox_unit_h(o);
	    x += ofs[idx/col];
	    y -= ((idx / col) * pixbox_unit_h(o));
	    break;
	  case 3:
	    x = orgleft - pixbox_unit_h(o);
	    y = orgtop + p->ofs;
	    x -= ((idx / col) * pixbox_unit_h(o));
	    y -= ofs[idx/col];
	    break;
	  }
	pixmap_draw_content(o, p, x, y);
      }
  }
  D (widget_sync (o));
}


extern widget_t pmcontainer4_create();

struct item_type item_type_pixbox =
{
  "pixbox",
  widget_type_pixbox,
  sizeof (struct widget_pixbox),
  pmcontainer4_create, 
  pmcontainer_destroy,
  pixbox_configure,
  pixbox_display,
  pmcontainer_action,
  pmcontainer_invoke,
  pmcontainer_clear,
};
