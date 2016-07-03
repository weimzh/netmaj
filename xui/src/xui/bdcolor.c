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
 * bdcolor.c -- 
 *
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:53:10 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:53:15 1996
 *
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"

widget_t bdcolor;
extern widget_t root_widget;
extern integer_t xwidth, xheight;

void set_basiccolorpixel ();

struct colorcombo
{
  integer_t color;
  char *name;
};

extern integer_t red, tann, yellow, green, purple, blue, tomato, gray;
extern integer_t darkgreen, yellowgreen, forestgreen, seagreen, darkskyblue;
extern integer_t black, white, darkorange;
extern integer_t indianred, maroon, deeppink, khaki, mintcream, cadetblue,
  chocolate, darkorchid;

#define N_COMBO 11

static struct colorcombo colorcombo[N_COMBO];
integer_t basiccolorpixel;

void
colorcombo_init ()
{
  if (!flag_basiccolorname)
    {
      flag_basiccolorname = "darkgreen";
    }

#define setcolor(idx, cname)  \
  colorcombo[idx].name = # cname; colorcombo[idx].color = cname;

  setcolor (0, darkgreen);
  setcolor (1, darkorange);
  setcolor (2, yellowgreen);
  setcolor (3, seagreen);
  setcolor (4, darkskyblue);
  setcolor (5, indianred);
  setcolor (6, maroon);
  setcolor (7, khaki);
  setcolor (8, seagreen);
  setcolor (9, chocolate);
  setcolor (10, darkorchid);

  set_basiccolorpixel (flag_basiccolorname);
}

integer_t
get_basiccolorpixel ()
{
  return basiccolorpixel;
}

void
set_basiccolorpixel (char *name)
{
  int i;
  for (i = 0; i < N_COMBO; i++)
    {
      if (!colorcombo[i].name)
        return;
      if (!strcmp (name, colorcombo[i].name))
        {
          basiccolorpixel = colorcombo[i].color;
          return;
        }
    }
}

static void
select_bdcolor callbackarg_param
{
  integer_t org = get_basiccolorpixel ();
  snd (audioplay_color ());

  set_basiccolorpixel (colorcombo[val].name);
  flag_basiccolorname = colorcombo[val].name;

  if (colorcombo[val].color != yellow)
      widget_configure (w, resource_textcolor, yellow);
  else
      widget_configure (w, resource_textcolor, tomato);
  widget_configure (w, resource_relief, relief_sunken);
  widget_display (w);
  widget_flush (w);

  if (org != get_basiccolorpixel ())
    {
      widget_change_models (root_widget, get_basiccolorpixel ());
    }

  widget_configure (w, resource_relief, relief_raised);
  if (colorcombo[val].color != white)
    widget_configure (w, resource_textcolor, white);
  else
    widget_configure (w, resource_textcolor, black);

  widget_display (w);
  widget_flush (w);

  widget_unmap (bdcolor);
  board_redraw (1);
}

widget_t
bdcolor_init ()
{
  widget_t frame;
  widget_t aframe, bframe, cframe;
  widget_t xframe, yframe, zframe, w;
  widget_t texts[N_COMBO];

  frame = widget_create ("frame", root_widget);

  aframe = widget_create ("text", frame);
  widget_set_geometry (aframe, xwidth, 54);
  widget_configure (aframe, resource_textcolor, tomato);
  widget_configure (aframe, resource_textfont, XLARGETIMESIB);
  widget_configure (aframe, resource_textshadow, 1);
  widget_configure (aframe, resource_text, "  Board Color Selection");

  bframe = widget_create ("frame", frame);
  widget_configure (bframe, resource_packed, resource_horizontal);
  widget_set_geometry (bframe, xwidth, 20 * N_COMBO);

  xframe = widget_create ("frame", bframe);
  widget_set_geometry (xframe, 100, 20 * N_COMBO);
  widget_configure (xframe, resource_wrap, 0);

  yframe = widget_create ("frame", bframe);
  widget_set_geometry (yframe, xwidth - 200, 20 * N_COMBO);

  zframe = widget_create ("frame", bframe);
  widget_set_geometry (zframe, 100, 20 * N_COMBO);
  widget_configure (zframe, resource_wrap, 0);

  {
    int i;
    for (i = 0; i < N_COMBO; i++)
      {
        char buffer[64];
        texts[i] = widget_create ("text", yframe);
        sprintf (buffer, "Click me!! %s", colorcombo[i].name);
        widget_model_change (texts[i], colorcombo[i].color,
                             black, black, black);
        widget_set_geometry (texts[i], xwidth - 200, 20);
        widget_configure (texts[i], resource_relief, relief_raised);
        widget_configure (texts[i], resource_border_width, 2);
        widget_configure (texts[i], resource_text, buffer);
        widget_configure (texts[i], resource_cannot_change_model, 1);
        if (colorcombo[i].color != white)
          widget_configure (texts[i], resource_textcolor, white);
        else
          widget_configure (texts[i], resource_textcolor, black);
        widget_configure (texts[i], resource_textfont, HELVB);
        widget_add_callback (texts[i], resource_buttonpress_callback,
                             select_bdcolor, i);
      }
  }
  widget_pack (yframe);
  widget_pack (bframe);
  widget_pack (frame);
  widget_fix (frame);
  widget_set_geometry (frame, xwidth, xheight);
  return frame;
}


void
bdcolor_callback callbackarg_param
{
  extern widget_t board;
  extern widget_t about;

  if (!bdcolor)
    {
      bdcolor = bdcolor_init ();
    }

  widget_unmap (board);
  widget_unmap (about);

  widget_map (bdcolor);
  widget_display (bdcolor);
  widget_flush (bdcolor);
}
