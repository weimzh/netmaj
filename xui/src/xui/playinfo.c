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
 * pinfo.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Aug 17 23:07:49 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"
#include "gt/widget/text.h"
#include "pfinfo.h"

widget_t pinfo;
extern integer_t xwidth, xheight;
extern struct playerinfo playerinfo;

static widget_t pinfoheaderpmframe;
static widget_t playerinfopmframe, playerinfopointframe[21];
static widget_t yakupmframe[2][60];
static widget_t dirnameframes[50];
static void playinfo_keywait_callback ();
extern integer_t tomato, red, yellow, white, blue, purple, darkorange;
extern integer_t yellow, white, darkgreen, black;

static void pinfo_dir_display ();
static void pinfo_bg_init ();
static void pinfo_bg_end ();
static void pinfo_display ();

static void
select_playername callbackarg_param
{
  char *text = text_text (w);
  if (!text)
    {
      return;
    }
  if (!*text)
    {
      return;
    }

  widget_configure (w, resource_textcolor, black);
  widget_model_change (w, yellow, black, black, black);
  widget_configure (w, resource_border_width, 1);
  widget_configure (w, resource_relief, relief_frame);

  widget_display (w);
  widget_flush (w);

  read_player_info (text);
  pinfo_display (text);
  widget_display (pinfo);
  widget_flush (pinfo);

  widget_configure (w, resource_textcolor, white);
  widget_model_change (w, get_basiccolorpixel (), black, black, black);
  widget_configure (w, resource_border_width, 0);
  widget_configure (w, resource_relief, 0);

  widget_display (w);
  widget_flush (w);
}

static widget_t
create_pinfo_frame (widget_t parent)
{
  widget_t frame, aframe, bframe, hframe, dirframe;
  widget_t okbutton;
  integer_t i, j, k;
  widget_t xframe, yframe, zframe[3];
  integer_t height = 0;

  read_pinfo_pixmap ();

  frame = widget_create ("frame", parent);
  widget_set_geometry (frame, xwidth, xheight);

  hframe = widget_create ("frame", frame);
  widget_configure (hframe, resource_wrap, 0);
  widget_configure (hframe, resource_packed, resource_horizontal);
  widget_set_geometry (hframe, xwidth, 32);

  pinfoheaderpmframe = widget_create ("text", hframe);
  widget_set_geometry (pinfoheaderpmframe, xwidth, 32);
  widget_configure (pinfoheaderpmframe, resource_textfont, XLARGETIMESIB);
  widget_configure (pinfoheaderpmframe, resource_text, "      NETMAJ Player Information");
  widget_configure (pinfoheaderpmframe, resource_textcolor, tomato);
  widget_configure (pinfoheaderpmframe, resource_textshadow, 1);
  height += 48;

  dirframe = widget_create ("frame", frame);
  widget_configure (dirframe, resource_packed, resource_horizontal);
  widget_set_geometry (dirframe, xwidth, 72);
  height += 72;

  {
    int i, j, k = 0;
    for (i = 0; i < 8; i++)
      {
	widget_t f;
	f = widget_create ("frame", dirframe);
	widget_set_geometry (f, xwidth / 8, 72);
	for (j = 0; j < 72 / 18; j++)
	  {
	    dirnameframes[k] = widget_create ("text", f);
	    widget_set_geometry (dirnameframes[k], xwidth / 8, 18);
	    widget_add_callback (dirnameframes[k],
				 resource_buttonpress_callback,
				 select_playername, 0);
	    widget_configure (dirnameframes[k], resource_transparent, 0);
	    widget_configure (dirnameframes[k], resource_textfont, TINYJP);
	    k++;
	  }
	widget_pack (f);
      }
  }
  widget_pack (dirframe);

#define VALCOLWIDTH  88
#define TBLBMWIDTH  110
#define TBLBMHEIGHT 350
  aframe = widget_create ("frame", frame);
  widget_configure (aframe, resource_wrap, 0);
  widget_configure (aframe, resource_packed, resource_horizontal);
  widget_set_geometry (aframe, xwidth, TBLBMHEIGHT);

  xframe = widget_create ("frame", aframe);
  widget_configure (xframe, resource_wrap, 0);
  widget_configure (xframe, resource_packed, resource_horizontal);
  widget_set_geometry (xframe, TBLBMWIDTH+VALCOLWIDTH, TBLBMHEIGHT);

  playerinfopmframe = widget_create ("pmcontainer", xframe);
  widget_set_geometry (playerinfopmframe, TBLBMWIDTH, TBLBMHEIGHT);

  yframe = widget_create ("frame", xframe);
  widget_configure (yframe, resource_wrap, 0);
  widget_set_geometry (yframe, VALCOLWIDTH, TBLBMHEIGHT);
  widget_pack (xframe);


  bframe = widget_create ("frame", frame);
  widget_configure (bframe, resource_wrap, 0);
  widget_configure (bframe, resource_packed, resource_horizontal);
  widget_set_geometry (bframe, xwidth, BUTTON_H);

  for (i = 0; i < 19; i++)
    {
      playerinfopointframe[i] = widget_create ("text", yframe);
      widget_set_geometry (playerinfopointframe[i], VALCOLWIDTH, 18);
      widget_configure (playerinfopointframe[i], resource_relief, relief_sunken);
      widget_configure (playerinfopointframe[i], resource_border_width, 1);
      widget_configure (playerinfopointframe[i], resource_textfont, TINYJP);
    }
  widget_pack (yframe);

  for (i = 0; i < 3; i++)
    {
      zframe[i] = widget_create ("frame", aframe);
      widget_configure (zframe[i], resource_wrap, 0);
      widget_set_geometry (zframe[i], 154, xheight - (height + BUTTON_H));
    }
  widget_pack (aframe);
  for (k = 0, j = 0; j < 3; j++)
    {
      for (i = 0; i < 19; i++)
	{
	  widget_t w,ww;
	  w = widget_create ("frame", zframe[j]);
	  widget_set_geometry (w, 154, 18);
	  widget_configure (w, resource_packed, resource_horizontal);

	  ww = widget_create("text", w);
	  widget_configure (ww, resource_text, " ");
	  widget_set_geometry (ww, 8, 18);
	  yakupmframe[0][k] = widget_create ("text", w);
	  widget_set_geometry (yakupmframe[0][k], 154-8-32, 18);
	  yakupmframe[1][k] = widget_create ("text", w);
	  widget_set_geometry (yakupmframe[1][k], 32, 18);
	  widget_configure (yakupmframe[0][k], resource_textfont, TINYJP);
	  widget_configure (yakupmframe[1][k], resource_textfont, TINYJP);
	  k++;
	  widget_pack (w);
	}
      widget_pack (zframe[j]);
    }
  widget_configure (bframe, resource_frame_start, 270);
  playinfookbutton = okbutton = button_create (bframe, BUTTON_W, BUTTON_H,
					       "ok", yellow, TIMESI,
				 (void *) playinfo_keywait_callback, frame);
  widget_pack (bframe);
  widget_pack (frame);
  widget_fix (frame);
  widget_set_geometry (frame, xwidth, xheight);

  if (pm_pinfo.pm)
    {
      pmcontainer_print (playerinfopmframe, pm_pinfo);
    }

  return frame;
}

void
pinfo_init ()
{
}

void
pinfo_clear ()
{
  integer_t i;

  for (i = 0; i < 19; i++)
    {
      if (playerinfopointframe[i])
	{
	  widget_configure (playerinfopointframe[i], resource_text, "");
	}
    }
  for (i = 0; i < 60; i++)
    {
      if (yakupmframe[0][i])
	{
	  widget_configure (yakupmframe[0][i], resource_text, "");
	  widget_configure (yakupmframe[1][i], resource_text, "");
	}
    }

}

static void
pinfo_display (char *name)
{
  char buffer[64];
  pinfo_clear ();

  if (!playerinfo.rname[0]) strcpy(playerinfo.rname,name);
  widget_configure (playerinfopointframe[0], resource_text, playerinfo.rname);

  sprintf (buffer, "%10d", (playerinfo.pnt * 100) + (playerinfo.tn * 500));
  widget_configure (playerinfopointframe[1], resource_text, buffer);
  sprintf (buffer, "%10d", playerinfo.game);
  widget_configure (playerinfopointframe[2], resource_text, buffer);
  sprintf (buffer, "%10d", playerinfo.pn);
  widget_configure (playerinfopointframe[3], resource_text, buffer);
  sprintf (buffer, "%10d", playerinfo.pnt);
  widget_configure (playerinfopointframe[4], resource_text, buffer);
  sprintf (buffer, "%10d", playerinfo.tn);
  widget_configure (playerinfopointframe[5], resource_text, buffer); 

  if (playerinfo.game)
    {
      sprintf (buffer, "%4d %3.1f%%", playerinfo.juni[1], (100.0 * playerinfo.juni[1]) / playerinfo.game);
      widget_configure (playerinfopointframe[6], resource_text, buffer);
      sprintf (buffer, "%4d %3.1f%%", playerinfo.juni[2], (100.0 * playerinfo.juni[2]) / playerinfo.game);
      widget_configure (playerinfopointframe[7], resource_text, buffer);
      sprintf (buffer, "%4d %3.1f%%", playerinfo.juni[3], (100.0 * playerinfo.juni[3]) / playerinfo.game);
      widget_configure (playerinfopointframe[8], resource_text, buffer);
      sprintf (buffer, "%4d %3.1f%%", playerinfo.juni[4], (100.0 * playerinfo.juni[4]) / playerinfo.game);
      widget_configure (playerinfopointframe[9], resource_text, buffer);
    }

  if (playerinfo.pn)
    {
      sprintf (buffer, "%10d", playerinfo.ron_cnt + playerinfo.tumo_cnt,
      (100.0 * (playerinfo.ron_cnt + playerinfo.tumo_cnt)) / playerinfo.pn);
      widget_configure (playerinfopointframe[10], resource_text, buffer);

      sprintf (buffer, "%10d", playerinfo.nn, (100.0 * playerinfo.nn) / playerinfo.pn);
      widget_configure (playerinfopointframe[11], resource_text, buffer);
      sprintf (buffer, "%10d", playerinfo.rn, (100.0 * playerinfo.rn) / playerinfo.pn);
      widget_configure (playerinfopointframe[12], resource_text, buffer);
      sprintf (buffer, "%10d", playerinfo.tumo_cnt, (100.0 * playerinfo.tumo_cnt) / playerinfo.pn);
      widget_configure (playerinfopointframe[13], resource_text, buffer);
      sprintf (buffer, "%10d", playerinfo.ron_cnt, (100.0 * playerinfo.ron_cnt) / playerinfo.pn);
      widget_configure (playerinfopointframe[14], resource_text, buffer);
      sprintf (buffer, "%10d", playerinfo.furi_cnt, (100.0 * playerinfo.furi_cnt) / playerinfo.pn);
      widget_configure (playerinfopointframe[15], resource_text, buffer);
    }
  if (playerinfo.tumo_cnt > 0)
    {
      sprintf (buffer, "%10d", playerinfo.tumo_pnt / playerinfo.tumo_cnt, playerinfo.max_tumo_pnt);
      widget_configure (playerinfopointframe[16], resource_text, buffer);
    }
  if (playerinfo.ron_cnt > 0)
    {
      sprintf (buffer, "%10d", playerinfo.ron_pnt / playerinfo.ron_cnt, playerinfo.max_ron_pnt);
      widget_configure (playerinfopointframe[17], resource_text, buffer);
    }
  if (playerinfo.furi_cnt > 0)
    {
      sprintf (buffer, "%10d", playerinfo.furi_pnt / playerinfo.furi_cnt, playerinfo.max_furi_pnt);
      widget_configure (playerinfopointframe[18], resource_text, buffer);
    }

  {
    int i, k = 0;
    for (i = 0; i < 58; i++)
      {
	gpixmap2_t yaku;
	if (playerinfo.yakus[i] > 0)
	  {
	   widget_configure (yakupmframe[0][k], resource_text, result_str[i]);
	   sprintf (buffer, "%4d", playerinfo.yakus[i]);
	   widget_configure (yakupmframe[1][k], resource_text, buffer);
	   k++;
	  }
      }				/* for */
    for (; k < 60; k++) if (yakupmframe[0][k])
      {
	widget_configure (yakupmframe[0][k], resource_text, "");
	widget_configure (yakupmframe[1][k], resource_text, "");
      }
  }
}

void
pinfo_dir_clear ()
{
  int i;
  for (i = 0; i < 40; i++)
    {
      if (dirnameframes[i] && text_have_text_p (dirnameframes[i]))
	{
	  widget_configure (dirnameframes[i], resource_text, "");
	}
    }
  pinfo_clear ();
}

static void *arg_name;

void
pinfo_callback callbackarg_param
{
  extern widget_t about;
  char *name = 0;
    snd (audioplay_info ());
  if (!pinfo)
    {
      extern widget_t root_widget;
        pinfo = create_pinfo_frame (root_widget);
        lang_set ();
    }
  widget_unmap (about);
  pinfo_dir_clear ();
  pinfo_dir_display ();

  arg_name = (void *) val;
  if (!val)
    {
      name = flag_username;
    }
  else
    {
      name = ((char *) val);
    }

  if (name)
    {
      read_player_info (name);
      pinfo_display (name);
    }

  widget_map (pinfo);
  widget_display (pinfo);
  widget_flush (pinfo);

  pinfo_bg_init ();
}

static integer_t
sort_name (struct playerinfo *list, struct playerinfo *p)
{
  char *sp = list->name;
  char *tp = p->name;
  while (*sp && *tp)
    {
      if (*sp > *tp)
	{
	  return 1;
	}
      sp++, tp++;
    }
  if (*tp)
    return 0;
  return 1;
}

static void
pinfo_dir_display ()
{
  struct playerinfo *base = 0, *p = 0;
  int i;

  if (0 == read_player_first_info ())
    {
      return;
    }
  do
    {
      p = (struct playerinfo *) xmalloc (sizeof (struct playerinfo));
      memcpy (p, &playerinfo, sizeof (struct playerinfo));
      base = (struct playerinfo *) playerinfo_sort_append (base, p, sort_name);
    }
  while (read_player_next_info ());
  if (!p)
    {
      return;
    }

  p = base;
  i = 0;
  while (p)
    {
      widget_configure (dirnameframes[i], resource_text, p->name);
      p = p->next;
      i++;
    }
  while (base)
    {
      p = base;
      base = base->next;
      xfree ((void *) p);
    }
}

static void
playinfo_keywait_callback ()
{
  pinfo_bg_end ();
  widget_unmap (pinfo);

  if (arg_name)
    {
      rank_callback (0, 0, 0);
    }
  else
    {
      board_redraw (1);
    }
  arg_name = 0;
}

static int
bg_action ()
{
  static integer_t *colors[] =
    {&tomato, &red, &yellow, &white, &blue, &purple, &darkorange};

  static int n;

  widget_configure (pinfoheaderpmframe, resource_textcolor,
		    *colors[(unsigned int) (n++) % 7]);
  widget_display (pinfoheaderpmframe);
  widget_flush (pinfoheaderpmframe);

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

static void
pinfo_bg_init ()
{
  stop_timer ();
  set_timer_handler (bg_handler, 7);
}


static void
pinfo_bg_end ()
{
  clear_timer_handler ();
}
