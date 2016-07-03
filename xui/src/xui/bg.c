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

static int n_count;

static int
bg_action ()
{
  n_count++;
  if (n_count > 34)
    {
      clear_timer_handler ();
      return 0;
    }
  pai2pix (n_count * 4, 3, 0, 0);
  pai2pix (n_count * 4, 3, 0, 1);

  return 0;
}

static int
bg_handler ()
{
  stop_timer ();
  save_handler (0, bg_action);
  resume_timer ();

  return 0;
}

void
bg_init ()
{
  stop_timer ();
  set_timer_handler (bg_handler, 7);
}
