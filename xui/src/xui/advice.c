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
 * advice.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:53:25 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:53:30 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "xnet.h"
#include "xnetmaj.h"
#include "pixread.h"
#include "pmutil.h"
#include "ahand.h"

extern int in_choice;
extern int in_res;

#if 0
#include "bitmap/misc/advtumo.xpm"
#include "bitmap/misc/advreach.xpm"
#include "bitmap/misc/advtahai.xpm"
#include "bitmap/misc/advice.xpm"
#include "bitmap/misc/b_advice.xpm"
#include "bitmap/misc/advron.xpm"
#include "bitmap/misc/advpon.xpm"
#include "bitmap/misc/advtie.xpm"
#include "bitmap/misc/advkan.xpm"
#include "bitmap/misc/advesc.xpm"
#else
#if 1
#include "bitmap/misc/adv.xpm"
#else
#include "./adv.xpm"
#endif
#endif


struct gpixmap2 pm_advtumo;
struct gpixmap2 pm_advreach;
struct gpixmap2 pm_advtahai;
struct gpixmap2 pm_advice;
struct gpixmap2 pm_advron;
struct gpixmap2 pm_advpon;
struct gpixmap2 pm_advtie;
struct gpixmap2 pm_advkan;
struct gpixmap2 pm_advesc;
struct gpixmap2 pm_advtenpai;

void
read_advice_pixmaps ()
{
  read_pix (advtumo, pm_advtumo);
  read_pix (advreach, pm_advreach);
  read_pix (advtahai, pm_advtahai);
  read_pix (advice, pm_advice);
  read_pix (b_advice, pm_b_advice);
  read_pix (advron, pm_advron);
  read_pix (advpon, pm_advpon);
  read_pix (advtie, pm_advtie);
  read_pix (advkan, pm_advkan);
  read_pix (advesc, pm_advesc);
  read_pix (advtenpai, pm_advtenpai);
}

integer_t
tenpai_p (global_t *gp, integer_t who)
{
  int i, n = hand[who].closed_num, tenpai = 0;
  ahand_t a;

  for (i = 0; i < n; i++)
    {
      ah_new (&a, &hand[who], i);
      ah_analize (&a, 0);
      if (result_rest == 1)
        tenpai = 1;
    }
  return tenpai;
}

static integer_t
furo_p (global_t *gp, int who)
{
  int i;
  for (i = 0; i < 4; i++)
    {
      if (hand[who].opened_kind[i] != 0)
        return 1;
    }
  return 0;
}

static void
advice (global_t *gp)
{
  widget_t canvas = advicepmframe;
  pmcontainer_clear (advicepmframe);

  if (!flag_adviceselect)
    {
      goto info;
    }

  if (in_choice)
    {
      int save, ask;
      save = ask_attr;
      analize (gp, vself);
      ask = ask_attr;
      ask_attr = save;

      if (T_GET (ask) == T_HN_TSM)
        {
          print_colorpixmap (pm_advtumo);
        }
      else if (T_GET (ask) == T_RV_RCH)
        {
          if (furo_p (gp, vself))
            {
              goto tenpai;
            }
          else
            {
              print_colorpixmap (pm_advreach);
            }
          print_colorpixmap2 (pai2pix (ask_pai, 3, 0, 0));
        }
      else
        {
        tenpai:
          print_colorpixmap (pm_advtahai);
          print_colorpixmap2 (pai2pix (ask_pai, 3, 0, 0));
          if (tenpai_p (gp, vself))
            {
              print_colorpixmap (pm_advtenpai);
            }
        }
      widget_display (advicepmframe);
      widget_flush (advicepmframe);

    }
  else if (in_res)
    {
      int ask, how;
      int save;

      save = ask_attr;
      ask = analize_res (gp, &how);
      ask_attr = save;

      if (ask == R_TIE)
        {
          print_colorpixmap (pm_advtie);
          print_colorpixmap2 (pai2pix (how, 3, 0, 0));
        }
      else if (ask == R_RON)
        {
          print_colorpixmap (pm_advron);
        }
      else if (ask == R_PON)
        {
          print_colorpixmap (pm_advpon);
        }
      else if (ask == R_KAN)
        {
          print_colorpixmap (pm_advkan);
        }
      else
        {
          print_colorpixmap (pm_advesc);
        }
      widget_display (advicepmframe);
      widget_flush (advicepmframe);
    }
  advice_policy ();
info:

  hand_pai_advice_info_display (gp);
  widget_display (hand_pai_advice_info);
  widget_flush (hand_pai_advice_info);
}

char *homedir;

static void
setup_homedir ()
{
  homedir = getenv ("HOME");
  if (!homedir)
    homedir = "/tmp";
}

void
quick_log_button_callback callbackarg_param
{
#define QUICK_LOG "netmaj.ql"
  extern global_t *game_global;
  FILE *fp;
  char buffer[256];

  snd (audioplay_quicklog ());
  setup_homedir ();
  sprintf (buffer, "%s/%s", homedir, QUICK_LOG);
  fp = fopen (buffer, "a");
  if (fp)
    {
      log_play (game_global, fp);
      fclose (fp);
    }
}

void
advice_button_callback callbackarg_param
{
  extern global_t *game_global;
  extern int in_result;

  if (in_result)
    {
      quick_log_button_callback (w, val, arg);
    }
  else
    {
      snd (audioplay_advice ());
      advice (game_global);
    }
}

void
advice_clear ()
{
  pmcontainer_clear (advicepmframe);
  pmcontainer_clear (hand_pai_advice_info);
}

void
advice_clear_flush ()
{
  advice_clear ();
  widget_display (advicepmframe);
  widget_display (hand_pai_advice_info);
  widget_flush (advicepmframe);
  widget_flush (hand_pai_advice_info);
}

/* end of file */
