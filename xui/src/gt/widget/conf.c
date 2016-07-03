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
 * conf.c -- 
 * 
 * $Id: conf.c,v 1.2 1996/07/31 22:47:28 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:01:44 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Apr 11 00:05:52 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include <stdlib.h>

#define xofs(t)   (_offsetof(struct widget, t))
static struct configure widget_configspec[] =
{
  {resource_left, datatype_int, xofs (left), 0},
  {resource_top, datatype_int, xofs (top), 0},
  {resource_height, datatype_int, xofs (height), 0},
  {resource_width, datatype_int, xofs (width), 0},
  {resource_packed, datatype_char, xofs (packed), 0},
  {resource_invisible, datatype_char, xofs (invisible), 0},
  {resource_border_width, datatype_char, xofs (border_width), 0},
  {resource_active_relief, datatype_char, xofs (active.relief), 0},
  {resource_inactive_relief, datatype_char, xofs (inactive.relief), 0},
  {resource_relief, datatype_char, xofs (active.relief), 0},
  {resource_wrap, datatype_char, xofs (wrap), 0},
  {resource_transparent, datatype_char, xofs (transparent), 0},
  {resource_fix, datatype_char, xofs (abs.fixed), 0},
  {resource_name, datatype_pointer, xofs (name), 0},
  {resource_cannot_change_model, datatype_char, xofs (cannot_change_model), 0},
  0
};

boolean
configure_proc (item, config, resourceid, arg)
     widget_t item;
     configure_t config;
     integer_t resourceid;
     void *arg;
{
  configure_t p = config;
  char *object = (char *) item;
  integer_t ofs;

  for (; p->resourceid; p++)
    {
      if (p->resourceid != resourceid)
	{
	  continue;
	}
      ofs = p->offset;
      switch (p->datatype)
	{
	case datatype_string2int:
	  *(integer_t *) (object + ofs) = atoi ((char *) arg);
	  break;
	case datatype_int:
	  *(integer_t *) (object + ofs) = (integer_t) (arg);
	  break;
	case datatype_char:
	  *(char *) (object + ofs) = (char) (int) (arg);
	  break;
	case datatype_string:
	  *(char **) (object + ofs) = (char *) arg;
	  break;
	case datatype_pointer:
	  *(void **) (object + ofs) = (void *) arg;
	  break;
	case datatype_pixmap:
	  *(gpixmap_t *) (object + ofs) = (gpixmap_t) arg;
	  break;
	default:
	  break;
	}
      return 1;
    }
  if (config == widget_configspec)
    {
      return 0;
    }
  configure_proc (item, widget_configspec, resourceid, arg);
  return 0;
}
