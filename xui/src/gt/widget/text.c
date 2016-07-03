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
#include "gt/widget/text.h"

#define text_length(p)  ((struct widget_text *)p)->length
#define text_pixmap(p)  ((struct widget_text *)p)->pixmap
#define text_color(p)     ((struct widget_text *)p)->textcolor
#define text_font(p)     ((struct widget_text *)p)->textfont
#define text_shadow_p(p)        ((struct widget_text *)p)->textshadow


void widget_text_configure(w, text, color, font, shadow)
widget_t w;
char *text;
int color, font, shadow;
{
  if (text) widget_configure (w, resource_text, text);
  text_color(w) = color;
  text_font(w) = font;
  text_shadow_p(w) = shadow;
}

char *
widget_get_text (w)
     widget_t w;
{
  return text_text (w);
}

static widget_t
_text_create (p)
     widget_t p;
{
  extern integer_t white;
  widget_set_geometry (p, 32, 50);
  text_color (p) = white;
  return p;
}

static void
_text_destroy (o)
     widget_t o;
{
}

static void
_text_clear (o)
     widget_t o;
{
}

#define xofs(t)   (_offsetof(struct widget_text, t))

static struct configure text_configspec[] =
{
  {resource_text, datatype_string, xofs (text), 0},
  {resource_textcolor, datatype_int, xofs (textcolor), 0},
  {resource_textfont, datatype_int, xofs (textfont), 0},
  {resource_textshadow, datatype_char, xofs (textshadow), 0},
  {0,},
};

static void
_text_configure (item, resourceid, value)
     widget_t item;
     integer_t resourceid;
     void *value;
{
  if (resourceid == resource_text)
    {
      char *s = value;
      if (!text_text (item))
	{
	  text_text (item) = xmalloc (strlen (s) + 1);
	  text_length (item) = strlen (s);
	}
      else
	{
	  if (text_length (item) <= strlen (s))
	    {
	      text_text (item) = xrealloc (text_text (item), strlen (s) + 1);
	      text_length (item) = strlen (s);
	    }
	}
      strcpy (text_text (item), s);
      return;
    }
  configure_proc (item, text_configspec, resourceid, value);
}

static void
_text_display (o)
     widget_t o;
{
  integer_t left, top;

  widget_abs_xy (o, 0, 0, &left, &top);

  rectanglewidget_display (o);
  if (text_text (o))
    {
      integer_t borderwidth;

      borderwidth = widget_border_width (o);
      if (text_shadow_p (o))
	{
	  gdev_drawshadowtext (widget_gdev (o),
			       left + borderwidth, top + borderwidth,
			       text_color (o), text_font (o), text_text (o),
			       strlen (text_text (o)));
	}
      else
	{
	  gdev_drawtext (widget_gdev (o),
			 left + borderwidth, top + borderwidth,
			 text_color (o), text_font (o), text_text (o),
			 strlen (text_text (o)));
	}
    }
}

static void
_text_action (o)
     void *o;
{
}

static void
_text_invoke (o, arg)
     widget_t o;
     struct callbackarg *arg;
{
  widget_call_callback (o, resource_buttonpress_callback, arg);
}

boolean
text_have_text_p (w)
     widget_t w;
{
  return text_text (w) && *text_text (w);
}

integer_t
text_string_n_pixels (w, str, len)
     widget_t w;
     char *str;
     integer_t len;
{
  return gdev_string_n_pixels (widget_gdev (w), text_font (w), str, len);
}

struct item_type item_type_text =
{
  "text",
  widget_type_text,
  sizeof (struct widget_text),
  _text_create,
  _text_destroy,
  _text_configure,
  _text_display,
  _text_action,
  _text_invoke,
  _text_clear,
  0,
};
