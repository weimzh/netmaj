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
#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/core.h"
#include "gt/widget/button.h"
#include "gt/widget/frame.h"
#include "gt/widget/pixmapfr.h"
#include "gt/widget/menu.h"
#include "gt/widget/toplevel.h"
#include "gt/widget/text.h"
#include "gt/widget/pixmapco.h"
#include "gt/widget/radio.h"

#include "gt/widget/4/pixbox.h"
#include "gt/widget/4/pixmap4.h"

#include "gt/dev/gdev.h"
#include <stdio.h>
#include <stdlib.h>

static item_type_t item_types;

static item_type_t
item_type_search_from_name (s)
     char *s;
{
  item_type_t t = item_types;

  for (; t; t = t->next)
    {
      if (!strcmp (t->name, s))
	{
	  break;
	}
    }
  if (!t)
    {
      fprintf (stderr, "gt: cannot find %s in item list\n", s);
      abort ();
    }

  return t;
}

widget_t
widget_add_child (parent, child)
     widget_t parent, child;
{
  widget_t ch = parent->child;

  child->parent = parent;
  if (!ch)
    {
      parent->child = child;
      return 0;
    }
  while (ch->next)
    {
      ch = ch->next;
    }
  ch->next = child;
  return ch;
}



widget_t
widget_create (s, parent)
     char *s;
     widget_t parent;
{
  item_type_t t = item_type_search_from_name (s);
  widget_t w = (void *) xcalloc (t->size);
  extern gdev_t main_gdev;

  w->dev = main_gdev;
  if (parent)
    {
      widget_add_child (parent, w);
    }
  {
    extern integer_t forestgreen, black;
    model_init (main_gdev, &w->active, get_basiccolorpixel (), black);
    model_init (main_gdev, &w->inactive, forestgreen, black);
  }
  w->type = t;
  (*(t->proc.create)) (w);
  return w;
}

void
_widget_configure (s, ac, av)
     widget_t s;
     int ac;
     char *av;
{
  (*(s->type->proc.configure)) (s, ac, av);
}


void
widget_invoke (s, arg)
     widget_t s;
     struct callbackarg *arg;
{
  (*(s->type->proc.invoke)) (s, arg);
}

void
widget_clear (s, arg)
     widget_t s;
     struct callbackarg *arg;
{
  (*(s->type->proc.clear)) (s, arg);
}

void
widget_delete (w)
     widget_t w;
{
  widget_t parent;
  parent = w->parent;
  if (!w)
    return;

  if (parent->child == w)
    {
      parent->child = w->next;
    }
  else
    {
      widget_t ch;
      ch = parent->child;
      while (ch->next != w && ch)
	{
	  ch = ch->next;
	}
      if (ch == 0)
	{
	  return;
	}
      ch->next = w->next;
    }
}

void
widget_destory (o)
     widget_t o;
{
  widget_t q;
  for (q = o->child; q; q = q->next)
    {
      widget_destory (q);
    }
  (*(o->type->proc.destroy)) (o);
  widget_delete (o);
}


static void
item_type_init ()
{
  item_types = &item_type_button;
  item_type_button.next = &item_type_frame;
  item_type_frame.next = &item_type_toplevel;
  item_type_toplevel.next = &item_type_pixmapframe;
  item_type_pixmapframe.next = &item_type_text;
  item_type_text.next = &item_type_pmcontainer;
  item_type_pmcontainer.next = &item_type_core;
  item_type_core.next = &item_type_radio;
  item_type_radio.next = &item_type_pixbox;
  item_type_pixbox.next = &item_type_pmcontainer4;
}

void
install_widget (p)
     struct item_type *p;
{
  p->next = item_types;
  item_types = p;
}

void
item_init ()
{
  item_type_init ();
}
