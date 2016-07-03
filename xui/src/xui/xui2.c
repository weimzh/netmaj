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
 * xui2.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Modifier        : suz@kt.rim.or.jp
 * Created On      : Tue Apr 11 11:13:47 1995
 *   mainly user interface parts 
 *   called ui_??? from client main loop
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 *  Copyright (C) 1996 Koji Suzuki All rights reserved
 */

#include "xnet.h"
#include "xnetmaj.h"
#include "pixread.h"
#include "pmutil.h"

extern int choiced;

#define D(a)

extern widget_t root_widget;

static struct
{
  integer_t _tblpos[4];
}

maptblpos[4] =
{
  {3, 0, 1, 2},
  {2, 3, 0, 1},
  {1, 2, 3, 0},
  {0, 1, 2, 3}
};

integer_t *tblpos;
global_t *game_global;
int hand_startpos;
int prog_plview = 0;

integer_t
mytblpos (integer_t i)
{
  return tblpos[i];
}

integer_t
pos2who (integer_t i)
{
  int who;
  global_t *gp = game_global;
  for (who = 0; who < 4; who++)
    {
      if (pplayer[who] == i)
        {
          return who;
        }
    }
  return who;
}

#ifdef ASCII_MESSAGE
char talk_messages[15][60] =
  { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15" };
#else
char talk_messages[15][60] = {
  "よろしくおねがいします",
  "はやくしてね",
  "ちょっとまってね",
  "やった！！",
  "しまった",
  "困った",
  "まいった",
  "地獄に落ちろ!",
  "ギャー",
  "うるさいっ！",
  "ほっといてくれ！",
  "へっへっへっ。",
  "もうすぐぶっ飛び〜♪",
  "ラッキーしやがって",
  "んなの鳴くな",
};

#endif

static void
load_talk_str (global_t *gp)
{
  pf_param_str (gp, "talk_1", talk_messages[0]);
  pf_param_str (gp, "talk_2", talk_messages[1]);
  pf_param_str (gp, "talk_3", talk_messages[2]);
  pf_param_str (gp, "talk_4", talk_messages[3]);
  pf_param_str (gp, "talk_5", talk_messages[4]);
  pf_param_str (gp, "talk_6", talk_messages[5]);
  pf_param_str (gp, "talk_7", talk_messages[6]);
  pf_param_str (gp, "talk_8", talk_messages[7]);
  pf_param_str (gp, "talk_9", talk_messages[8]);
  pf_param_str (gp, "talk_10", talk_messages[9]);
  pf_param_str (gp, "talk_11", talk_messages[10]);
  pf_param_str (gp, "talk_12", talk_messages[11]);
  pf_param_str (gp, "talk_13", talk_messages[12]);
  pf_param_str (gp, "talk_14", talk_messages[13]);
  pf_param_str (gp, "talk_15", talk_messages[14]);
}

void
ui_init (global_t *gp, int kind, char *type)
{
  extern widget_t board;

  load_talk_str (gp);

  game_global = gp;

  switch (kind)
    {
    case PROG_NETMAJ:
      break;
    case PROG_NETMAJ1:
      break;
    case PROG_PLVIEW:;
      prog_plview = 1;
      break;
    }
  board_setup (kind, type);

  load_xrcfile ();
  setup ();
  make_board ();
/*  make_point (); */
/*  make_about (); */

  read_pixmaps ();
  read_attr_pixmaps ();
  read_decorate_pixmaps ();

  widget_dev_map (board);       /* display window here !! */
  widget_sync (board);
  openning ();
  lang_set ();
/*  board_reset (); */
  board_redraw (1);

}

void
ui_term ()
{
}


#define print_digit(p)      _print_digit(canvas, p)
#define print_kanjidigit(p) _print_kanjidigit(canvas, p)

static widget_t canvas;

void
_print_digit (widget_t w, integer_t val)
{
  char buffer[64], *p = buffer;
  sprintf (buffer, "%d", val);

  if (widget_text_p (w))
    {
      char *p = strdup (buffer);
      if (!p)
        {
          fatal ("");
        }
      widget_configure (w, resource_text, p);
    }
  else
    {
      fprintf (stderr, "error print digit");
    }
}

static void
_print_kanjidigit (widget_t w, integer_t val)
{
  if (val < 0 || val > 10)
    {
      return;                   /* unsupport 10 ponba or 10 kyoku */
    }
  else
    {
      _print_colorpixmap (w, pm_digit[val]);
    }
}

void
print_pmdigit (widget_t w, integer_t val)
{
  if (val < 0 || val > 10)
    {
      return;                   /* unsupport 10 ponba or 10 kyoku */
    }
  else
    {
      _print_colorpixmap (w, pm_digit[val]);
    }
}

#define print_n_hon(val)  _print_n_hon(canvas, val)

static void
_print_n_hon (widget_t w, integer_t val)
{
  if (val >= 100)
    {
      return;
    }
  if (val < 0 || (val >= 10 && val < 100))
    {
      pmcontainer_set_adigit (w, val, 0);
      return;
    }
  if (val > 5 || val == 0)
    {
      _print_colorpixmap (w, pm_digit[val]);
    }
  else
    {
      _print_colorpixmap (w, pm_n_hon[val]);
    }
}

static void
draw_status (global_t *gp)
{
  int pai, i;
  D (fprintf (stderr, "draw status"));

  if (!ch_dora && !ch_play)
    {
      return;
    }
  ch_dora = 0;

  canvas = state_frame;
  canvas_clear ();
  print_colorpixmap (pm_bturn_names[big_turn]);
  print_kanjidigit (small_turn + 1);
  print_colorpixmap (pm_sturn_sfx);
  if (small_turn_cnt > 0)
    {
      print_n_hon (small_turn_cnt);
      print_colorpixmap (pm_sturn_cnt_sfx);
    }
  canvas = reststr_frame;
  canvas_clear ();
  print_colorpixmap (pm_rest_str);
  canvas = rest_frame;
  print_digit (rest_point);

  canvas = mt_reststr_frame;
  canvas_clear ();
  print_colorpixmap (pm_mt_rest_str);
  canvas = mt_rest_frame;
  print_digit (mt_rest (gp));

  canvas = dorastr_frame;
  canvas_clear ();
  print_colorpixmap (pm_dora_str);

  canvas = dora_frame;
  canvas_clear ();
  for (i = 0; i < 5; i++)
    {
      print_colorpixmap2 (pai2pix (mt_dora (gp, i), 3, 0, PAIOPEN));
    }

  canvas = uradorastr_frame;
  canvas_clear ();
  print_colorpixmap (pm_uradora_str);

  canvas = uradora_frame;
  canvas_clear ();
  for (i = 0; i < 5; i++)
    {
      pai = mt_uradora (gp, i);
      if (PAICARD3D && BUILD4 && !pai)
        pai = 0x100;
      print_colorpixmap2 (pai2pix (pai, 3, 0, PAIOPEN));
    }

  widget_display (status_frame);
  widget_flush (status_frame);
}

#define print_str(s)  _print_str(canvas, s)

void
_print_str (widget_t w, char *s)
{
  widget_configure (w, resource_text, s);
}

widget_t last_linepos;
widget_t last_mark;

static void
draw_player (global_t *gp, integer_t who)
{
  int i, n, m, x, nn;
  int pp;

  if (!ch_play && !ch_point)
    return;

  pp = pplayer[who];

  canvas = user_kaze[tblpos[who]];
  canvas_clear ();
  print_colorpixmap (pm_bturn_names[who]);
  print_colorpixmap (pm_ie);

  canvas = user_state[tblpos[who]];
  canvas_clear ();
  if ((who + small_turn) % 4 == 0)
    {
      print_colorpixmap (pm_user_state);
    }
  canvas = user_name[tblpos[who]];
  print_str (player[pp].name);

  canvas = user_gpoint[tblpos[who]];
  print_digit (player[pp].gpoint);
  canvas = user_ppoint[tblpos[who]];
  print_digit (player[pp].ppoint);

  canvas = user_chip2[tblpos[who]];
  if (canvas)
    canvas_clear ();
  canvas = user_chip[tblpos[who]];
  canvas_clear ();

  n = nn = player[pp].tpoint;
  if (user_chip2[tblpos[who]])
    x = 20;
  else
    x = 10;

  if (n > 95 && x == 20)
    n = 95;
  if (n > 45 && x == 10)
    n = 45;

  m = 0;
  if (n < 0)
    n = -n;

  while ((n + m) > x)
    {
      n -= 5;
      m += 1;
    }

  for (i = 0; i < (n + m) && i < 20; i++)
    {
      if (i == 10)
        canvas = user_chip2[tblpos[who]];
      if (nn > 0 && i < m)
        {
          print_colorpixmap (pm_chipblue5);
        }
      else if (nn > 0)
        {
          print_colorpixmap (pm_chipblue);
        }
      else if (i < m)
        {
          print_colorpixmap (pm_chipred5);
        }
      else
        {
          print_colorpixmap (pm_chipred);
        }
    }

  widget_display (name_frame[tblpos[who]]);
  widget_flush (name_frame[tblpos[who]]);
}

static void
draw_river1 (global_t *gp, integer_t who)
{
  int max;
  int pp;
  int i, ii;
  int pai, reachpos = -1;
  char paiviewattr[RIVER_MAX];
  char riveridx[RIVER_MAX];
  integer_t current = 0;

  if (!ch_river[who] && !ch_play)
      return;

  ch_river[who] = 0;

  /* count up river tiles */
  for (max = 0; max < RIVER_MAX; max++)
    {
      if (!rv[who][max].in && !rv[who][max].out)
        {
          break;
        }
    }
  pp = pplayer[who];

  memset (paiviewattr, PAIOPEN, max);   /* opened  */

  /* pon, reach etc */
  for (i = 0; i < max; i++)
    {
      int t, r;
      t = T_GET (rv[who][i].attr);
      r = R_GET (rv[who][i].attr);
      if (t == T_HN_KAN || t == T_HN_KAN_C || t == T_HN_TSM ||
          r == R_TIE || r == R_PON || r == R_KAN || r == R_RON)
        {
          /* not normal pai (dark) */
          paiviewattr[i] = PAIOPEN | PAIDARK;
        }
    }
  /* check removed pai */
  if (flag_invisible_riverpai)
    {
      integer_t idx = 0;
      for (i = 0; i < max; i++)
        {
          if (paiviewattr[i] == (PAIOPEN | PAIDARK)
              && R_GET (rv[who][i].attr) != R_RON
              && !(T_GET (rv[who][i].attr) == T_RV_RCH))
            {
              riveridx[i] = -1;
            }
          else
            {
              riveridx[i] = idx++;
            }
        }
    }
  else
    {
      for (i = 0; i < max; i++)
        {
          riveridx[i] = i;
        }
    }

  canvas = river_frame[tblpos[who]];
  canvas_clear ();

  {
    extern int rv_fixed;
    for (i = 0; i < max; i++)
      {
        int reach_p;
        gpixmap2_t paipix;

        ii = riveridx[i];
        pai = rv[who][i].out;
        if (rvp == &(rv[who][i]))
          {
            current = 1;
          }
        reach_p = (T_GET (rv[who][i].attr) == T_RV_RCH);
        paipix = pai2pix (pai, tblpos[who], reach_p, paiviewattr[i]);

        if (ii == -1)
          {
            continue;
          }
        if (!rv_fixed && rvp == &(rv[who][i]))
          {
            print_colorpixmap2_withmargin (paipix, HANDPAIMARGIN);
          }
        else
          {
            print_colorpixmap2 (paipix);
          }
      }
  }
  canvas = river_attr1[tblpos[who]];
  canvas_clear ();
  {
    extern int rv_fixed;
    for (i = 0; i < max; i++)
      {
        int reach_p;
        gpixmap2_t attr2pix2 (), paipix;

        ii = riveridx[i];
        pai = (rv[who][i].in == rv[who][i].out);
        if (rvp == &(rv[who][i]))
          {
            current = 1;
          }
        reach_p = (T_GET (rv[who][i].attr) == T_RV_RCH);
        paipix = attr2pix2 (pai, tblpos[who], reach_p);

        if (ii == -1)
          {
            continue;
          }
        if (!rv_fixed && rvp == &(rv[who][i]))
          {
            print_colorpixmap2_withmargin (paipix, HANDPAIMARGIN);
          }
        else
          {
            print_colorpixmap2 (paipix);
          }
      }
  }

  canvas = mark_frame[tblpos[who]];
  canvas_clear ();
  if (flag_posmark && current)
    {
      last_mark = canvas;
      print_colorpixmap (pm_current);
    }
  widget_display (canvas);
  widget_flush (canvas);
  canvas = linepos_frame[tblpos[who]];
  if (flag_linepos && canvas)
    {
      extern integer_t white, black, gray;
      if (current)
        {
          last_linepos = canvas;
          widget_model_change (canvas, gray, gray, gray, gray);
          widget_configure (canvas, resource_cannot_change_model, 1);
        }
      else
        {
          widget_configure (canvas, resource_cannot_change_model, 0);
          widget_model_change (canvas, get_basiccolorpixel (), white, white,
                               white);
        }
      widget_display (canvas);
      widget_flush (canvas);
    }
  widget_display (river_frame[tblpos[who]]);
  widget_display (river_attr1[tblpos[who]]);
  widget_display (river_attr2[tblpos[who]]);
  widget_display (river_attr3[tblpos[who]]);
  widget_display (river_attr4[tblpos[who]]);
  widget_flush (river_frame[tblpos[who]]);
  widget_flush (river_attr1[tblpos[who]]);
  widget_flush (river_attr2[tblpos[who]]);
  widget_flush (river_attr3[tblpos[who]]);
  widget_flush (river_attr4[tblpos[who]]);
}

static void
draw_river2 (global_t *gp, integer_t who)
{
  int max;
  int pp;
  int i, ii;
  int pai, reachpos = -1;
  char paiviewattr[RIVER_MAX];
  char riveridx[RIVER_MAX];
  integer_t current = 0;

  if (!ch_river[who] && !ch_play)
    {
      return;
    }

  ch_river[who] = 0;

  /* count up river tiles */
  for (max = 0; max < RIVER_MAX; max++)
    {
      if (!rv[who][max].in && !rv[who][max].out)
        {
          break;
        }
    }
  pp = pplayer[who];

  memset (paiviewattr, PAIOPEN, max);   /* opened  */

  /* pon, reach etc */
  for (i = 0; i < max; i++)
    {
      int t, r;
      t = T_GET (rv[who][i].attr);
      r = R_GET (rv[who][i].attr);
      if (t == T_HN_KAN || t == T_HN_KAN_C || t == T_HN_TSM ||
          r == R_TIE || r == R_PON || r == R_KAN || r == R_RON)
        {
          /* not normal pai (dark) */
          paiviewattr[i] = PAIOPEN | PAIDARK;
        }
    }
  /* check removed tile */
  if (flag_invisible_riverpai)
    {
      integer_t idx = 0;
      for (i = 0; i < max; i++)
        {
          if (paiviewattr[i] == (PAIOPEN | PAIDARK)
              && R_GET (rv[who][i].attr) != R_RON
              && !(T_GET (rv[who][i].attr) == T_RV_RCH))
            {
              riveridx[i] = -1;
            }
          else
            {
              riveridx[i] = idx++;
            }
        }
    }
  else
    {
      for (i = 0; i < max; i++)
        {
          riveridx[i] = i;
        }
    }

  {
    extern int rv_fixed;
    for (i = 0; i < max; i++)
      {
        int reach_p;
        gpixmap2_t paipix;

        ii = riveridx[i];
        if (ii >= 0 && (ii % 6 == 0))
          {
            canvas = river_frames[tblpos[who]][ii / 6];
            canvas_clear ();
          }
        pai = rv[who][i].out;
        if (rvp == &(rv[who][i]))
          {
            current = 1;
          }
        reach_p = (T_GET (rv[who][i].attr) == T_RV_RCH);

        if (ii == -1)
          {
            continue;
          }
        if (PAICARD3D && BUILD4 && (tblpos[who] == 0))
          {
            int margin = 0;
            if (ii % 6 == 0 && !rv_fixed && rvp == &(rv[who][i]))
              margin = HANDPAIMARGIN;

            if (ii % 6 == 0 || (!rv_fixed && rvp == &(rv[who][i])))
              {
                paipix =
                  pai2pix (pai | 0x100, tblpos[who], reach_p, paiviewattr[i]);
                print_colorpixmap2_withmargin (paipix, margin);
              }
            paipix = pai2pix (pai, tblpos[who], reach_p, paiviewattr[i]);
            print_colorpixmap2 (paipix);
          }
        else if (PAICARD3D && BUILD4 && (tblpos[who] == 2))
          {
            int margin = -7;
            if (ii % 6 == 0 || !rv_fixed && rvp == &(rv[who][i]))
              margin = 0;
            if (ii % 6 == 0 && !rv_fixed && rvp == &(rv[who][i]))
              margin = HANDPAIMARGIN;
            paipix = pai2pix (pai, tblpos[who], reach_p, paiviewattr[i]);
            print_colorpixmap2_withmargin (paipix, margin);
            paipix =
              pai2pix (pai | 0x100, tblpos[who], reach_p, paiviewattr[i]);
            print_colorpixmap2 (paipix);
          }
        else
          {
            paipix = pai2pix (pai, tblpos[who], reach_p, paiviewattr[i]);
            if (!rv_fixed && rvp == &(rv[who][i]))
              {
                print_colorpixmap2_withmargin (paipix, HANDPAIMARGIN);
              }
            else
              {
                print_colorpixmap2 (paipix);
              }
          }
      }
  }
  {
    extern int rv_fixed;
    for (i = 0; i < max; i++)
      {
        int reach_p;
        gpixmap2_t attr2pix2 (), paipix;

        ii = riveridx[i];
        if (ii >= 0 && (ii % 6 == 0))
          {
            canvas = river_attr1s[tblpos[who]][ii / 6];
            canvas_clear ();
          }
        pai = (rv[who][i].in == rv[who][i].out);
        if (rvp == &(rv[who][i]))
          {
            current = 1;
          }
        reach_p = (T_GET (rv[who][i].attr) == T_RV_RCH);

        if (ii == -1)
          {
            continue;
          }
        if (!rv_fixed && rvp == &(rv[who][i]))
          {
            paipix = attr2pix2 (pai, tblpos[who], reach_p);
            print_colorpixmap2_withmargin (paipix, HANDPAIMARGIN);
          }
        else
          {
            paipix = attr2pix2 (pai, tblpos[who], reach_p);
            print_colorpixmap2 (paipix);
          }
      }
  }

  canvas = mark_frame[tblpos[who]];
  canvas_clear ();
  if (flag_posmark && current)
    {
      last_mark = canvas;
      print_colorpixmap (pm_current);
    }
  widget_display (canvas);
  widget_flush (canvas);
  canvas = linepos_frame[tblpos[who]];
  if (flag_linepos && canvas)
    {
      extern integer_t white, black, gray;
      if (current)
        {
          last_linepos = canvas;
          widget_model_change (canvas, gray, gray, gray, gray);
          widget_configure (canvas, resource_cannot_change_model, 1);
        }
      else
        {
          widget_configure (canvas, resource_cannot_change_model, 0);
          widget_model_change (canvas, get_basiccolorpixel (), white, white,
                               white);
        }
      widget_display (canvas);
      widget_flush (canvas);
    }
  widget_display (river_attr1s[tblpos[who]][0]);
  widget_display (river_attr1s[tblpos[who]][1]);
  widget_display (river_attr1s[tblpos[who]][2]);
  widget_display (river_attr1s[tblpos[who]][3]);
  widget_display (river_frames[tblpos[who]][0]);
  widget_display (river_frames[tblpos[who]][1]);
  widget_display (river_frames[tblpos[who]][2]);
  widget_display (river_frames[tblpos[who]][3]);
  widget_flush (river_attr1s[tblpos[who]][0]);
  widget_flush (river_attr1s[tblpos[who]][1]);
  widget_flush (river_attr1s[tblpos[who]][2]);
  widget_flush (river_attr1s[tblpos[who]][3]);
  widget_flush (river_frames[tblpos[who]][0]);
  widget_flush (river_frames[tblpos[who]][1]);
  widget_flush (river_frames[tblpos[who]][2]);
  widget_flush (river_frames[tblpos[who]][3]);
}

static void
draw_river (global_t *gp, integer_t who)
{
  if (BUILD4)
    draw_river2 (gp, who);
  else
    draw_river1 (gp, who);
}

static void
draw_hand1 (global_t *gp, integer_t who)
{
  int i, j, k, n;
  int pp, pai, naki, n_naki_pai;
  int y;
  int opened = 0;

  if (!ch_hand[who] && !ch_play)
    {
      return;
    }
  ch_hand[who] = 0;

  pp = pplayer[who];

  n = hand[who].closed_num;
  canvas = hand_frame[tblpos[who]];
  canvas_clear ();

  opened = 0;
  if (who == vself)
    {
      opened |= !(mt_uradora (gp, 0) < 4);
      if (n != 14 && n != 11 && n != 8 && n != 5 && n != 2)
        {
          goto normal;
        }
      if (opened)
        {
          widget_configure (canvas, resource_pixmapstart, PAI_W * 3);
        }
      for (i = 0; i < n - 1; i++)
        {
          print_colorpixmap2 (pai2pix
                              (hand[who].closed[i], tblpos[who], 0, opened));
        }
      print_colorpixmap2_withmargin (pai2pix
                                     (hand[who].closed[i], tblpos[who], 0,
                                      opened), HANDPAIMARGIN);
      widget_configure (canvas, resource_pixmapstart, HANDPAISTART);
    }
  else
    {
      opened = !(hand[who].closed[0] < 4);      /* pai is invisible pai */
    normal:
      if (opened)
        {
          widget_configure (canvas, resource_pixmapstart, PAI_W * 3);
          for (i = 0; i < n; i++)
            {
              print_colorpixmap2 (pai2pix
                                  (hand[who].closed[i], tblpos[who], 0,
                                   opened));
            }
        }
      else
        {
          for (i = 0; i < n; i++)
            {
              print_colorpixmap2 (pai2pix
                                  (hand[who].closed[i], tblpos[who], 0,
                                   opened));
            }
        }
      widget_configure (canvas, resource_pixmapstart, HANDPAISTART);
    }

  n = hand[who].closed_num;
  i++;

#ifndef SMALL
  canvas = openhand_frame[tblpos[who]];
  canvas_clear ();
#endif

  naki = 0;
  n_naki_pai = 0;
  for (j = 3; j >= 0; j--)
    {
      if (hand[who].opened_kind[j])
        {
          naki++;
          if (hand[who].opened_kind[j] == H_KAN_CLOSED
              || hand[who].opened_kind[j] == H_KAN_OPENED)
            {
              n_naki_pai += 4;
            }
          else
            {
              n_naki_pai += 3;
            }
        }
    }
  if (naki)
    {
#ifndef SMALL
      pmcontainer_clear (canvas);
      widget_configure (canvas, resource_pixmapstart, (PAI_W * 8) +
                        (3 - naki) * (PAI_W * 4) +
                        (OPENPAIMARGIN * (3 - naki)));
#else
      integer_t w, h;
      integer_t startpos;

      widget_get_geometry (canvas, &w, &h);
      startpos = w - (naki * OPENPAIMARGIN)
        - (naki * (PAI_W2 - PAI_W)) - (PAI_W * n_naki_pai) - 8;
#endif
      for (j = 3; j >= 0; j--)
        {
          if (hand[who].opened_kind[j])
            {
              int hflag = 0;
              pmcontainer_set_margin (canvas, OPENPAIMARGIN);
              for (k = 0; k < 4; k++)
                {
                  hflag = 0;
                  if (k == 3 && (hand[who].opened_kind[j] == H_TIE
                                 || hand[who].opened_kind[j] == H_PON))
                    {
                      break;
                    }
                  pai = hand[who].opened[j][k];
                  if (hand[who].opened_kind[j] == H_TIE && k == 0)
                    {
                      hflag = 1;
                    }
                  if (hand[who].opened_kind[j] == H_PON &&
                      hand[who].opened_from[j] != who &&
                      k == (who - hand[who].opened_from[j] + 3) % 4)
                    {
                      hflag = 1;
                    }
                  if (hand[who].opened_kind[j] == H_KAN_OPENED &&
                      hand[who].opened_from[j] != who &&
                      k == (who - hand[who].opened_from[j] + 3) % 4 + 1)
                    {
                      hflag = 1;
                    }
                  if (hand[who].opened_kind[j] == H_KAN_CLOSED
                      && (k == 1 || k == 2))
                    {
                      pai = P_INV;
                    }
                  {
                    gpixmap2_t paipixmap;
                    paipixmap =
                      pai2pix (pai, tblpos[who], hflag,
                               PAIOPEN /*|PAIDARK */ );
#ifndef SMALL
                    print_colorpixmap2 (paipixmap);
#else
                    if (startpos)
                      {
                        print_colorpixmap2_withofs (paipixmap, startpos);
                        startpos = 0;
                      }
                    else
                      {
                        print_colorpixmap2 (paipixmap);
                      }
#endif
                  }

                  i++;
                }
              pmcontainer_set_margin (canvas, 0);
            }
        }
      widget_display (pai_frame[tblpos[who]]);
      widget_flush (pai_frame[tblpos[who]]);
    }
  else
    {
      widget_display (hand_frame[tblpos[who]]);
      widget_flush (hand_frame[tblpos[who]]);
    }

  if (tblpos[who] == 3)
    {
      /* hand_pai */ ;
    }
}

static void
draw_hand2 (global_t *gp, integer_t who)
{
  int i, j, k, n;
  int naki_width, men_width;
  int pp, pai, naki;
  int y;
  int opened = 0;
  integer_t width, h;
  integer_t startpos;

  if (!ch_hand[who] && !ch_play)
    {
      return;
    }
  ch_hand[who] = 0;

  pp = pplayer[who];

  n = hand[who].closed_num;
  naki_width = 0;
  if (PAICARD3D && BUILD4 && (tblpos[who] == 0 || tblpos[who] == 2))
    {
      men_width = (PAI_W - 1) * n + (HANDPAIMARGIN + 1) * 2;
      for (j = 0; j < 4; j++)
        {
          if (hand[who].opened_kind[j])
            {
              if (hand[who].opened_kind[j] == H_KAN_CLOSED)
                {
                  naki_width += (PAI_W - 1) * 4;
                }
              else if (hand[who].opened_kind[j] == H_KAN_OPENED)
                {
                  naki_width += (PAI_W - 1) * 3 + (PAI_W2 - 1);
                }
              else if (hand[who].opened_kind[j] == H_TIE ||
                       hand[who].opened_kind[j] == H_PON)
                {
                  naki_width += (PAI_W - 1) * 2 + (PAI_W2 - 1);
                }
              naki_width += (HANDPAIMARGIN + 1);
            }
          else
            {
              break;
            }
        }
    }
  else
    {
      men_width = (PAI_W) * n + (HANDPAIMARGIN) * 2;
      for (j = 0; j < 4; j++)
        {
          if (hand[who].opened_kind[j])
            {
              if (hand[who].opened_kind[j] == H_KAN_CLOSED)
                {
                  naki_width += PAI_W * 4;
                }
              else if (hand[who].opened_kind[j] == H_KAN_OPENED)
                {
                  naki_width += PAI_W * 3 + PAI_W2;
                }
              else if (hand[who].opened_kind[j] == H_TIE ||
                       hand[who].opened_kind[j] == H_PON)
                {
                  naki_width += PAI_W * 2 + PAI_W2;
                }
              if (!(BUILD4 && PAICARD) && j != 0)
                {
                  naki_width += HANDPAIMARGIN;
                }
            }
          else
            {
              break;
            }
        }
    }
  naki = j;
  canvas = hand_frame[tblpos[who]];
  canvas_clear ();

  widget_get_geometry (canvas, &width, &h);
  if (!(tblpos[who] & 1))
    width = h;

  if (n != 14 && n != 11 && n != 8 && n != 5 && n != 2)
    {
      startpos = (width - naki_width - men_width - PAI_W) / 2;
    }
  else
    {
      startpos = (width - naki_width - men_width) / 2;
    }
  if (who == vself)
    hand_startpos = startpos;

  widget_configure (canvas, resource_pixmapstart, startpos);
  opened = 0;
  if (who == vself)
    {
      opened |= !(mt_uradora (gp, 0) < 4);
      if (n != 14 && n != 11 && n != 8 && n != 5 && n != 2)
        {
          goto normal;
        }
      for (i = 0; i < n - 1; i++)
        {
          print_colorpixmap2 (pai2pix (hand[who].closed[i],
                                       tblpos[who], 0, opened));
        }

      print_colorpixmap2_withmargin (pai2pix (hand[who].closed[i],
                                              tblpos[who], 0, opened),
                                     HANDPAIMARGIN);
    }
  else
    {
      opened = !(hand[who].closed[0] < 4);      /* pai is invisible pai */
    normal:
      if (opened)
        {
          if (n > 0 && PAICARD3D && BUILD4 && tblpos[who] == 0)
            {
              print_colorpixmap2 (pai2pix (hand[who].closed[0] | 0x100,
                                           tblpos[who], 0, opened));
            }
          for (i = 0; i < n; i++)
            {
              print_colorpixmap2 (pai2pix
                                  (hand[who].closed[i], tblpos[who], 0,
                                   opened));
            }
          if (n > 0 && PAICARD3D && BUILD4 && tblpos[who] == 2)
            {
              print_colorpixmap2 (pai2pix (hand[who].closed[n - 1] | 0x100,
                                           tblpos[who], 0, opened));
            }
        }
      else
        {
          if (n > 0 && PAICARD3D && BUILD4 && tblpos[who] == 0)
            {
              print_colorpixmap2 (pai2pix (hand[who].closed[0] | 0x100,
                                           tblpos[who], 0, opened));
            }
          for (i = 0; i < n; i++)
            {
              print_colorpixmap2 (pai2pix
                                  (hand[who].closed[i], tblpos[who], 0,
                                   opened));
            }
          if (n > 0 && PAICARD3D && BUILD4 && tblpos[who] == 2)
            {
              print_colorpixmap2 (pai2pix (hand[who].closed[n - 1] | 0x100,
                                           tblpos[who], 0, opened));
            }
        }
    }
  if (naki)
    {
      startpos = width - naki_width - startpos;
      for (j = naki - 1; j >= 0; j--)
        {
          if (hand[who].opened_kind[j])
            {
              int hflag = 0;
              int kmax;

              kmax = 4;
              if (hand[who].opened_kind[j] == H_TIE
                  || hand[who].opened_kind[j] == H_PON)
                kmax = 3;

              for (k = 0; k < kmax; k++)
                {
                  gpixmap2_t paipixmap;
                  hflag = 0;

                  pai = hand[who].opened[j][k];
                  if ((hand[who].opened_kind[j] == H_TIE && k == 0)
                      || (hand[who].opened_kind[j] == H_PON &&
                          hand[who].opened_from[j] != who &&
                          k == (who - hand[who].opened_from[j] + 3) % 4)
                      || (hand[who].opened_kind[j] == H_KAN_OPENED &&
                          hand[who].opened_from[j] != who &&
                          k == (who - hand[who].opened_from[j] + 3) % 4 + 1))
                    {
                      hflag = 1;
                    }
                  if (hand[who].opened_kind[j] == H_KAN_CLOSED
                      && (k == 1 || k == 2))
                    {
                      pai = P_INV;
                    }
                  if (startpos > 0)
                    {
                      if (PAICARD3D && BUILD4 && tblpos[who] == 0)
                        {
                          paipixmap =
                            pai2pix (pai | 0x100, tblpos[who], hflag,
                                     PAIOPEN);
                          print_colorpixmap2_withofs (paipixmap, startpos);
                          paipixmap =
                            pai2pix (pai, tblpos[who], hflag, PAIOPEN);
                          print_colorpixmap2 (paipixmap);
                          startpos = 0;
                        }
                      else
                        {
                          paipixmap =
                            pai2pix (pai, tblpos[who], hflag, PAIOPEN);
                          print_colorpixmap2_withofs (paipixmap, startpos);
                          startpos = 0;
                        }
                      if (PAICARD3D && BUILD4 && tblpos[who] == 2)
                        {
                          paipixmap =
                            pai2pix (pai | 0x100, tblpos[who], hflag,
                                     PAIOPEN);
                          print_colorpixmap2 (paipixmap);
                        }
                    }
                  else
                    {
                      if (k == 0 && PAICARD3D && BUILD4 && tblpos[who] == 0)
                        {
                          paipixmap =
                            pai2pix (pai | 0x100, tblpos[who], hflag,
                                     PAIOPEN);
                          print_colorpixmap2 (paipixmap);
                        }
                      paipixmap = pai2pix (pai, tblpos[who], hflag, PAIOPEN);
                      if (k != 0 && PAICARD3D && BUILD4 && tblpos[who] == 2)
                        {
                          print_colorpixmap2_withmargin (paipixmap, -7);
                        }
                      else
                        {
                          print_colorpixmap2 (paipixmap);
                        }
                      if (PAICARD3D && BUILD4 && tblpos[who] == 2)
                        {
                          paipixmap =
                            pai2pix (pai | 0x100, tblpos[who], hflag,
                                     PAIOPEN);
                          print_colorpixmap2 (paipixmap);
                        }
                    }
                  pmcontainer_set_margin (canvas, 0);
                }
              i++;
              if (PAICARD3D && BUILD4 &&
                  !(tblpos[who] == 0 || tblpos[who] == 2))
                {
                  pmcontainer_set_margin (canvas, OPENPAIMARGIN);
                }
            }
        }
      widget_display (hand_frame[tblpos[who]]);
      widget_display (pai_frame[tblpos[who]]);
      widget_flush (hand_frame[tblpos[who]]);
      widget_flush (pai_frame[tblpos[who]]);
    }
  else
    {
      widget_display (hand_frame[tblpos[who]]);
      widget_flush (hand_frame[tblpos[who]]);
    }
}

static void
draw_hand (global_t *gp, integer_t who)
{
  if (BUILD4)
    draw_hand2 (gp, who);
  else
    draw_hand1 (gp, who);
}

void
draw_title (global_t *gp)
{
  widget_flush (menu_frame);
}

int
ui_key ()
{
  extern widget_t root_widget;
  extern int play_speed;
  if (prog_plview)
    xsleep (play_speed);
  if (widget_eventsqueued (root_widget))
    {
      _widget_handle_event ();
    }
  return 0;
}


void
ui_draw (global_t *gp, int redraw)
{
  int i;

  D (fprintf (stderr, "ui draw"));

  flag_not_needredraw = 0;

  if (flag_not_needredraw)
    {
      return;
    }

  if (redraw)
    {
      ch_play = 1;
    }
  if (ch_play)
    {
      board_reset ();
    }
  tblpos = ((integer_t *) & (maptblpos[vself]._tblpos));        /* ANSI C */

  if (redraw)
    {
      draw_title (gp);
    }

  draw_status (gp);
  if (flag_not_needdraw_hand)
    {
      for (i = 0; i < 4; i++)
        {
          draw_player (gp, i);
        }
    }
  else
    {
      for (i = 0; i < 4; i++)
        {
          draw_player (gp, i);
          draw_river (gp, i);
          draw_hand (gp, i);
        }
    }
  board_sync ();
  ch_play = 0;
  ch_point = 0;
  flag_not_needdraw_hand = 0;
}

void
ui_expose (global_t *gp)
{
}

int
ui_menu (global_t *gp, int c)
{
  return 0;
}


#include "misc.h"

extern integer_t flag_enable_select;
extern integer_t n_select;
extern integer_t readable_socket;
extern integer_t readable_key;
extern integer_t flag_enable_onboard;
extern integer_t iam;

integer_t
ui_event_wait (global_t *gp, int time_out)
{
  fd_set fds;
  struct timeval to, *top;
  int maxfd, fd, fd2;
  int xfd;
  D (fprintf (stderr, "ui event wait"));
  fd = callback_fd (gp);
  fd2 = callback_fd2 (gp);
  if (fd == -1)
    {
      goto xevent;
    }
  else if (fd == -2)
    {
      goto xevent;
    }
  else if (fd < 0 || readable_socket)
    {
      goto xevent;
    }
  xfd = ConnectionNumber (xdisplay);
  FD_ZERO (&fds);
  FD_SET (0, &fds);
  FD_SET (xfd, &fds);
  FD_SET (fd, &fds);
  if (fd2 > 0)
    FD_SET (fd2, &fds);
  if (time_out >= 0)
    {
      to.tv_sec = time_out / 10;
      to.tv_usec = (time_out % 10) * 100000;
      top = &to;
    }
  else
    {
      top = 0;
    }
#define _max(a, b) ((a > b) ? a : b)
  maxfd = _max (fd, xfd);
  if (fd2 > 0)
    maxfd = _max (fd2, maxfd);
  if (-1 == select (maxfd + 1, &fds, (fd_set *) 0, (fd_set *) 0, top))
    {
      goto xevent;
    }
  if ((fd > 0 && FD_ISSET (fd, &fds)) || (fd2 > 0 && FD_ISSET (fd2, &fds)))
    {
      readable_socket = 1;
    }
  if (FD_ISSET (xfd, &fds))
    {
      readable_key = 1;
    xevent:
      if (widget_eventsqueued (root_widget))
        {
          _widget_handle_event ();
          invoke_handlers ();
        }
    }
  return 0;
}

boolean button_map_p = 0;
boolean button_map_flag = 0;

void
board_button_map (integer_t flag)
{
  if (flag_advice)
    {
      widget_map (advice_button);
    }
  if (flag & CAN_TIE)
    widget_map (tie_button);
  if (flag & CAN_PON)
    widget_map (pon_button);
  if (flag & CAN_RON)
    widget_map (ron_button);
  if (flag & CAN_KAN)
    widget_map (kan_button);
  if ((flag & CAN_CANCEL) && flag_cancelbutton)
    widget_map (cancel_button);
  if (flag & CAN_TUMO)
    widget_map (tumo_button);
  if (flag & CAN_REACH)
    widget_map (reach_button);

  button_map_p = 1;
  button_map_flag = flag;
}

void
board_button_display (integer_t flag)
{
  if (flag_advice)
    {
      widget_display (advice_button);
    }
  if (flag & CAN_TIE)
    widget_display (tie_button);
  if (flag & CAN_PON)
    widget_display (pon_button);
  if (flag & CAN_RON)
    widget_display (ron_button);
  if (flag & CAN_KAN)
    widget_display (kan_button);
  if ((flag & CAN_CANCEL) && flag_cancelbutton)
    widget_display (cancel_button);
  if (flag & CAN_TUMO)
    widget_display (tumo_button);
  if (flag & CAN_REACH)
    widget_display (reach_button);
}

void
board_button_unmap ()
{
  widget_unmap (advice_button);
  widget_unmap (kan_button);
  widget_unmap (tumo_button);
  widget_unmap (tie_button);
  widget_unmap (pon_button);
  widget_unmap (cancel_button);
  widget_unmap (ron_button);
  widget_xunmap (reach_button);
  button_map_p = 0;
  button_map_flag = 0;
}

/*
   choice:
   wake up new event loop ;

   This implementation is different from cui version, In `choice', we
   cannot handle socket IO.

 */
static void
display_action ()
{
  if (!BUILD4)
    {
      widget_display (action);
    }
  else
    {
      widget_display (choicestate_frame);
      widget_display (buttonaction);
      widget_display (action);
    }
}

integer_t
choice (global_t *gp, int n_draw, int argc, gpixmap2_t argv[], int st, int flag)
{
  int i;

  canvas = choicestate_frame;
  canvas_clear ();
  print_colorpixmap (pm_choice_state[st]);

  canvas = choice_frame;
  canvas_clear ();
  for (i = 0; i < n_draw; i++)
    {
      print_colorpixmap2 (argv[i]);
    }
  pmcontainer_display (choice_frame);
  pmcontainer_display (choicestate_frame);

  board_button_map (flag);
  board_button_display (flag);
  board_button_flush ();

  /* setup global variable for callback routines */
  flag_not_needredraw = 0;
  flag_enable_select = 1;
  n_select = argc;
  flag_enable_onboard = !!(flag & 0x80000000);
  iam = tblpos[vself];
  if (flag_enable_onboard)
    {
      void board_button3press_callback ();
      widget_define_button_hook (3, board_button3press_callback);
    }
  if (flag_advicealways)
    {
      advice_button_callback (0, 0, 0);
    }

  get_choice (gp);

  n_select = 0;
  widget_define_button_hook (3, 0);

  pmcontainer_clear (choicestate_frame);
  pmcontainer_clear (choice_frame);
  pmcontainer_display (choicestate_frame);
  pmcontainer_display (choice_frame);
  board_button_unmap ();

  advice_clear_flush ();
  display_action ();
  board_button_flush ();

  return choiced;
}

void
mark (widget_t canvas, widget_t linecanvas)
{
  extern integer_t gray;
  if (last_mark)
    {
      pmcontainer_clear (last_mark);
      widget_display (last_mark);
      widget_flush (last_mark);
      last_mark = 0;
    }
  if (last_linepos)
    {
      extern integer_t black;
      widget_configure (last_linepos, resource_cannot_change_model, 0);
      widget_model_change (last_linepos,
                           get_basiccolorpixel (), black, black, black);
      widget_display (last_linepos);
      widget_flush (last_linepos);
      last_linepos = 0;
    }
  canvas_clear ();
  if (flag_posmark)
    {
      print_colorpixmap (pm_current);
    }
  widget_display (canvas);
  widget_flush (canvas);
  last_mark = canvas;

  if (flag_linepos)
    {
      widget_model_change (linecanvas, gray, gray, gray, gray);
      widget_configure (linecanvas, resource_cannot_change_model, 1);
      widget_display (linecanvas);
      widget_flush (linecanvas);
      last_linepos = linecanvas;
    }
}

void
restart_callback callbackarg_param
{
  do_disconnect (game_global);
  last_mark = 0;
}

void
game_redraw ()
{
  integer_t flag = button_map_flag;
  integer_t map = button_map_p;

  ui_draw (game_global, 1);
  button_map_flag = flag;
  button_map_p = map;
  board_redraw (0);
}

void
flush_callback callbackarg_param
{
  extern widget_t root_widget;
    game_redraw ();
}

void
invisible_config ()
{
  extern integer_t black, yellow, white;
  integer_t color;

  color = (flag_invisible_riverpai) ? black : white;
  widget_configure (visible_button, resource_textcolor, color);
}

void
visible_callback callbackarg_param
{
  flag_invisible_riverpai = !flag_invisible_riverpai;
  invisible_config ();
  game_redraw ();
  widget_flush (root_widget);
  widget_sync (root_widget);
}
