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
 * x11evth.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:56:39 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "gt/widget/widget.h"
#include "gt/widget/local.h"

void
widget_palette_init ()
{
  extern int darkgreen, forestgreen;
  extern gdev_t main_gdev;
  model_color_alloc (main_gdev, darkgreen);
  model_color_alloc (main_gdev, forestgreen);
}

static void (*button1_hook_f) ();
static void (*button2_hook_f) ();
static void (*button3_hook_f) ();

void
widget_define_button_hook (number, hook)
     integer_t number;
     void (*hook) ();
{
  switch (number)
    {
    case 1:
      button1_hook_f = hook;
      break;
    case 2:
      button2_hook_f = hook;
      break;
    case 3:
      button3_hook_f = hook;
      break;
    }
}


extern widget_t root_widget;

void
_widget_handle_event ()
{
  widget_t w;
  extern gdev_t main_gdev;
  struct callbackarg carg;
  integer_t x, y;
  XEvent xevent;

  XNextEvent (main_gdev->x11.g.display, &xevent);
  switch (xevent.type)
    {
    default:
      return;
    case Expose:
      widget_force_flush (root_widget);
      break;
    case ButtonPress:
      switch (xevent.xbutton.button)
	{
	case 1:
	  if (button1_hook_f)
	    {
	      (*button1_hook_f) ();
	    }
	  w = widget_pointed (root_widget,
			      xevent.xbutton.x, xevent.xbutton.y,
			      &x, &y);
	  if (w)
	    {
	      carg.e.x = xevent.xbutton.x;
	      carg.e.y = xevent.xbutton.y;
	      carg.x = x;
	      carg.y = y;
	      widget_invoke (w, &carg);
	    }
	  break;
	case 2:
	  if (button2_hook_f)
	    {
	      (*button3_hook_f) ();
	    }
	  break;
	case 3:
	  if (button3_hook_f)
	    {
	      (*button3_hook_f) ();
	    }
	  else
	    {
	      bind_call (resource_button3press_callback);
	    }
	  break;
	default:
	  break;
	}			/* which button */
      break;
    case ButtonRelease:
      w = widget_pointed (root_widget,
			  xevent.xbutton.x, xevent.xbutton.y,
			  &x, &y);
      if (w)
	{
	  carg.e.x = xevent.xbutton.x;
	  carg.e.y = xevent.xbutton.y;
	  carg.x = x;
	  carg.y = y;
	  widget_call_callback (w, resource_buttonrelease_callback, &carg);
	}
      break;
    case KeyPress:
      w = widget_pointed (root_widget,
			  xevent.xbutton.x, xevent.xbutton.y,
			  &x, &y);
      if (w)
	{
	  char buffer[64];
	  int nkey;
	  KeySym mykey;
	  carg.e.x = xevent.xbutton.x;
	  carg.e.y = xevent.xbutton.y;
	  carg.x = x;
	  carg.y = y;
	  nkey = XLookupString ((void *) &xevent, buffer, 64, &mykey, 0);
	  if (nkey != 0)
	    {
	      carg.cchar = buffer[0];
	      widget_call_callback (w, resource_keypress_callback, &carg);
	    }
	}
      break;
    }
}
