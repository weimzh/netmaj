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
 * pixdec.c -- read decoration pixmaps
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:13:56 1995
 *   read decoration pixmaps
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */
#include "xnet.h"
#include "pixread.h"

#include "bitmap/misc/digit0.xpm"
#include "bitmap/misc/digit1.xpm"
#include "bitmap/misc/digit2.xpm"
#include "bitmap/misc/digit3.xpm"
#include "bitmap/misc/digit4.xpm"
#include "bitmap/misc/digit5.xpm"
#include "bitmap/misc/digit6.xpm"
#include "bitmap/misc/digit7.xpm"
#include "bitmap/misc/digit8.xpm"
#include "bitmap/misc/digit9.xpm"
#include "bitmap/misc/bigton.xpm"
#include "bitmap/misc/bignan.xpm"
#include "bitmap/misc/bigsya.xpm"
#include "bitmap/misc/bigpei.xpm"
#include "bitmap/misc/kyoku.xpm"
#include "bitmap/misc/honba.xpm"
#include "bitmap/misc/reststr.xpm"
#include "bitmap/misc/dorastr.xpm"
#include "bitmap/misc/uradoras.xpm"
#include "bitmap/misc/ie.xpm"
#include "bitmap/misc/n1hon.xpm"
#include "bitmap/misc/n2hon.xpm"
#include "bitmap/misc/n3hon.xpm"
#include "bitmap/misc/n4hon.xpm"
#include "bitmap/misc/n5hon.xpm"
#include "bitmap/misc/userstat.xpm"
#include "bitmap/misc/ch1.xpm"
#include "bitmap/misc/ch2.xpm"
#include "bitmap/misc/ch0.xpm"
#include "bitmap/misc/mainbg.xpm"
#include "bitmap/misc/logo.xpm"
#include "bitmap/misc/asia.xpm"
#include "bitmap/misc/presskey.xpm"
#include "bitmap/misc/current.xpm"

#include "bitmap/misc/btkan.xpm"
#include "bitmap/misc/btreach.xpm"
#include "bitmap/misc/bttie.xpm"
#include "bitmap/misc/btpon.xpm"
#include "bitmap/misc/bttumo.xpm"
#include "bitmap/misc/btron.xpm"
#include "bitmap/misc/mt_rest_.xpm"

#include "bitmap/misc/num1.xpm"
#include "bitmap/misc/num2.xpm"
#include "bitmap/misc/num3.xpm"
#include "bitmap/misc/num4.xpm"


static char **n_hon_data[] =
{
  0,
  fbitmap (n1hon),
  fbitmap (n2hon),
  fbitmap (n3hon),
  fbitmap (n4hon),
  fbitmap (n5hon),
};
static char **digit_data[] =
{
  fbitmap (digit0),
  fbitmap (digit1),
  fbitmap (digit2),
  fbitmap (digit3),
  fbitmap (digit4),
  fbitmap (digit5),
  fbitmap (digit6),
  fbitmap (digit7),
  fbitmap (digit8),
  fbitmap (digit9),
};

static char **posdigit_data[] =
{
  0,
  fbitmap (num1),
  fbitmap (num2),
  fbitmap (num3),
  fbitmap (num4),
};


static char **bturn_data[] =
{
  fbitmap (bigton),
  fbitmap (bignan),
  fbitmap (bigsya),
  fbitmap (bigpei),
};

void
read_decorate_pixmaps ()
{
  int i;

  for (i = 0; i < 10; i++)
    {
      read_pix_idx (digit_data[i], pm_digit[i]);
    }
  for (i = 1; i < 6; i++)
    {
      read_pix_idx (n_hon_data[i], pm_n_hon[i]);
    }
  for (i = 0; i < 5; i++)
    {
      if (posdigit_data[i])
	{
	  read_pix_idx (posdigit_data[i], pm_posdigit[i]);
	}
    }

  for (i = 0; i < 4; i++)
    {
      read_pix_idx (bturn_data[i], pm_bturn_names[i]);
    }

  read_pix (ie, pm_ie);

  read_pix (kyoku, pm_sturn_sfx);
  read_pix (honba, pm_sturn_cnt_sfx);
  read_pix (reststr, pm_rest_str);
  read_pix (mt_rest_str, pm_mt_rest_str);
  read_pix (dorastr, pm_dora_str)
  read_pix (uradorastr, pm_uradora_str);
  read_pix (userstate, pm_user_state);

  read_pix (ch0, pm_choice_state[0]);
  read_pix (ch1, pm_choice_state[1]);
  read_pix (ch2, pm_choice_state[2]);
  read_pix (current, pm_current);

  read_pix (btkan, pm_btkan);
  read_pix (btreach, pm_btreach);
  read_pix (btpon, pm_btpon);
  read_pix (bttie, pm_bttie);
  read_pix (btron, pm_btron);
  read_pix (bttumo, pm_bttumo);
}


void
read_openning_pixmaps ()
{
  read_pix (logo, pm_logo);
  read_pix (presskey, pm_presskey);
  read_pix (asia, pm_asia);
  read_pix (mainbg, pm_mainbg);
}


void
actionbutton_setup ()
{
  extern int yellow;

  lang_set_cancel(0);

  if (BUILD4) {
      lang_set_advice(0);
  } else {
      widget_configure (advice_button, resource_pixmap, pm_b_advice.pm);
  }
  widget_configure (kan_button, resource_pixmap, pm_btkan.pm);
  widget_configure (tumo_button, resource_pixmap, pm_bttumo.pm);
  widget_configure (tie_button, resource_pixmap, pm_bttie.pm);
  widget_configure (pon_button, resource_pixmap, pm_btpon.pm);
  widget_configure (ron_button, resource_pixmap, pm_btron.pm);
  widget_configure (reach_button, resource_pixmap, pm_btreach.pm);
  widget_configure (advice_button, resource_pixmap_mask, pm_b_advice.Mpm);
  widget_configure (kan_button, resource_pixmap_mask, pm_btkan.Mpm);
  widget_configure (tumo_button, resource_pixmap_mask, pm_bttumo.Mpm);
  widget_configure (tie_button, resource_pixmap_mask, pm_bttie.Mpm);
  widget_configure (pon_button, resource_pixmap_mask, pm_btpon.Mpm);
  widget_configure (ron_button, resource_pixmap_mask, pm_btron.Mpm);
  widget_configure (reach_button, resource_pixmap_mask, pm_btreach.Mpm);
  lang_set();
}
