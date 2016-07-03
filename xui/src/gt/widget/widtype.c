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
 * This file is part of xui
 * 
 * widtype.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:56:48 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"

#define def_foo_p(n) \
boolean widget_##n##_p(self) \
widget_t self;\
{\
   return self->type->type == identcat(widget_type_,n);\
}


def_foo_p (text);
def_foo_p (menu);
def_foo_p (frame);
def_foo_p (menuitem);
def_foo_p (button);
def_foo_p (toplevel);
def_foo_p (pixmapframe);
def_foo_p (pmcontainer);
def_foo_p (canvas);

char *
widget_type_string (w)
     widget_t w;
{
  switch (w->type->type)
    {
    case widget_type_text:
      return "widget_type_text";
    case widget_type_menu:
      return "widget_type_menu";
    case widget_type_frame:
      return "widget_type_frame";
    case widget_type_menuitem:
      return "widget_type_menuitem";
    case widget_type_button:
      return "widget_type_button";
    case widget_type_toplevel:
      return "widget_type_toplevel";
    case widget_type_pixmapframe:
      return "widget_type_pixmapframe";
    case widget_type_pmcontainer:
      return "widget_type_pixmapcontainer";
    default:
      return "unknown";
    }
}
