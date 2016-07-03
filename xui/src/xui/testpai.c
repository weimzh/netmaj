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
 * testpai.c -- 
 *
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu May 11 10:27:04 1995
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */
#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"

static widget_t testpai;
static widget_t pais[10];

void
testpai_init ()
{
  widget_t frames[6], canvas, framepoint;
  integer_t i, x, y;
  extern integer_t xwidth, xheight;

  extern widget_t root_widget;
  extern integer_t xwidth, xheight;

  testpai = widget_create ("frame", root_widget);
  widget_configure (testpai, resource_name, "testpai");
  widget_set_geometry (testpai, xwidth, xheight);

  framepoint = create_basic_info_frame ("frame", testpai, frames, PAI_W * 18,
                                        1, 1);
  widget_configure (frames[4], resource_frame_start, 270);
  widget_get_geometry (framepoint, &x, &y);
  widget_set_geometry (frames[0], xwidth, 5);

  for (i = 0; i < 10; i++)
    {
      pais[i] = canvas = widget_create ("pmcontainer", framepoint);
      widget_set_geometry (canvas, xwidth, PAI_H);
    }

  widget_pack (testpai);
  widget_fix (testpai);

  widget_set_geometry (testpai, xwidth, xheight);
}

static void
testpai_draw (integer_t start, integer_t end, integer_t ofs)
{
  widget_t canvas;
  int i;
  int k = 0;

#define N_PAI 35
  canvas = pais[k];
  canvas_clear ();
  for (i = start; i <= end; i++)
    {
      print_colorpixmap2 (pai2pix (i * 4 + ofs, 3, 1, 0));
    }
  k++;

  canvas = pais[k];
  canvas_clear ();
  for (i = start; i <= end; i++)
    {
      print_colorpixmap2 (pai2pix (i * 4 + ofs, 3, 1, 1));
    }
  k++;

  canvas = pais[k];
  canvas_clear ();
  for (i = start; i <= end; i++)
    {
      print_colorpixmap2 (pai2pix (i * 4 + ofs, 3, 1, 3));
    }
  k++;

  canvas = pais[k];
  canvas_clear ();
  for (i = start; i <= end; i++)
    {
      print_colorpixmap2 (pai2pix (i * 4 + ofs, 3, 0, 0));
    }
  k++;

  canvas = pais[k];
  canvas_clear ();
  for (i = start; i <= end; i++)
    {
      print_colorpixmap2 (pai2pix (i * 4 + ofs, 3, 0, 1));
    }
  k++;

  canvas = pais[k];
  canvas_clear ();
  for (i = start; i <= end; i++)
    {
      print_colorpixmap2 (pai2pix (i * 4 + ofs, 3, 0, 3));
    }
  k++;

  widget_display (testpai);
  widget_flush (testpai);
  wait_key ();
}

void
testpai_callback callbackarg_param
{
  extern widget_t board;
  extern widget_t about;

  widget_unmap (board);
  widget_unmap (about);
  widget_map (testpai);

  testpai_draw (1, 9, 0);
  testpai_draw (10, 18, 0);
  testpai_draw (19, 27, 0);
  testpai_draw (28, 34, 0);
  testpai_draw (0, 0, 0);
  testpai_draw (5, 5, 1);
  testpai_draw (14, 14, 1);
  testpai_draw (23, 23, 1);

  widget_unmap (testpai);
  board_redraw (1);
}
