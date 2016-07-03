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

#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"
#include "pfinfo.h"
#include <stdlib.h>

widget_t rank;
extern integer_t xwidth, xheight;
extern struct playerinfo playerinfo;

#define N_SLOT 21

static widget_t rankheaderpmframe, header;
static widget_t orderframe[N_SLOT];
static widget_t nameframe[N_SLOT];
static widget_t pointframe[N_SLOT];

static void rank_bg_init ();
static void rank_bg_end ();

static void rank_point_callback callbackarg_param;
static void rank_money_callback callbackarg_param;
static void rank_furi_callback callbackarg_param;

static void
rank_keywait_callback callbackarg_param
{
  rank_bg_end ();
  widget_unmap (rank);
  board_redraw (1);
}

void rank_callback callbackarg_param;

static void
goto_playinfo_callback callbackarg_param
{
  char *s;
  s = widget_get_text (w);

  if (s && *s)
    {
      rank_bg_end ();
      widget_unmap (rank);
      pinfo_callback (w, (integer_t) s, 0);
    }
}

static int sort_money (struct playerinfo *list, struct playerinfo *p);
static int sort_furi (struct playerinfo *list, struct playerinfo *p);
static int sort_point (struct playerinfo *list, struct playerinfo *p);
static int (*sort_func) (struct playerinfo *, struct playerinfo *) = sort_money;

static widget_t
create_rank_frame (widget_t parent)
{
  widget_t frame, frame0, aframe, bframe, cframe;
  widget_t okbutton, quitbutton;
  integer_t i, j, k, h = 0;
  widget_t actionframe, okframe, xframe, header;
  extern int yellow, purple, red, white;

  if (flag_rank_sort_func_str)
    {
      if (!strcmp (flag_rank_sort_func_str, "money"))
        {
          sort_func = sort_money;
        }
      if (!strcmp (flag_rank_sort_func_str, "furi"))
        {
          sort_func = sort_furi;
        }
      if (!strcmp (flag_rank_sort_func_str, "point"))
        {
          sort_func = sort_point;
        }
    }
  rank = frame = widget_create ("frame", parent);

  header = widget_create ("text", frame);
  widget_set_geometry (header, xwidth, 54);
  widget_configure (header, resource_textfont, XLARGETIMESIB);
  widget_configure (header, resource_text, "      NETMAJ Ranking");
  widget_configure (header, resource_textcolor, red);
  widget_configure (header, resource_textshadow, 1);
  h += 54;

  rankheaderpmframe = widget_create ("pmcontainer", frame);
  widget_configure (rankheaderpmframe, resource_pixmapstart, PAI_W * 3);
  widget_set_geometry (rankheaderpmframe, xwidth, PAI_H);
  h += PAI_H;

  xframe = widget_create ("frame", frame);
  widget_configure (xframe, resource_packed, resource_horizontal);
  widget_set_geometry (xframe, xwidth, xheight - (h + BUTTON_H + 4));

  okframe = widget_create ("frame", frame);
  widget_configure (okframe, resource_packed, resource_horizontal);
  widget_set_geometry (okframe, xwidth, BUTTON_H + 4);

  h = xheight - (h + BUTTON_H + 4);
  frame0 = widget_create ("frame", xframe);     /* order */
  widget_set_geometry (frame0, 20, h);

  aframe = widget_create ("frame", xframe);     /* order */
  widget_set_geometry (aframe, 20, h);

  bframe = widget_create ("frame", xframe);     /* name */
  widget_set_geometry (bframe, 110, h);

  cframe = widget_create ("frame", xframe);     /* 1 */
  widget_set_geometry (cframe, 490, h);

  for (i = 0; i < N_SLOT; i++)
    {
      orderframe[i] = widget_create ("text", aframe);
      widget_set_geometry (orderframe[i], 40, 18);
      widget_configure (orderframe[i], resource_textcolor, yellow);
      widget_configure (orderframe[i], resource_textfont, TIMESIB);
    }
  widget_pack (aframe);

  for (i = 0; i < N_SLOT; i++)
    {
      nameframe[i] = widget_create ("text", bframe);
      widget_set_geometry (nameframe[i], 110, 18);
      widget_configure (nameframe[i], resource_textcolor, white);
      widget_configure (nameframe[i], resource_textfont, MEDIUMJP);
      widget_add_callback (nameframe[i], resource_buttonpress_callback,
                           goto_playinfo_callback, "");

    }
  widget_pack (bframe);

  for (i = 0; i < N_SLOT; i++)
    {
      pointframe[i] = widget_create ("text", cframe);
      widget_set_geometry (pointframe[i], 490, 18);
      widget_configure (pointframe[i], resource_textfont, TINYJP);
    }
  widget_pack (cframe);

  widget_configure (okframe, resource_frame_start, 270);
  rankokbutton = okbutton = button_create (okframe, BUTTON_W, BUTTON_H,
                                           "ok", yellow, TIMESIB,
                                           (void *) rank_keywait_callback,
                                           rank);
  rankmoneybutton = okbutton =
    button_create (okframe, BUTTON_W, BUTTON_H, "money", yellow, TIMESIB,
                   (void *) rank_money_callback, rank);
  rankpointbutton = okbutton =
    button_create (okframe, BUTTON_W, BUTTON_H, "point", yellow, TIMESIB,
                   (void *) rank_point_callback, rank);
  rankfuributton = okbutton =
    button_create (okframe, BUTTON_W, BUTTON_H, "furi", yellow, TIMESIB,
                   (void *) rank_furi_callback, rank);
  widget_pack (okframe);
  widget_pack (frame);
  widget_set_geometry (frame, xwidth, xheight);
  widget_fix (frame);
  widget_configure (frame, resource_wrap, 0);
  return frame;
}

void
rank_init ()
{
}

void
rank_clear ()
{
}

static int
sort_money (struct playerinfo *list, struct playerinfo *p)
{
  int x = p->pnt * 100 + p->tn * 500;
  int y = list->pnt * 100 + list->tn * 500;
  if (y < x)
    return 1;
  return 0;
}

static int
sort_point (struct playerinfo *list, struct playerinfo *p)
{
  int x = (p->pnt * 100);
  int y = (list->pnt * 100);

  if (p->game)
    {
      x /= p->game;
    }
  else
    {
      x = 0;
    }
  if (list->game)
    {
      y /= list->game;
    }
  else
    {
      y = 0;
    }

  if (y < x)
    return 1;
  return 0;
}

static int
sort_furi (struct playerinfo *list, struct playerinfo *p)
{
  int x = (p->furi_cnt);
  int y = (list->furi_cnt);

  if (y < x)
    return 1;
  return 0;
}

struct playerinfo *
playerinfo_sort_append (struct playerinfo *list, struct playerinfo *p,
                        integer_t (*compare) (struct playerinfo *, struct playerinfo *))
{
  struct playerinfo *q = list, *last = 0;

  if (!q)
    return p;

  while (q)
    {
      if ((*compare) (q, p))
        {
          if (last)
            {
              last->next = p;
            }
          else
            {
              list = p;
            }
          p->next = q;
          return list;
        }
      else
        {
          ;
        }
      last = q;
      q = q->next;
    }
  if (last)
    {
      last->next = p;
    }
  return list;
}


void
rank_display ()
{
  char buffer[100];
  struct playerinfo *base = 0, *p = 0;
  int i;

  rank_clear ();

  if (0 == read_player_first_info ())
    {
      return;
    }
  do
    {
      p = (struct playerinfo *) xmalloc (sizeof (struct playerinfo));
      memcpy (p, &playerinfo, sizeof (struct playerinfo));
      base =
        (struct playerinfo *) playerinfo_sort_append (base, p,
                                                      (void *) sort_func);
    }
  while (read_player_next_info ());
  if (!p)
    {
      return;
    }

  widget_configure (orderframe[0], resource_text, "No");
  for (p = base, i = 1; i < 19; i++)
    {
      if (p)
        {
          sprintf (buffer, "%2d", i);
          widget_configure (orderframe[i], resource_text, buffer);
          widget_configure (orderframe[i], resource_border_width, 0);
          p = p->next;
        }
      else
        {
          widget_configure (orderframe[i], resource_border_width, 0);
        }
    }
  widget_configure (nameframe[0], resource_text, "Name");
  for (p = base, i = 1; i < 19; i++)
    {
      if (p)
        {
          if (!p->rname[0])
            strcpy (p->rname, p->name);
          widget_configure (nameframe[i], resource_text, p->rname);
          widget_configure (nameframe[i], resource_relief, relief_sunken);
          widget_configure (nameframe[i], resource_border_width, 1);
          p = p->next;
        }
      else
        {
          widget_configure (nameframe[i], resource_text, "");
          widget_configure (nameframe[i], resource_border_width, 0);
        }
    }

  sprintf (buffer, "%10s%10s%10s %5s%5s%5s%5s %6s%6s",
           "TOTAL", "POINT", "CHIP", "#TOP", "#2", "#3", "#4", "AGARI",
           "FURI");
  widget_configure (pointframe[0], resource_text, buffer);
  for (p = base, i = 1; i < 19; i++)
    {
      if (p)
        {
          int games, len;
          games = p->game ? p->game : 1;
          sprintf (buffer, "%10d%10d%10d %5d%5d%5d%5d %6d%6d",
                   p->pnt * 100 + p->tn * 500, p->pnt * 100, p->tn * 500,
                   p->juni[1], p->juni[2], p->juni[3], p->juni[4],
                   (p->tumo_pnt + p->ron_pnt) / games, p->furi_pnt / games);

          widget_configure (pointframe[i], resource_text, buffer);
          p = p->next;
        }
      else
        {
          widget_configure (pointframe[i], resource_text, "");
        }
    }
  while (base)
    {
      p = base;
      base = base->next;
      xfree ((void *) p);
    }
}

static void rank_bg_init ();
static void rank_bg_end ();

void
rank_callback callbackarg_param
{
  extern widget_t board;
  extern widget_t about;

  snd (audioplay_rank ());
  if (!rank)
    {
      extern widget_t root_widget;
        create_rank_frame (root_widget);
        lang_set ();
    }

  rank_bg_init ();
  widget_unmap (about);
  rank_display ();

  widget_unmap (board);
  widget_map (rank);
  widget_display (rank);
  widget_flush (rank);
}


static void
rank_point_callback callbackarg_param
{
  sort_func = sort_point;
  flag_rank_sort_func_str = "point";
  rank_callback (w, 0, 0);
}

static void
rank_furi_callback callbackarg_param
{
  sort_func = sort_furi;
  flag_rank_sort_func_str = "furi";
  rank_callback (w, 0, 0);
}

static void
rank_money_callback callbackarg_param
{
  sort_func = sort_money;
  flag_rank_sort_func_str = "money";
  rank_callback (w, 0, 0);
}

#ifdef USE_RAND
#define random	rand
#define srandom srand
#endif

static int n_count;
static int pais[13];

static int
paicompare (const void *p, const void *q)
{
  return (*((int *)p) - *((int *)q));
}

static int
bg_action ()
{
  widget_t canvas;
  unsigned int i;

retry:
  canvas = rankheaderpmframe;
  if (n_count == 13)
    {
      qsort (pais, 13, sizeof (int), paicompare);
      pmcontainer_clear (rankheaderpmframe);
      for (i = 0; i < 13; i++)
        {
          print_colorpixmap2 (pai2pix (pais[i], 3, 0, 0));
        }
      n_count++;
    }
  else
    {
      if (n_count == 14)
        {
          n_count++;
          goto next;
        }
      if (n_count < 13)
        {
        pairetry:
          pais[n_count] = i = (((unsigned int) (random ())) % 136);
          if (i < 4)
            goto pairetry;
          print_colorpixmap2 (pai2pix (i, 3, 0, 0));
          n_count++;
        }
      else
        {
          pmcontainer_clear (rankheaderpmframe);
          n_count = 0;
          goto retry;
        }
    }

next:
  widget_display (rankheaderpmframe);
  widget_flush (rankheaderpmframe);

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
rank_bg_init ()
{
  srandom (time (0));
  n_count = 0;
  stop_timer ();
  pmcontainer_clear (rankheaderpmframe);
  set_timer_handler (bg_handler, 7);
}


static void
rank_bg_end ()
{
  clear_timer_handler ();
}
