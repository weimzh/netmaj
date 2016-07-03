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
#include "gt/widget/widget.h"
#include "gt/widget/pixcont.h"
#include "gt/widget/local.h"


void pixmap_draw_content(widget_t o, content_t p, integer_t left, integer_t top)
{
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
