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
 * convini.c -- 
 * 
 * $Id: convini.c,v 1.2 1996/07/31 22:47:29 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:01:38 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 23:01:39 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"

widget_t
button_create (parent, w, h, text, color, font, callback, arg)
     widget_t parent;
     char *text;
     integer_t w, h, color, font;
     void (*callback) ();
     void *arg;
{
  widget_t button;
  button = widget_create ("button", parent);
  widget_set_geometry (button, w, h);
  widget_configure (button, resource_text, text);
  widget_configure (button, resource_textcolor, color);
  widget_configure (button, resource_textfont, font);
  widget_add_callback (button,
		       resource_buttonpress_callback,
		       (void *) callback, arg);
  return button;
}

widget_t
text_create (parent, w, h, text, color, font, shadow)
     widget_t parent;
     char *text;
     integer_t w, h, color;
     integer_t font;
     integer_t shadow;
{
  widget_t header = widget_create ("text", parent);
  widget_set_geometry (header, w, h);
  widget_configure (header, resource_text, text);
  widget_configure (header, resource_textfont, font);
  widget_configure (header, resource_textcolor, color);
  widget_configure (header, resource_textshadow, shadow);
  return header;
}
