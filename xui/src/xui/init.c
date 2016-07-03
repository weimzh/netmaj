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
 * This file is part of xnetmaj
 * 
 * init.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:14:08 1995
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "xnet.h"
#include "pixread.h"

widget_t root_widget;

static void
openning_pixmapfree ()
{
}

static integer_t waitkey_flag;

void
keywait_callback ()
{
  waitkey_flag = 1;
}

void
wait_key ()
{
  waitkey_flag = 0;
  widget_define_button_hook (3, keywait_callback);
  widget_event_discard (root_widget);
  for (; waitkey_flag == 0;)
    {
      if (widget_eventsqueued (root_widget))
        {
          _widget_handle_event ();
          invoke_handlers ();
        }
      else
        {
          ysleep (1);
        }
    }
  widget_define_button_hook (3, 0);
}

extern integer_t xwidth, xheight;

#include <time.h>

void
openning ()
{
  widget_t open_frame, base_frame;
  extern integer_t xheight;
  extern integer_t xwidth;
  widget_t pmframes[6];

  base_frame = title_create ();

  widget_set_background (root_widget, pm_mainbg.pm);

  snd (audioplay_openning ());
  widget_map (base_frame);
  widget_display (base_frame);
  widget_flush_sfx_shaffle (base_frame, 30, 30);
  sleep (1);

  widget_unmap (base_frame);
  widget_destory (base_frame);
}

void
setup ()
{
  root_widget = widget_init (xwidth, xheight);

  attr_init ();
  read_pixmaps ();
  read_adigit_pixmap ();
  read_bdigit_pixmap ();
  pai2pix_init ();
  auto_init ();
}

