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
 */

#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"

extern widget_t root_widget;
extern integer_t xwidth, xheight;

void about_callback ();

widget_t about;

static void
about_keywait_callback ()
{
  widget_unmap (about);
  board_redraw (1);
}

extern void testpai_callback callbackarg_param;

static char *history[] = {
  "         Copyright (c) 1994,1995,1996 Todo Software All rights reserved.",
  "         Copyright (c) 1996 Koji Suzuki All rights reserved.",
  0,
};

void
about_init ()
{
  widget_t framepoint, frames[6], canvas;
  integer_t x, y;
  extern integer_t xwidth, xheight;
  extern integer_t yellow;
  char **p;

  framepoint = create_basic_info_frame ("frame", about, frames, PAI_W * 18,
                                        0, 1);
  widget_configure (frames[4], resource_frame_start, 50);
  aboutokbutton = frames[5];
  {
    if (!PAICARD)
      {
        testpaibutton = button_create (frames[4],
                                       BUTTON_W, BUTTON_H,
                                       "pai test",
                                       yellow,
                                       TIMESI, (void *) testpai_callback, "");
        testpai_init ();
      }
  }

  {
    if (!PAICARD || BUILD4)
      {
        void pinfo_callback ();
        totalbutton = button_create (frames[4],
                                     BUTTON_W, BUTTON_H,
                                     "info",
                                     yellow,
                                     TIMESI, (void *) pinfo_callback, 0);
      }
  }

  {
    void rank_callback ();
    if (!PAICARD || BUILD4)
      {
        rankbutton = button_create (frames[4],
                                    BUTTON_W, BUTTON_H,
                                    "rank",
                                    yellow,
                                    TIMESI, (void *) rank_callback, "");
      }
  }

  {
    void bdcolor_callback ();
    bdcolorbutton = button_create (frames[4],
                                   BUTTON_W, BUTTON_H,
                                   "color",
                                   yellow,
                                   TIMESI, (void *) bdcolor_callback, "");
  }
  {
    extern widget_t about;
    void config_callback ();
    configbutton = button_create (frames[4],
                                  BUTTON_W, BUTTON_H,
                                  "config",
                                  yellow,
                                  TIMESI, (void *) config_callback, about);
  }
  widget_add_callback (frames[5],
                       resource_buttonpress_callback,
                       (void *) about_keywait_callback, "");


  widget_get_geometry (framepoint, &x, &y);

#define aboutinfo(string, font) \
  canvas = widget_create("text", framepoint); \
  widget_set_geometry (canvas, xwidth, 20);\
  widget_configure(canvas, resource_textfont, font);\
  widget_configure(canvas, resource_text, string);

  canvas = widget_create("text", framepoint);
  widget_set_geometry (canvas, xwidth, 40);
  widget_configure(canvas, resource_textfont, LUCIDAB);
  widget_configure(canvas, resource_text, "Mahjong for X11");

  for (p = history; *p; p++)
    {
      aboutinfo (*p, TIMESI);
    }
  aboutinfo ("         Visit http://www.sfc.wide.ad.jp/~kusune/netmaj/", TIMESIB);
  aboutinfo ("", HELVB);
  aboutinfo ("         Based on:", TIMESIB);
  aboutinfo ("xnetmaj: Todo Software", HELVB);
  aboutinfo ("         Mail to NBC02365@niftyserve.or.jp", TIMESI);
  aboutinfo ("netmaj: Koji Suzuki", HELVB);
  aboutinfo ("         Mail to suz@kt.rim.or.jp", TIMESI);
  aboutinfo ("", HELVB);

  widget_pack (about);
  widget_fix (about);

  widget_set_geometry (about, xwidth, xheight);
}

void
make_about ()
{
  about = widget_create ("frame", root_widget);
  widget_configure (about, resource_name, "about");
  widget_set_geometry (about, xwidth, xheight);
  about_init ();
}

void
about_callback callbackarg_param
{
  extern widget_t board;

  if (!about)
    {
      make_about ();
      lang_set ();
    }

  widget_unmap (board);
  widget_map (about);
  widget_display (about);
  widget_flush (about);
}
