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
 * bind.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Mon Apr 17 12:28:48 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "gt/widget/widget.h"

static struct bind
  {
    integer_t id;
    integer_t enable;
    integer_t resource;
      integer_t (*func) ();
    void *val;
    struct bind *next;
  }
 *bp;
static integer_t bind_id;

void
bind_call (resource)
     integer_t resource;
{
  struct bind *p;
  for (p = bp; p; p = p->next)
    {
      if (p->resource == resource && p->enable)
	{
	  ((*(p->func)) (p->val));
	}
    }
  return;
}

integer_t
bind_save (resource, func, val)
     integer_t resource;
integer_t (*func) ();
     void *val;
{
  struct bind *p;

  p = (void *) xcalloc (sizeof (struct bind));
  p->resource = resource;
  p->func = func;
  p->val = val;
  p->next = bp;
  bp = p;
  p->enable = 1;
  return p->id = bind_id++;
}
