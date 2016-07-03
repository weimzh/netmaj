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
 * title.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Fri Aug 25 18:32:10 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "xnet.h"
#include "pixread.h"

extern widget_t root_widget;
extern integer_t xwidth, xheight;

widget_t title;

widget_t
title_create ()
{
  widget_t frame;
  widget_t aframe, bframe, cframe;
  widget_t xframe, yframe, zframe, open_frame, bottom, canvas;
  integer_t space;
  integer_t width, height;
  extern integer_t white;

#define NETMAJSYSTEM "netmaj for X11"
  frame = widget_create ("frame", root_widget);

  aframe = widget_create ("text", frame);
  widget_set_geometry (aframe, xwidth, 54);
  widget_configure (aframe, resource_textcolor, white);
  widget_configure (aframe, resource_textfont, HELVB);
  widget_configure (aframe, resource_textshadow, 1);
  widget_configure (aframe, resource_text, NETMAJSYSTEM);
  widget_configure (aframe, resource_textcolor, white);

  bframe = widget_create ("frame", frame);
  widget_configure (bframe, resource_packed, resource_horizontal);
  widget_set_geometry (bframe, xwidth, 20 * 10);

#define bmw (PAICARD ? 640 : 400)
#define bmh 300

  width = bmw;
  height = bmh;
  space = (xwidth - width) / 2;
  if (space < 0)
    space = 0;

  if (!PAICARD)
    {
      xframe = widget_create ("frame", bframe);
      widget_set_geometry (xframe, space, height);
      widget_configure (xframe, resource_wrap, 0);
    }

  open_frame = yframe = widget_create ("pmcontainer", bframe);
  widget_set_geometry (yframe, width, height);

  zframe = widget_create ("frame", bframe);
  widget_set_geometry (zframe, space, height);

  bottom = cframe = widget_create ("pmcontainer", frame);
  widget_set_geometry (cframe, xwidth, 140);
  widget_configure (cframe, resource_wrap, 0);
  widget_configure (cframe, resource_pixmapstart, space - 10);

  widget_pack (bframe);
  widget_pack (frame);

  widget_set_geometry (frame, xwidth, xheight);
  pmcontainer_print (open_frame, pm_asia);
  pmcontainer_print (bottom, pm_logo);

  return frame;
}
