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
 * toplevel.c -- 
 * 
 * $Id: toplevel.c,v 1.2 1996/07/31 22:47:59 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:56:56 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:57:02 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/toplevel.h"

widget_t
toplevel_create (p)
     widget_t p;
{
  frame_packed (p) = resource_virtical;
  return p;
}

void
toplevel_destroy (p)
     widget_t p;
{
}

void
toplevel_clear (p)
     widget_t p;
{
}

void
toplevel_configure (p)
     widget_t p;
{
}

void
toplevel_display (p)
     widget_t p;
{
}

void
toplevel_action (p)
     widget_t p;
{
}

void
toplevel_invoke (p)
     widget_t p;
{
}




struct item_type item_type_toplevel =
{
  "toplevel",
  widget_type_toplevel,
  sizeof (struct widget_toplevel),
  toplevel_create,
  toplevel_destroy,
  toplevel_configure,
  toplevel_display,
  toplevel_action,
  toplevel_invoke,
  toplevel_clear,
  0,
};
