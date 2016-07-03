/*  NETMAJ-XUI -- X User Interface --  Copyright Notice
 *
 *      Copyright (c) 1995,1996 Todo Software All rights reserved.
 *      Copyright (C) 1994, 1995, 1996 Koji Suzuki (suz@kt.rim.or.jp)
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
 *   Koji Suzuki   : suz@kt.rim.or.jp
 */
/*  -*- Mode: C -*- 
 * This file is part of xnetmaj
 * 
 * yaku.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Modifier        : suz@kt.rim.or.jp
 * Created On      : Tue Apr 11 11:13:27 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Wed Jan 31 22:26:11 1996
 *   draw yaku and points window
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 *  Copyright (C) 1996 Koji Suzuki All rights reserved
 */

#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"

extern FILE *logfile;
extern int auto_play;

widget_t point;
extern integer_t xwidth, xheight;
static widget_t point_header;
static widget_t point_results;
static widget_t point_home[4];
static widget_t point_home_user[4];
static widget_t point_home_point[4];
static widget_t point_fubase;
static widget_t point_fu;
static widget_t point_fustr;
static widget_t point_fan;
static widget_t point_fanstr;
static widget_t point_yaku[30];
static widget_t point_yaku_point[30];

static widget_t save_point;
static widget_t save_results;
static widget_t save_home_user[4];
static widget_t save_home_point[4];
static widget_t save_fubase;
static widget_t save_fu;
static widget_t save_fustr;
static widget_t save_fan;
static widget_t save_fanstr;
static widget_t save_yaku[30];
static widget_t save_yaku_point[30];

static widget_t wrap_point;
static widget_t wrap_results;
static widget_t wrap_home_point[4];
static widget_t wrap_fubase;
static widget_t wrap_fu;
static widget_t wrap_fustr;
static widget_t wrap_fan;
static widget_t wrap_fanstr;
static widget_t wrap_yaku[30];
static widget_t wrap_yaku_point[30];

extern global_t *hora_global;

void keywait_callback ();
void quit_callback ();

extern integer_t yellow, darkorange, white;

widget_t
create_basic_info_frame (char *type, widget_t parent, widget_t pmframes[6],
                         integer_t centerwidth, integer_t frame3_p,
                         integer_t button_p)
{
  widget_t frame1, frame2, frame3, frame2left, frame2center, frame2right, frame4;
  widget_t okbutton, quitbutton;
  integer_t x;
  integer_t frame1h, frame2h, frame3h, frame4h;

  frame1 = widget_create ("frame", parent);
  frame2 = widget_create ("frame", parent);
  frame3 = widget_create ("pmcontainer", parent);
  frame4 = widget_create ("frame", parent);

  widget_configure (frame1, resource_packed, resource_horizontal);
  widget_set_geometry (frame1, xwidth, TOPMARGIN);
  widget_configure (frame1, resource_wrap, 0);

  widget_configure (frame2, resource_packed, resource_horizontal);
  widget_configure (frame4, resource_packed, resource_horizontal);
  frame2left = widget_create ("pmcontainer", frame2);
  frame2center = widget_create (type, frame2);
  frame2right = widget_create ("pmcontainer", frame2);

  frame1h = TOPMARGIN;
  if (frame3_p)
    {
      frame3h = BOTTOMMARGIN;
    }
  else
    {
      frame3h = 5;
    }

  if (frame1h < 0)
    {
      frame1h = 5, frame3h = 5;
    }

  frame4h = BUTTON_H;
  frame2h = xheight - (frame1h + frame3h + frame4h + BUTTON_H);

  widget_set_geometry (frame1, xwidth, frame1h);
  widget_configure (frame1, resource_wrap, 0);
  widget_set_geometry (frame3, xwidth, frame3h);
  widget_set_geometry (frame2, xwidth, frame2h);
  {
    integer_t leftmargin = (xwidth - centerwidth) / 2;
    widget_set_geometry (frame2left, leftmargin, frame2h);
    widget_set_geometry (frame2center, xwidth - leftmargin * 2, frame2h);
    widget_set_geometry (frame2right, leftmargin, frame2h);
  }

  if (button_p)
    {
      okbutton = widget_create ("button", frame4);
/*    quitbutton = widget_create ("button", frame4);  */
      widget_set_geometry (okbutton, BUTTON_W, BUTTON_H);
/*    widget_set_geometry (quitbutton, BUTTON_W, BUTTON_H); */
      widget_configure (okbutton, resource_text, "ok");
      widget_configure (okbutton, resource_textcolor, yellow);
      widget_configure (okbutton, resource_textfont, TIMESI);
      widget_add_callback (okbutton,
                           resource_buttonpress_callback,
                           (void *) keywait_callback, "");
/*
   widget_configure (quitbutton, resource_text, "quit");
   widget_configure (quitbutton, resource_textcolor, yellow);
   widget_configure (quitbutton, resource_textfont, TIMESI);
   widget_add_callback (quitbutton,
   resource_buttonpress_callback,
   (void *) quit_callback, ""); */
    }

  pmframes[0] = frame1;
  pmframes[1] = frame2left;
  pmframes[2] = frame2right;
  pmframes[3] = frame3;
  pmframes[4] = frame4;
  pmframes[5] = okbutton;

  return frame2center;
}

void quick_log_button_callback callbackarg_param;

void
point_init ()
{
  widget_t framepoint, frames[6], button;
  widget_t point_cell[30];
  integer_t i, x, y;
  integer_t n_point_idx_count;  /* calculation maximam cell */
  extern integer_t yellow;

  memset (point_cell, 0, sizeof (point_cell));

  read_result_name ();

  framepoint =
    create_basic_info_frame ("frame", point, frames, PAI_W * 18, 0, 1);
  widget_configure (frames[4], resource_frame_start, 270);
  pointokbutton = frames[5];

  quicklogbutton = button = widget_create ("button", frames[4]);
  widget_set_geometry (button, BUTTON_W, BUTTON_H);
  widget_configure (button, resource_text, "quicklog");
  widget_configure (button, resource_textcolor, yellow);
  widget_configure (button, resource_textfont, TIMESI);
  widget_add_callback (button,
                       resource_buttonpress_callback,
                       (void *) quick_log_button_callback, "");
  if (!PAICARD)
    {
      point_header = widget_create ("pmcontainer", framepoint);
      widget_set_geometry (point_header, 100, YAKUHEAD_H);
      pmcontainer_print (point_header, pm_point_header);
    }

  widget_get_geometry (frames[1], &x, &y);

  n_point_idx_count = ((y - ((HEAD_H + 4) * 2) - (PAI_H + 4) * 3) / CELL_H);
  for (i = 0; i < 24; i++)
    {
      point_cell[i] = widget_create ("frame", framepoint);
      widget_configure (point_cell[i], resource_packed, resource_horizontal);

      if (i == 0)
        {
          widget_configure (point_cell[i],
                            resource_packed, resource_virtical);
          point_results = widget_create ("pmcontainer", point_cell[i]);
          widget_set_geometry (point_results, xwidth, HEAD_H + 4);
          hora_init (point_cell[i], xwidth - 100);
        }
      else if (1 <= i && i <= 4)
        {
          widget_set_geometry (point_cell[i], xwidth, CELLHOME_H);
          point_home[i - 1] = widget_create ("pmcontainer", point_cell[i]);
          point_home_user[i - 1] = widget_create ("text", point_cell[i]);
          point_home_point[i - 1] = widget_create ("text", point_cell[i]);
          widget_set_geometry (point_home[i - 1], 100, CELLHOME_H);
          widget_set_geometry (point_home_user[i - 1], 100, CELLHOME_H);
          widget_set_geometry (point_home_point[i - 1], 150, CELLHOME_H);

          widget_configure (point_home_user[i - 1], resource_textfont,
                            MEDIUMJP);
          widget_configure (point_home_point[i - 1], resource_textfont,
                            TIMESIB);
        }
      else if (i == 5)
        {
          /* spaceing */
          widget_t graph;
          widget_set_geometry (point_cell[i], xwidth, CELL_H);
          graph = widget_create ("pmcontainer", point_cell[i]);
          widget_set_geometry (graph, xwidth, CELL_H);
        }
      else if (i == 6)
        {
          point_fubase = point_cell[i];
          point_fu = widget_create ("text", point_cell[i]);
          point_fustr = widget_create ("pmcontainer", point_cell[i]);
          point_fan = widget_create ("text", point_cell[i]);
          point_fanstr = widget_create ("pmcontainer", point_cell[i]);
          widget_set_geometry (point_fu, 48, HEAD_H);
          widget_set_geometry (point_fustr, 48, HEAD_H);
          widget_set_geometry (point_fan, 48, HEAD_H);
          widget_set_geometry (point_fanstr, 48, HEAD_H);
          widget_configure (point_fu, resource_textfont, LUCIDA);
          widget_configure (point_fan, resource_textfont, LUCIDA);
        }
#define YB 7
#define YL 5
      else if (0 <= (i - YB) && (i - YB) < YL)
        {
          widget_t *wk;
          point_yaku[i - YB] = widget_create ("text", point_cell[i]);
          point_yaku_point[i - YB] = widget_create ("text", point_cell[i]);
          point_yaku[i - YB + YL] = widget_create ("text", point_cell[i]);
          point_yaku_point[i - YB + YL] =
            widget_create ("text", point_cell[i]);


          widget_configure (point_yaku[i - YB], resource_textfont, MEDIUMJP);
          widget_configure (point_yaku[i - YB + YL], resource_textfont,
                            MEDIUMJP);
          widget_configure (point_yaku_point[i - YB], resource_textfont,
                            TIMESIB);
          widget_configure (point_yaku_point[i - YB + YL], resource_textfont,
                            TIMESIB);
          widget_set_geometry (point_yaku[i - YB], 200, CELL_H);
          widget_set_geometry (point_yaku[i - YB + YL], 200, CELL_H);
          widget_set_geometry (point_yaku_point[i - YB], 100, CELL_H);
          widget_set_geometry (point_yaku_point[i - YB + YL], 100, CELL_H);
        }
    }                           /* for */

  pmcontainer_print (point_fustr, pm_point_fu_sfx);
  pmcontainer_print (point_fanstr, pm_point_fan_sfx);

  widget_pack (point);
  widget_fix (point);

  widget_set_geometry (point, xwidth, xheight);
}

static void
point_clear ()
{
  integer_t i;
  pmcontainer_clear (point_results);
  for (i = 0; i < 4; i++)
    {
      pmcontainer_clear (point_home[i]);
    }
  for (i = 0; i < 30; i++)
    {
      if (!point_yaku[i])
        {
          continue;
        }
      widget_configure (point_yaku[i], resource_text, "");
    }

}

static int
point_color (integer_t point)
{
  int color;
  if (point > 0)
    color = white;
  else if (point == 0)
    color = yellow;
  else
    color = darkorange;
  return color;
}

static int
point_bgcolor (integer_t point)
{
  int color;
  extern int blue, red;
  if (point > 0)
    color = blue;
  else if (point == 0)
    color = get_basiccolorpixel ();
  else
    color = red;
  return color;
}

void
ui_result (global_t * gp)
{
  int p, i;
  int iy[4];
  char buf[1024];
  int n = 0;

  if (flag_pause && !auto_play && !BUILD4)
    {
      wait4key ();
    }
  if (!point)
    make_point ();
  if (BUILD4)
    wrap_widgets ();

  snd (audioplay_yaku ());
  flag_not_needredraw = 0;
  point_clear ();

  memcpy (hora_global, gp, sizeof (global_t));
  hora_reset ();

  for (p = 0; p < 4; p++)
    {
      if (result[p].flag == 0)
        {
          continue;
        }
      n++;
    }
  for (p = 0; p < 4; p++)
    {
      if (result[p].flag == 0)
        {
          continue;
        }

      if (result[p].flag != RESULT_RYUKYOKU && (result[p].who != -1))
        {
          hora_set (hora_global, result[p].who);
        }
      else
        {
          hora_set (hora_global, -1);
        }

      result_cvt_to_int (result + p, iy);
      pmcontainer_print (point_results, pm_result_name[result[p].flag]);

      for (i = 0; i < 4; i++)
        {
          integer_t color;
          pmcontainer_print (point_home[i], pm_home_names[i]);
          print_yakupoint (point_home_point[i], result[p].points[i]);

          widget_configure (point_home_user[i], resource_text,
                            player[pplayer[i]].name);
          color = point_color (result[p].points[i]);
          widget_configure (point_home_user[i], resource_textfont, MEDIUMJP);
          widget_configure (point_home_user[i], resource_textcolor, color);
          {
            extern integer_t *tblpos;
            color = point_bgcolor (result[p].points[i]);
            widget_model_change (name_frame[tblpos[i]], color, white, white,
                                 white);
          }
        }

      if (result[p].fu)
        {
          widget_configure (point_fubase, resource_invisible, 0);
          print_yakupoint (point_fu, result[p].fu);
          print_yakupoint (point_fan, result[p].fan);
        }
      else
        {
          widget_configure (point_fubase, resource_invisible, 1);
        }

#define yaku_idx(idx)  		\
		if (!point_yaku[idx]) { goto YAKUEND; }

      {
        integer_t idx = 0;
        for (i = 0; i < Y_MAX; i++)
          {
            if (Y_GET (iy, i))
              {
                char buf[128];
                int len, n;

                yaku_idx (idx);

                strcpy (buf, result_str[i]);
                len = strlen (buf);

                if (buf[len - 1] == ')' && buf[len - 3] == '(')
                  {
                    buf[len - 3] = buf[len - 1] = 0;
                    n = buf[len - 2] - '0';
                    print_yakupoint (point_yaku_point[idx], n);
                  }
                else
                  {
                    widget_configure (point_yaku_point[idx], resource_text,
                                      "");
                  }
                widget_configure (point_yaku[idx], resource_text, buf);
                idx++;
              }
          }

        if (result[p].dora)
          {
            yaku_idx (idx);
            widget_configure (point_yaku[idx], resource_text, dora_str);
            print_yakupoint (point_yaku_point[idx], result[p].dora);
            idx++;
          }
        if (result[p].uradora)
          {
            yaku_idx (idx);
            widget_configure (point_yaku[idx], resource_text, uradora_str);
            print_yakupoint (point_yaku_point[idx], result[p].uradora);
            idx++;
          }
        if (result[p].akadora)
          {
            yaku_idx (idx);
            widget_configure (point_yaku[idx], resource_text, akadora_str);
            print_yakupoint (point_yaku_point[idx], result[p].akadora);
            idx++;
          }
        for (; idx < 30; idx++)
          {
            if (!point_yaku[idx])
              {
                continue;
              }
            widget_configure (point_yaku[idx], resource_text, "");
            widget_configure (point_yaku_point[idx], resource_text, "");
          }
      }
    YAKUEND:
      draw_hora ();
      {
        extern widget_t board;
        if (!BUILD4)
          {
            widget_unmap (board);
          }
        widget_map (point);
        widget_display (point);
        widget_flush (point);
        {
          for (i = 0; i < 4; i++)
            {
              widget_display (name_frame[i]);
              widget_flush (name_frame[i]);
            }
        }

        if (!auto_play)
          wait_key ();

        hora_reset ();
      }
    }

  linepos_clear ();
  for (i = 0; i < 4; i++)
    {
      widget_model_change (name_frame[i], get_basiccolorpixel (), white,
                           white, white);
    }
  if (!BUILD4)
    {
      widget_unmap (point);
    }
  else
    {
      unwrap_widgets ();
    }
  board_reset ();
  board_redraw (1);

  if (logfile)
    log_play (gp, logfile);
}

















/*
   gameend window
 */

widget_t gameend;
static widget_t gameend_header;
static widget_t gameend_pos;
static widget_t gameend_pai;
static widget_t gameend_name[4];
static widget_t gameend_tpoint1[4];
static widget_t gameend_tpoint2[4];
static widget_t gameend_rpoint[4];
static widget_t gameend_xpoint[4];

void
gameend_init ()
{
  widget_t framepoint, pmframes[6];
  widget_t point_cell[4];
  integer_t i;
  framepoint =
    create_basic_info_frame ("frame", gameend, pmframes, 400, 0, 1);
  widget_configure (pmframes[4], resource_frame_start, 270);
  gameendokbutton = pmframes[5];

  gameend_header = widget_create ("pmcontainer", framepoint);
  widget_set_geometry (gameend_header, xwidth - (LEFTMARGIN * 2), YAKUHEAD_H);

  gameend_pos = widget_create ("pmcontainer", framepoint);
  widget_set_geometry (gameend_pos, xwidth - (LEFTMARGIN * 2), 73);

  for (i = 0; i < 4; i++)
    {
      widget_t w;

      point_cell[i] = widget_create ("frame", framepoint);
      widget_configure (point_cell[i], resource_packed, resource_horizontal);
      widget_set_geometry (point_cell[i], xwidth, RESULTHOME_H);

      gameend_name[i] = widget_create ("text", point_cell[i]);
      widget_set_geometry (gameend_name[i], 160, RESULTHOME_H);
      widget_configure (gameend_name[i], resource_textfont, MEDIUMJP);
      widget_configure (gameend_name[i], resource_textcolor, white);

      w = widget_create ("frame", point_cell[i]);
      widget_configure (w, resource_packed, resource_virtical);

      gameend_tpoint1[i] = widget_create ("pmcontainer", w);
      gameend_tpoint2[i] = widget_create ("pmcontainer", w);
      widget_set_geometry (gameend_tpoint1[i], 110, 14);
      widget_set_geometry (gameend_tpoint2[i], 110, 14);

      gameend_rpoint[i] = widget_create ("text", point_cell[i]);
      widget_set_geometry (gameend_rpoint[i], 110, RESULTHOME_H);
      widget_configure (gameend_rpoint[i], resource_textfont, TIMESIB);
      widget_configure (gameend_rpoint[i], resource_textcolor, white);

      gameend_xpoint[i] = widget_create ("text", point_cell[i]);
      widget_set_geometry (gameend_xpoint[i], 110, RESULTHOME_H);
      widget_configure (gameend_xpoint[i], resource_textfont, TIMESIB);
      widget_configure (gameend_xpoint[i], resource_textcolor, white);
    }

  pmcontainer_print (gameend_header, pm_gameend_header);
  widget_pack (gameend);
  widget_fix (gameend);

  widget_set_geometry (gameend, xwidth, xheight);
}



void
gameend_clear ()
{
  integer_t i;
  for (i = 0; i < 4; i++)
    {
      widget_configure (gameend_name[i], resource_text, "");
      pmcontainer_clear (gameend_pos);
      pmcontainer_clear (gameend_tpoint1[i]);
      pmcontainer_clear (gameend_tpoint2[i]);
      pmcontainer_clear (gameend_rpoint[i]);
      pmcontainer_clear (gameend_xpoint[i]);
    }
}

static void
print_pos (canvas, val)
     widget_t canvas;
     integer_t val;
{
  if (val <= 0 || val > 4)
    {
      return;                   /* unsupport 10 ponba or 10 kyoku */
    }
  else
    {
      _print_colorpixmap (canvas, pm_posdigit[val]);
    }
}

static void
print_pospai (canvas, val)
     widget_t canvas;
     integer_t val;
{
  gpixmap2_t pm;
  static char kindtable[] = { 19, 1, 10, 19, 10, 1, 19, 10, 10, 1 };
  unsigned int kind = (((unsigned int) randval ()) % sizeof (kindtable));

  /*assert (val >= 1 && val <= 4); */
  pm = pai2pix ((4 * (kindtable[kind % sizeof (kindtable)] + (val - 1))),
                3, 0, 1);
  print_colorpixmap2_withmargin (pm, 10);
  print_colorpixmap2 (pm);
  print_colorpixmap2 (pm);
}



ui_message_gend (gp)
     global_t *gp;
{
  int i, j, n;
  integer_t xpoint[4];
  static int kind = 0;
  widget_t canvas;

  lang_set_auto ();
  if (auto_button)
    {
      widget_display (auto_button);
      widget_flush (auto_button);
    }

  if (!gameend)
    make_point ();
  snd (audioplay_gameend ());

  for (i = 0; i < 4; i++)
    {
      widget_t canvas;
      int nn, m, x;

      widget_configure (gameend_name[i], resource_text, player[i].name);
      print_yakupoint (gameend_rpoint[i], player[i].rpoint);
      n = nn = player[i].tpoint;
      if (n < 0)
        n = -n;
      if (n > 95)
        n = 95;
      m = 0;
      while (n + m > 20)
        {
          n -= 5;
          m++;
        }
      canvas = gameend_tpoint2[i];
      canvas_clear ();
      canvas = gameend_tpoint1[i];
      canvas_clear ();
      for (j = 0; j < n + m; j++)
        {
          if (j == 10)
            {
              canvas = gameend_tpoint2[i];
            }
          if (j < m && nn > 0)
            {
              print_colorpixmap (pm_chipblue5);
            }
          else if (j < m && nn < 0)
            {
              print_colorpixmap (pm_chipred5);
            }
          else if (nn > 0)
            {
              print_colorpixmap (pm_chipblue);
            }
          else
            {
              print_colorpixmap (pm_chipred);
            }
        }

      xpoint[i] = player[i].rpoint * 100 + player[i].tpoint * 500;
      print_yakupoint (gameend_xpoint[i], xpoint[i]);
      widget_configure (gameend_name[i], resource_textcolor,
                        point_color (xpoint[i]));
      widget_configure (gameend_xpoint[i], resource_textcolor,
                        point_color (xpoint[i]));
    }
  {
    int i, pos = 1;
    for (i = 0; i < 4; i++)
      {
        if (player[i].rpoint > player[self].rpoint)
          {
            pos++;
          }
      }
    canvas = gameend_pos;
    canvas_clear ();
/*    print_pos (gameend_pos, pos); */
    print_pospai (gameend_pos, pos);

    switch (pos)
      {
      case 1:
        snd (audioplay_1st ());
        break;
      case 2:
        snd (audioplay_2nd ());
        break;
      case 3:
        snd (audioplay_3rd ());
        break;
      case 4:
        snd (audioplay_4th ());
        break;
      }

  }


  {
    extern widget_t board;
    widget_unmap (board);
    widget_unmap (point);

    widget_map (gameend);
    widget_display (gameend);
    widget_flush (gameend);


    if (!auto_play)
      wait_key ();


    widget_unmap (gameend);

  }

  board_reset ();
  board_redraw (1);
  if (logfile)
    log_game (gp, logfile);
}

void
make_point ()
{
  extern widget_t root_widget;
  extern integer_t xwidth, xheight;

  point = widget_create ("frame", root_widget);
  gameend = widget_create ("frame", root_widget);
  widget_configure (point, resource_name, "point");
  widget_configure (gameend, resource_name, "gameend");
  widget_set_geometry (point, xwidth, xheight);
  widget_set_geometry (gameend, xwidth, xheight);

  widget_add_callback (point,
                       resource_keypress_callback,
                       (void *) keywait_callback, "");
  widget_add_callback (gameend,
                       resource_keypress_callback,
                       (void *) keywait_callback, "");
  widget_add_callback (point,
                       resource_buttonpress_callback,
                       (void *) keywait_callback, "");
  widget_add_callback (gameend,
                       resource_buttonpress_callback,
                       (void *) keywait_callback, "");

  point_init ();
  gameend_init ();
  lang_set ();
}


wrap_widgets ()
{
  int i;
  extern widget_t sub_board;
  extern int in_result;
  extern integer_t *tblpos;
  integer_t w, h;

  widget_t ww1, button;
  extern global_t *game_global;
  global_t *gp = game_global;

  in_result = 1;
  widget_unmap (about_button);
  widget_unmap (flush_button);
  widget_unmap (visible_button);

  if (!wrap_point)
    {
      wrap_point = widget_create ("frame", sub_board);
      widget_get_geometry (sub_board, &w, &h);
      widget_set_location (wrap_point, 8, 8);
      widget_set_geometry (wrap_point, w - 8, h - 8);
      wrap_results = widget_create ("pmcontainer", wrap_point);
      widget_set_geometry (wrap_results, name_width * 2, HEAD_H + 4);
      wrap_fubase = ww1 = widget_create ("frame", wrap_point);
      widget_configure (ww1, resource_packed, resource_horizontal);
      wrap_fu = widget_create ("text", ww1);
      wrap_fustr = widget_create ("pmcontainer", ww1);
      wrap_fan = widget_create ("text", ww1);
      wrap_fanstr = widget_create ("pmcontainer", ww1);
      widget_set_geometry (wrap_fu, 48, HEAD_H);
      widget_set_geometry (wrap_fustr, 48, HEAD_H);
      widget_set_geometry (wrap_fan, 48, HEAD_H);
      widget_set_geometry (wrap_fanstr, 48, HEAD_H);
      widget_configure (wrap_fu, resource_textfont, LUCIDA);
      widget_configure (wrap_fan, resource_textfont, LUCIDA);
      pmcontainer_print (wrap_fustr, pm_point_fu_sfx);
      pmcontainer_print (wrap_fanstr, pm_point_fan_sfx);
      for (i = 0; i < 20; i++)
        {
          ww1 = widget_create ("frame", wrap_point);
          widget_configure (ww1, resource_packed, resource_horizontal);
          wrap_yaku[i] = widget_create ("text", ww1);
          widget_configure (wrap_yaku[i], resource_text, "");
          wrap_yaku_point[i] = widget_create ("text", ww1);
          widget_set_geometry (wrap_yaku[i], name_width * 2 - 70, CELL_H);
          widget_set_geometry (wrap_yaku_point[i], 70, CELL_H);
          widget_configure (wrap_yaku[i], resource_textfont, MEDIUMJP);
          widget_configure (wrap_yaku_point[i], resource_textfont, TIMESIB);
        }

      widget_pack (wrap_point);
      /*  widget_fix (wrap_point); */
    }

  save_point = point;
  point = wrap_point;

  for (i = 0; i < 4; i++)
    {
      save_home_point[i] = point_home_point[i];
      save_home_user[i] = point_home_user[i];
    }
  for (i = 0; i < 4; i++)
    {
      point_home_point[i] = user_ppoint[tblpos[i]];
      point_home_user[i] = user_name[tblpos[i]];
    }
  for (i = 0; i < 30; i++)
    {
      save_yaku[i] = point_yaku[i];
      point_yaku[i] = wrap_yaku[i];
      save_yaku_point[i] = point_yaku_point[i];
      point_yaku_point[i] = wrap_yaku_point[i];
    }
  save_fubase = point_fubase;
  point_fubase = wrap_fubase;
  save_fu = point_fu;
  point_fu = wrap_fu;
  save_fustr = point_fustr;
  point_fustr = wrap_fustr;
  save_fan = point_fan;
  point_fan = wrap_fan;
  save_fanstr = point_fanstr;
  point_fanstr = wrap_fanstr;
  save_results = point_results;
  point_results = wrap_results;

  lang_set_cancel (1);
  lang_set_advice (1);

  widget_map (cancel_button);
  widget_map (advice_button);
  widget_display (cancel_button);
  widget_display (advice_button);
  widget_flush (cancel_button);
  widget_flush (advice_button);
  widget_configure (wrap_point, resource_invisible, 0);
}

unwrap_widgets ()
{
  extern int in_result;
  int i;

  lang_set_cancel (0);
  lang_set_advice (0);
  widget_unmap (cancel_button);
  widget_unmap (advice_button);
  actionbutton_setup ();

  for (i = 0; i < 4; i++)
    {
      point_home_point[i] = save_home_point[i];
      point_home_user[i] = save_home_user[i];
      widget_configure (user_name[i], resource_textcolor, yellow);
    }
  for (i = 0; i < 30; i++)
    {
      point_yaku[i] = save_yaku[i];
      point_yaku_point[i] = save_yaku_point[i];
    }
  point_fubase = save_fubase;
  point_fu = save_fu;
  point_fustr = save_fustr;
  point_fan = save_fan;
  point_fanstr = save_fanstr;
  point_results = save_results;
  point = save_point;

  widget_unmap (wrap_point);
  widget_configure (wrap_point, resource_invisible, 1);
  in_result = 0;
}
