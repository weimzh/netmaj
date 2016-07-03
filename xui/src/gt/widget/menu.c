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
#include "gt/widget/local.h"
#include "gt/widget/menu.h"

widget_t
menu_create (p)
     widget_t p;
{
  return p;
}
void
menu_destroy (p)
     widget_t p;
{
}
void
menu_clear (p)
     widget_t p;
{
}

#define xofs(t)   (_offsetof(struct widget_menu, t))

static struct configure menu_configspec[] =
{
  {resource_left, datatype_int, xofs (left), (void *) 0},
  {resource_top, datatype_int, xofs (top), 0},
  {resource_height, datatype_int, xofs (height), 0},
  {resource_width, datatype_int, xofs (width), 0},
/*  {resource_text,  datatype_string, xofs(text), 0 }, 
   {resource_pixmap,  datatype_pixmap, xofs(pixmap), 0 }, */
  {0,},
};
void
menu_configure (p, resourceid, value)
     widget_t p;
     integer_t resourceid;
     void *value;

{
  configure_proc (p, menu_configspec, resourceid, value);
}
void
menu_display (p)
     widget_t p;
{
}
void
menu_action (p)
     widget_t p;
{
}
void
menu_invoke (p)
     widget_t p;
{
}




struct item_type item_type_menu =
{
  "menu",
  widget_type_menu,
  sizeof (struct widget_menu),
  menu_create,
  menu_destroy,
  menu_configure,
  menu_display,
  menu_action,
  menu_invoke,
  menu_clear,
  0,
};

/***********************************************************************/


widget_t
menuitem_create (p)
     widget_t p;
{
}
void
menuitem_destroy (p)
     widget_t p;
{
}
void
menuitem_clear (p)
     widget_t p;
{
}

#undef xofs
#define xofs(t)   (_offsetof(struct widget_menuitem, t))

static struct configure menuitem_configspec[] =
{
  {resource_left, datatype_int, xofs (left), 0},
  {resource_top, datatype_int, xofs (top), 0},
  {resource_height, datatype_int, xofs (height), 0},
  {resource_width, datatype_int, xofs (width), 0},
/*  {resource_text,  datatype_string, xofs(text), 0 }, 
   {resource_pixmap,  datatype_pixmap, xofs(pixmap), 0 }, */
  {0,},
};
void
menuitem_configure (p, resourceid, value)
     widget_t p;
     integer_t resourceid;
     void *value;

{
  configure_proc (p, menu_configspec, resourceid, value);
}
void
menuitem_display (p)
     widget_t p;
{
}
void
menuitem_action (p)
     widget_t p;
{
}
void
menuitem_invoke (p)
     widget_t p;
{
}






struct item_type item_type_menuitem =
{
  "menuitem",
  widget_type_menuitem,
  sizeof (struct widget_menuitem),
  menuitem_create,
  menuitem_destroy,
  menuitem_configure,
  menuitem_display,
  menuitem_action,
  menuitem_invoke,
  menuitem_clear,
  0,
};
