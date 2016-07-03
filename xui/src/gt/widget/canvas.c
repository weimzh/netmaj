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
 * canvas.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Apr 13 19:48:50 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 23:01:55 1996
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"
#include "gt/widget/canvas.h"
#include <stdlib.h>
#include <string.h>

#define D(a)

widget_t
canvas_create (p)
     widget_t p;
{
  canvas_content (p) = 0;
  canvas_n_contents (p) = 0;
  p->wrap = 0;
  return p;
}

void
canvas_destroy (p)
     widget_t p;
{
}

#define xofs(t)   (_offsetof(struct widget_canvas, t))

static struct configure canvas_configspec[] =
{
  {0},
};

#define PIXMAP 1
#define TEXT   2

typedef struct content
  {
    integer_t row;
    integer_t col;
    int kind;
    void (*display) ();
    union
      {
	struct
	  {
	    integer_t width;
	    integer_t height;
	    integer_t color;
	    gpixmap_t content;
	    gpixmap_t mask;
	  }
	g;
	struct
	  {
	    char *text;
	    integer_t color;
	    integer_t font;
	  }
	t;
	struct
	  {
	    int ival;
	  }
	i;
      }
    u;
  }
 *content_t;

void
canvas_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  if (resourceid == resource_max_contents)
    {
      canvas_t cp = (canvas_t) item;
      integer_t size = ((integer_t) value) * sizeof (struct content);

      if (cp->content)
	{
	  cp->content = xrealloc (cp->content, size);
	}
      else
	{
	  cp->content = xcalloc (size);
	}
      return;
    }
  configure_proc (item, canvas_configspec, resourceid, value);
}

static
struct content *
canvas_new_content (w)
     widget_t w;
{
  canvas_t p = (canvas_t) w;
  content_t val;
  integer_t i;

  val = p->content;
  if (!widget_canvas_p (w))
    {
      abort ();
    }
retry:
  if (p->max_contents <= p->n_contents + 1)
    {
      widget_configure (w, resource_max_contents, p->max_contents * 2);
      goto retry;
    }
  if (!val)
    {
      widget_configure (w, resource_max_contents, 2);
    }

  val = p->content;
  i = p->n_contents;
  p->n_contents++;
  return &val[i];
}


void
canvas_setup (w, row, col)
     widget_t w;
     integer_t row, col;
{

}

static void
canvas_text_display (o, p, left, top)
     widget_t o;
     content_t p;
     integer_t left, top;
{
  gdev_drawtext (widget_gdev (o),
		 left, top,
		 p->u.t.color, p->u.t.font, p->u.t.text,
		 strlen (p->u.t.text));
}

void
canvas_add_text (w, row, col, s, color, font)
     widget_t w;
     integer_t row, col, color, font;
     char *s;
{
  struct content *cp;

  cp = canvas_new_content (w);
  cp->kind = TEXT;
  cp->display = canvas_text_display;
  cp->row = row;
  cp->col = col;
  cp->u.t.text = (char *) malloc (sizeof (char) * (strlen (s) + 1));
  strcpy (cp->u.t.text, s);
  cp->u.t.color = color;
  cp->u.t.font = font;
}

static void
canvas_pixmap_display (o, p, left, top)
     widget_t o;
     content_t p;
     integer_t left, top;
{
  if (p->u.g.mask)
    {
      gdev_renderpixmap (widget_gdev (o),
			 p->u.g.content, p->u.g.mask,
			 0, 0, 0,
			 widget_width (o),
			 widget_height (o),
			 left, top);
    }
  else
    {
    }
}

void
canvas_add_pixmap (w, row, col, colorp, content, mask)
     widget_t w;
     integer_t row, col;
     integer_t colorp;
     gpixmap_t content;
     gpixmap_t mask;
{
  integer_t width;
  integer_t height;

  struct content *cp;

  cp = canvas_new_content (w);

  gdev_drawable_get_geometry (w->dev, content, &width, &height);

  cp->kind = PIXMAP;
  cp->display = canvas_pixmap_display;
  cp->row = row;
  cp->col = col;
  cp->u.g.color = colorp;
  cp->u.g.content = content;
  cp->u.g.mask = mask;
  cp->u.g.width = width;
  cp->u.g.height = height;
}


void
canvas_clear (w)
     widget_t w;
{
  integer_t i;
  canvas_t p;
  content_t val;

  p = (canvas_t) w;
  val = p->content;
  if (!widget_canvas_p (w))
    {
      abort ();
    }
  for (i = 0; i < p->n_contents; i++)
    {
      if (val[i].kind == TEXT)
	{
	  if (val[i].u.t.text)
	    xfree (val[i].u.t.text);
	}
    }
  memset ((void *) val, 0, sizeof (struct content) * p->max_contents);
  p->n_contents = 0;
}


void
canvas_display (o)
     widget_t o;
{
  integer_t left, top, orgleft, orgtop;

  if (!widget_canvas_p (o))
    {
      abort ();
    }
  widget_abs_xy (o, 0, 0, &left, &top);

  rectanglewidget_display (o);
  widget_abs_xy (o, 0, 0, &orgleft, &orgtop);
  {
    integer_t i;
    canvas_t cp = (canvas_t) o;
    content_t p;

    for (i = 0; i < cp->n_contents; i++)
      {
	p = &(((struct content *) (cp->content))[i]);
	top = orgleft + p->row;
	left = orgtop + p->col;
	(*(p->display)) (o, p, left, top);
      }
  }
  D (widget_sync (o));
}

void
canvas_action (p)
     widget_t p;
{
}

void
canvas_invoke (p, arg)
     widget_t p;
     struct callbackarg *arg;

{
  widget_call_callback (p, resource_buttonpress_callback, arg);
  widget_sync (p);
}




struct item_type item_type_canvas =
{
  "canvas",
  widget_type_canvas,
  sizeof (struct widget_canvas),
  canvas_create,
  canvas_destroy,
  canvas_configure,
  canvas_display,
  canvas_action,
  canvas_invoke,
  canvas_clear,
  0,
};
