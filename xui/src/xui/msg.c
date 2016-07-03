/*  NETMAJ-XUI -- X User Interface --  Copyright Notice
 *
 *      Copyright (c) 1995, 1996 Todo Software All rights reserved.
 *      Copyright (C) 1994, 1995, 1996 Koji Suzuki (suz@kt.rim.or.jp)
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
#include "xnetmaj.h"
#include "pixread.h"

extern integer_t xwidth, xheight;
extern global_t *game_global;
extern global_t *hora_global;

extern int play_speed;
int player_cnt;

static struct
{
  integer_t tick;
  widget_t p;                   /* parent */
  widget_t f;                   /* frame */
  widget_t m;                   /* msg */
}
comm[4], decl[4], playframe;

#include "bitmap/misc/tting.xpm"
#include "bitmap/misc/failconn.xpm"
#include "bitmap/misc/connect.xpm"
#include "bitmap/misc/disconne.xpm"
#include "bitmap/misc/msgtumo.xpm"
#include "bitmap/misc/msgron.xpm"
#include "bitmap/misc/msgreach.xpm"
#include "bitmap/misc/msgtie.xpm"
#include "bitmap/misc/msgpon.xpm"
#include "bitmap/misc/msgkan.xpm"
#include "bitmap/misc/mark0.xpm"
#include "bitmap/misc/mark1.xpm"
#include "bitmap/misc/mark2.xpm"
#include "bitmap/misc/mark3.xpm"

struct gpixmap2 pm_msgconnectting;
struct gpixmap2 pm_msgfailconnect;
struct gpixmap2 pm_msgconnect;
struct gpixmap2 pm_msgdisconnect;
struct gpixmap2 pm_msgtumo;
struct gpixmap2 pm_msgron;
struct gpixmap2 pm_msgreach;
struct gpixmap2 pm_msgtie;
struct gpixmap2 pm_msgpon;
struct gpixmap2 pm_msgkan;
struct gpixmap2 pm_mark0;
struct gpixmap2 pm_mark1;
struct gpixmap2 pm_mark2;
struct gpixmap2 pm_mark3;

void terminal_add (char *text, int from);
void terminal_init ();

static void
playmessage_clear ()
{
  widget_xunmap (playframe.f);
  widget_flush (playframe.p);
}

void
message_unmap ()
{
  int i;
  for (i = 0; i < 4; i++)
    {
      widget_xunmap (comm[i].f);
    }
  for (i = 0; i < 4; i++)
    {
      widget_xunmap (decl[i].f);
    }
  widget_xunmap (playframe.f);
}

void
message_init (widget_t r[4], widget_t msg[4])
{
  integer_t i;
  widget_t frame;

  read_pix (tting, pm_msgconnectting);
  read_pix (failconn, pm_msgfailconnect);
  read_pix (connect, pm_msgconnect);
  read_pix (disconne, pm_msgdisconnect);
  read_pix (msgtumo, pm_msgtumo);
  read_pix (msgtumo, pm_msgtumo);
  read_pix (msgron, pm_msgron);
  read_pix (msgreach, pm_msgreach);
  read_pix (msgtie, pm_msgtie);
  read_pix (msgpon, pm_msgpon);
  read_pix (msgkan, pm_msgkan);
  read_pix (mark0, pm_mark0);
  read_pix (mark1, pm_mark1);
  read_pix (mark2, pm_mark2);
  read_pix (mark3, pm_mark3);

  for (i = 0; i < 4; i++)
    {
      comm[i].p = r[i];
      if (i == 3)
        {
          comm[i].f = widget_create ("frame", r[i]);
        }
      else
        {
          comm[i].f = widget_create ("frame", msg[i]);
        }
      widget_configure (comm[i].f, resource_border_width, 2);
      widget_configure (comm[i].f, resource_relief, relief_frame);
      comm[i].m = widget_create ("text", comm[i].f);
      widget_configure (comm[i].m, resource_border_width, 2);
      widget_configure (comm[i].m, resource_relief, relief_frame);
      widget_set_geometry (comm[i].f, name_width - 4, MSG_H + 4);
      widget_set_geometry (comm[i].m, name_width - 8, MSG_H);

      decl[i].p = r[i];
      decl[i].f = widget_create ("frame", r[i]);
      widget_configure (decl[i].f, resource_border_width, 2);
      widget_configure (decl[i].f, resource_relief, relief_frame);
      decl[i].m = widget_create ("pmcontainer", decl[i].f);
      widget_set_geometry (decl[i].f, name_width - 4, MSG_H + 4);
      widget_set_geometry (decl[i].m, name_width - 8, MSG_H);
      widget_pack (decl[i].f);
      widget_pack (comm[i].f);
      {
        extern integer_t yellowgreen, yellow, tann, white, gray, black;
        widget_set_color_change (decl[i].f, tann, tann, tann, tann);
        widget_set_color_change (comm[i].f, tann, tann, tann, tann);
        /*     widget_set_color_change (decl[i].m, white, white, white, white);   */
        /*     widget_set_color_change (comm[i].m, gray, gray, gray, gray);   */
        widget_configure (decl[i].f, resource_cannot_change_model, 1);
/*      widget_configure(decl[i].m, resource_cannot_change_model, 1);   */
        widget_configure (comm[i].f, resource_cannot_change_model, 1);
/*      widget_configure(comm[i].m, resource_cannot_change_model, 1);  */

        widget_configure (decl[i].m, resource_textcolor, white);
        widget_configure (comm[i].m, resource_textcolor, white);
      }
    }

  playframe.p = r[0];
  playframe.f = widget_create ("frame", r[0]);
  widget_configure (playframe.f, resource_border_width, 2);
  playframe.m = widget_create ("pmcontainer", playframe.f);
  widget_set_geometry (playframe.f, 94, MSG_H + 4);
  widget_set_geometry (playframe.m, 90, MSG_H);
  widget_pack (playframe.f);
  {
    int i;
    for (i = 0; i < 4; i++)
      {
        widget_configure (comm[i].f, resource_packed, 0);
      }
    for (i = 0; i < 4; i++)
      {
        widget_configure (decl[i].f, resource_packed, 0);
      }
    widget_configure (playframe.f, resource_packed, 0);
  }
  if (BUILD4)
    terminal_init ();
}


int
msg_tick ()
{
  int i;
  for (i = 0; i < 4; i++)
    {
      if (widget_map_p (comm[i].f))
        {
          comm[i].tick++;
          if (comm[i].tick > flag_talktick)
            {
              comm[i].tick = 0;
              widget_xunmap (comm[i].f);
              widget_flush (comm[i].p);
            }
        }
    }
  return 0;
}

void
msg_bg_init ()
{
  stop_timer ();
  set_timer_handler (msg_tick, 2);
}

void
msg_bg_done ()
{
  stop_timer ();
}


static void
popup_playmessage (gpixmap2_t pm)
{
  if (!widget_map_p (playframe.p))
    return;

  pmcontainer_print (playframe.m, (*pm));
  widget_map (playframe.f);
  widget_display (playframe.f);
  widget_flush (playframe.f);
  xsleep (play_speed * 3 + 2);
  playmessage_clear ();
}

void
popup_declmessage (i, pm, timeout)
     gpixmap2_t pm;
{
  if (!widget_map_p (decl[i].p))
    return;

  pmcontainer_print (decl[i].m, (*pm));
  widget_map (decl[i].f);
  widget_display (decl[i].f);
  widget_flush (decl[i].f);
  xsleep (timeout);
  widget_xunmap (decl[i].f);
  widget_flush (decl[i].p);
}

void
popup_commmessage (int i, char *str, int timeout)
{
  integer_t len;
  integer_t width;
  if (BUILD4)
    {
      terminal_add (str, i);
      return;
    }

  if (!flag_talk)
    {
      return;
    }
  if (!widget_map_p (comm[i].p))
    {
      return;
    }
  widget_configure (comm[i].m, resource_text, str);
  widget_configure (comm[i].m, resource_textfont, MEDIUMJP);
  width = text_string_n_pixels (comm[i].m, str, len = strlen (str));
  widget_set_geometry (comm[i].f, width + 8, MSG_H + 4);
  widget_set_geometry (comm[i].m, width + 4, MSG_H);
  widget_map (comm[i].f);
  widget_display (comm[i].f);
  widget_flush (comm[i].f);
  xsleep (timeout);
  widget_xunmap (comm[i].f);
  widget_flush (comm[i].p);
}

static void
undisplay_commmessage ()
{
  int i;
  for (i = 0; i < 4; i++)
    {
      if (widget_map_p (comm[i].f))
        {
          widget_xunmap (comm[i].f);
          widget_flush (comm[i].p);
        }
    }
}

void
ui_message_fail_connect ()
{
  extern widget_t root_widget;
  if (widget_eventsqueued (root_widget))
    {
      widget_display (root_widget);
      widget_flush (root_widget);
    }
  popup_playmessage (&pm_msgfailconnect);
}

void
ui_message_connecting ()
{
  extern widget_t root_widget;
  if (widget_eventsqueued (root_widget))
    {
      widget_display (root_widget);
      widget_flush (root_widget);
    }
  popup_playmessage (&pm_msgconnectting);
}

void
ui_message_connect ()
{
  player_cnt = 1;
  popup_playmessage (&pm_msgconnect);
}

void
ui_message_disconnect ()
{
  player_cnt = 0;
  lang_set_auto ();
  if (auto_button)
    {
      widget_display (auto_button);
      widget_flush (auto_button);
    }
  popup_playmessage (&pm_msgdisconnect);
}

void
ui_message_reach (i)
{
  snd (audioplay_declreach ());
  i = pos2who (i);
  popup_declmessage (mytblpos (i), &pm_msgreach, play_speed + flag_talktick);
}

void
ui_message_naki (i, kind)
{
  i = pos2who (i);
  if (kind == 0)
    {
      popup_declmessage (mytblpos (i), &pm_msgtie,
                         play_speed + flag_talktick);
    }
  else if (kind == 1)
    {
      popup_declmessage (mytblpos (i), &pm_msgpon,
                         play_speed + flag_talktick);
    }
  else if (kind == 2)
    {
      popup_declmessage (mytblpos (i), &pm_msgkan,
                         play_speed + flag_talktick);
    }
}

integer_t flag_pause;

void
wait4key ()
{
  widget_unmap (about_button);
  widget_unmap (flush_button);
  widget_unmap (visible_button);

  lang_set_cancel (1);
  widget_map (cancel_button);
  widget_display (cancel_button);
  widget_flush (cancel_button);
  wait_key ();
  lang_set_cancel (0);
  widget_unmap (cancel_button);
  lang_set ();
}

void
stop_config ()
{
  char *p;
  integer_t color;
  extern integer_t yellow, white;
  if (!stop_button)
    return;
  if (flag_pause == 0)
    {
      color = white;
      p = "no pause";
    }
  else
    {
      color = yellow;
      p = "pause";
    }

  widget_configure (stop_button, resource_textcolor, color);
  widget_configure (stop_button, resource_text, p);

}

void
stop_button_callback callbackarg_param
{
  flag_pause = !flag_pause;
  stop_config ();
}


ui_message_tumo (i)
{
  snd (audioplay_decltumo ());
  i = pos2who (i);
  popup_declmessage (mytblpos (i), &pm_msgtumo, play_speed + flag_talktick);
  flag_not_needredraw = 1;
}

ui_message_ron (i)
{
  snd (audioplay_declron ());
  i = pos2who (i);
  popup_declmessage (mytblpos (i), &pm_msgron, play_speed + flag_talktick);
  flag_not_needredraw = 1;
}

ui_message_player (gp)
     global_t *gp;
{
  int i;
  flag_not_needdraw_hand = 1;
  player_cnt = 0;
  for (i = 0; i < 4; i++)
    {
      if (strcmp ("---------", player[i].name))
        player_cnt++;
    }
  lang_set_auto ();
  if (auto_button)
    {
      widget_display (auto_button);
      widget_flush (auto_button);
    }
}

ui_message_play (gp)
     global_t *gp;
{

  rule_notify ();
  lang_set_auto ();
  if (auto_button)
    {
      widget_display (auto_button);
      widget_flush (auto_button);
    }
/*  flag_not_needredraw = 1; */
}

void
speed_config ()
{
  extern integer_t white, yellow;
  char buf[64];
  if (!speed_button)
    {
      return;
    }
  {
    sprintf (buf, "speed %d", flag_gamespeed + 1);
    widget_configure (speed_button, resource_textcolor, white);
  }
  play_speed = flag_gamespeed * 2;
  widget_configure (speed_button, resource_text, buf);
  widget_display (speed_button);
  widget_flush (speed_button);
}

void
speed_callback callbackarg_param
{
  flag_gamespeed++;
  if (flag_gamespeed >= 9)
    {
      flag_gamespeed = 0;
    }
  speed_config ();
}

extern integer_t yellow, black, white;

/* from player */
void
comment (int from, int i, char *body)
{
  char msgbuffer[256];
  char execbuffer[256];

  comment_divide (body, msgbuffer, execbuffer);
  comment_exec_command (execbuffer);

  from = pos2who (from);
  popup_commmessage (mytblpos (from), msgbuffer,
                     flag_talktick * 3 + play_speed);
}

static char *
replacecntl (char *s)
{
  char *org = s;
  while (*s)
    {
      if (0 < (*s) && (*s) < ' ')
        {
          *s = ' ';
        }
      s++;
    }
  return org;
}

char *analize_message (global_t *);

void
advice_policy ()
{
  popup_commmessage (3, replacecntl (analize_message (game_global)),
                     flag_talktick * 3 + play_speed);
}

/* from server */
void
popup (char *msg, int i)
{
}

void
send_play_message_callback callbackarg_param
{
  widget_configure (w, resource_textcolor, yellow);
  widget_configure (w, resource_border_width, 2);
  widget_configure (w, resource_relief, relief_sunken);
  widget_display (w);
  widget_flush (w);

  msgTalk (game_global, widget_get_text ((void *) w));
  xsleep (play_speed * 2 + 2);

  widget_configure (w, resource_textcolor, white);
  widget_configure (w, resource_border_width, 0);
  widget_configure (w, resource_relief, 0);
  widget_display (w);
  widget_flush (w);
}

void
sound_config ()
{
  extern integer_t white, yellow;
  char *s;
  int color;

  if (!sound_button)
    {
      return;
    }
  s = (flag_soundp) ? "sound on" : "silent";
  color = (flag_soundp) ? yellow : white;
  widget_configure (sound_button, resource_textcolor, color);
  widget_configure (sound_button, resource_text, s);
  widget_display (sound_button);
  widget_flush (sound_button);
}

void
sound_button_callback callbackarg_param
{
  flag_soundp = !flag_soundp;
  sound_config ();
}


#define MLINES 30
#define MCOLS  32
#define TM_W  8
#define TM_H  18

static int mlines;
static int mcols;
static int curline;
static char s_text[MLINES][MCOLS + 2];
static struct gpixmap2 s_pix[MLINES];
static widget_t tm;
static widget_t tm_text[MLINES];
static widget_t tm_pix[MLINES];

void
terminal_add (char *text, int from)
{
  int nl = 1;
  int i, j, len;
  gpixmap2_t pix;
  widget_t canvas;

  switch (from)
    {
    case 0:
      pix = &pm_mark0;
      break;
    case 1:
      pix = &pm_mark1;
      break;
    case 2:
      pix = &pm_mark2;
      break;
    case 3:
      pix = &pm_mark3;
      break;
    default:
      pix = 0;
      break;
    }

  len = 0;
  nl = ((len = strlen (text)) + mcols - 2) / (mcols - 1);
  if (curline + nl > mlines)
    {
      for (i = nl; i < mlines; i++)
        {
          s_pix[i - nl] = s_pix[i];
          memcpy (s_text[i - nl], s_text[i], MCOLS);
        }
      curline = mlines - nl;
      for (i = 0; i < nl; i++)
        {
          s_text[curline + i][0] = 0;
          s_pix[curline + i].pm = 0;
          s_pix[curline + i].Mpm = 0;
        }
    }
  if (pix)
    {
      s_pix[curline] = *pix;
    }
  j = 0;
  for (i = 0; i < len; i++)
    {
      if (text[i] & 0x80)
        {
          s_text[curline][j++] = text[i++];
          s_text[curline][j++] = text[i];
        }
      else
        {
          s_text[curline][j++] = text[i];
        }
      if (j >= mcols - 1)
        {
          curline++;
          j = 0;
        }
    }
  s_text[curline][j] = 0;
  if (s_text[curline][0])
    curline++;
  for (i = 0; i < mlines; i++)
    {
      canvas = tm_pix[i];
      canvas_clear ();
      if (s_pix[i].pm)
        {
          print_colorpixmap (s_pix[i]);
        }
      widget_configure (tm_text[i], resource_text, s_text[i]);
    }
  widget_display (tm);
  widget_flush (tm);
}

void
terminal_init ()
{
  integer_t w, h;
  widget_t work;
  extern integer_t yellowgreen, yellow, tann, white, gray, black;
  extern widget_t sub_board;
  int i, n;

  widget_get_geometry (sub_board, &w, &h);
  w -= 8;
  h -= 8;
  tm = widget_create ("frame", sub_board);
  widget_set_location (tm, 8, 8);
  widget_set_geometry (tm, w - 8, h - 8);
  mlines = n = (h - 8) / TM_H;
  mcols = (w - 8 - TM_H) / TM_W;
  curline = mlines;

  for (i = 0; i < n; i++)
    {
      work = widget_create ("frame", tm);
      widget_configure (work, resource_packed, resource_horizontal);
      tm_pix[i] = widget_create ("pmcontainer", work);
      tm_text[i] = widget_create ("text", work);
      widget_configure (tm_text[i], resource_textcolor, white);
      widget_configure (tm_text[i], resource_textfont, MEDIUMJP);
      widget_set_geometry (tm_pix[i], TM_H, TM_H);
      widget_set_geometry (tm_text[i], (w - 8 - TM_H), TM_H);
    }
  widget_pack (tm);
}
