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
 * This file is part of xnetmaj
 * 
 * core.c -- 
 * 
 * $Id: core.c,v 1.2 1996/07/31 22:47:30 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:01:35 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 23:01:36 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/core.h"

widget_t
core_create (p)
     widget_t p;
{
  return p;
}
void
core_destroy (p)
     widget_t p;
{
}
void
core_clear (p)
     widget_t p;
{
}

#define xofs(t)   (_offsetof(struct widget_core, t))

static struct configure core_configspec[] =
{
  {0,},
};

void
core_configure (p, resourceid, value)
     widget_t p;
     integer_t resourceid;
     void *value;

{
  configure_proc (p, core_configspec, resourceid, value);
}

void
core_display (p)
     widget_t p;
{
}
void
core_action (p)
     widget_t p;
{
}
void
core_invoke (p)
     widget_t p;
{
}




struct item_type item_type_core =
{
  "core",
  widget_type_core,
  sizeof (struct widget_core),
  core_create,
  core_destroy,
  core_configure,
  core_display,
  core_action,
  core_invoke,
  core_clear,
  0,
};
