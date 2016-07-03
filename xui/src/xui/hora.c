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
 * hora.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Sun Apr 23 01:03:16 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "xnet.h"
#include "xnetmaj.h"
#include "pixread.h"
#include "pmutil.h"

#define	print_colorpixmap2(arg) \
{ gpixmap2_t xpm = arg; \
    if((*xpm).pm) {pmcontainer_add(canvas, (*xpm).pm, (*xpm).Mpm, 1);} }

static widget_t hora_canvas;
static widget_t hora_handframe;
static widget_t hora_openhandframe;
static widget_t hora_doraframe;
static widget_t hora_uradoraframe;

extern integer_t xwidth, xheight;
static global_t _hora_global;
global_t *hora_global = &_hora_global;

void
hora_init (widget_t parent, integer_t width)
{
  widget_t w, d, p, o, dorastr, uradorastr;
  widget_t canvas;

  w = widget_create ("frame", parent);
  d = widget_create ("frame", w);
  p = widget_create ("frame", w);
  o = widget_create ("frame", w);

  widget_configure (d, resource_packed, resource_horizontal);
  dorastr = widget_create ("pmcontainer", d);
  hora_doraframe = widget_create ("pmcontainer", d);
  uradorastr = widget_create ("pmcontainer", d);
  hora_uradoraframe = widget_create ("pmcontainer", d);

  widget_set_geometry (dorastr, PAI_W, PAI_H + 4);
  widget_set_geometry (uradorastr, PAI_W, PAI_H + 4);

  canvas = dorastr;
  canvas_clear ();
  print_colorpixmap (pm_dora_str);

  canvas = uradorastr;
  canvas_clear ();
  print_colorpixmap (pm_uradora_str);
  widget_set_geometry (hora_doraframe, PAI_W * 5, PAI_H + 4);
  widget_set_geometry (hora_uradoraframe, PAI_W * 5, PAI_H + 4);

  hora_handframe = widget_create ("pmcontainer", p);
  widget_set_geometry (hora_handframe, width, PAI_H + 4);

  hora_openhandframe = widget_create ("pmcontainer4", o);
  widget_set_geometry (hora_openhandframe, width, PAI_H + 4);

  widget_pack (w);
  hora_canvas = w;
}

static integer_t hora_closed_num;
static integer_t hora_closed[14];
static integer_t hora_lastpai;
static integer_t hora_who;
static integer_t hora_opened_kind[4];
static integer_t hora_opened_from[4];
static integer_t hora_opened[4][4];
static integer_t hora_dora[5];
static integer_t hora_uradora[5];

void
hora_reset ()
{
  hora_closed_num = 14;
  memset (hora_closed, 0, sizeof (hora_closed));
  hora_lastpai = 0;
  memset (hora_opened_kind, 0, sizeof (hora_opened_kind));
  memset (hora_opened_from, 0, sizeof (hora_opened_from));
  memset (hora_opened, 0, sizeof (hora_opened));
  memset (hora_dora, 0, sizeof (hora_dora));
  memset (hora_uradora, 0, sizeof (hora_uradora));
}

void
hora_set (global_t *gp, integer_t who)
{
  int i, j, k, pai;
  int furo = 0;

  hora_lastpai = 0;
  hora_who = who;
  for (i = 0; i < 5; i++)
    {
      hora_dora[i] = mt_dora (gp, i);
      hora_uradora[i] = mt_uradora (gp, i);
    }
  if (who == -1)
    return;
  hora_closed_num = hand[who].closed_num;
  for (i = 0; i < 14; i++)
    {
      hora_closed[i] = hand[who].closed[i];
    }
  for (j = 0; j < 4; j++)
    {
      hora_opened_kind[j] = hand[who].opened_kind[j];
      hora_opened_from[j] = hand[who].opened_from[j];
      furo |= !!(hand[who].opened_kind[j]);
    }
  for (j = 0; j < 4; j++)
    {
      for (k = 0; k < 4; k++)
        {
          hora_opened[j][k] = hand[who].opened[j][k];
        }
    }

  if (rvp && T_GET (rvp->attr) == T_HN_TSM)
    {
      hora_lastpai = rvp->out;
    }
  else if (rvp && R_GET (rvp->attr) == R_RON)
    {
      hora_lastpai = rvp->out;
    }
}

void
draw_hora ()
{
  int i, j, k, pai;
  int naki;
  widget_t canvas;

  canvas = hora_doraframe;
  canvas_clear ();
  for (i = 0; i < 5; i++)
    {
      pai = hora_dora[i];
      if (pai)
        {
          print_colorpixmap2 (pai2pix (pai, 3, 0, PAIOPEN));
        }
    }
  canvas = hora_uradoraframe;
  canvas_clear ();
  for (i = 0; i < 5; i++)
    {
      pai = hora_uradora[i];
      if (pai)
        {
          print_colorpixmap2 (pai2pix (pai, 3, 0, PAIOPEN));
        }
    }

  canvas = hora_handframe;
  canvas_clear ();

  if (hora_lastpai && hora_closed[hora_closed_num - 1] != hora_lastpai)
    {
      for (i = 0; i < hora_closed_num; i++)
        {
          print_colorpixmap2 (pai2pix (hora_closed[i], 3, 0, PAIOPEN));
        }
      pmcontainer_set_margin (hora_handframe, HANDPAIMARGIN);
      print_colorpixmap2 (pai2pix (hora_lastpai, 3, 0, PAIOPEN));
    }
  else
    {
      for (i = 0; i < hora_closed_num - 1; i++)
        {
          print_colorpixmap2 (pai2pix (hora_closed[i], 3, 0, PAIOPEN));
        }
      pmcontainer_set_margin (hora_handframe, HANDPAIMARGIN);
      print_colorpixmap2 (pai2pix (hora_closed[i], 3, 0, PAIOPEN));
    }

  naki = 0;
  for (j = 3; j >= 0; j--)
    {
      if (hora_opened_kind[j])
        {
          naki++;
        }
    }

  canvas = hora_openhandframe;
  canvas_clear ();
  if (naki)
    {
      widget_configure (hora_openhandframe, resource_pixmapstart,
                        (PAI_W * 15) - (PAI_W * 3 * naki));
      widget_configure (hora_openhandframe, resource_direction, 0);
      for (j = 3; j >= 0; j--)
        {
          if (hora_opened_kind[j])
            {
              int hflag;
              pmcontainer_set_margin (canvas, OPENPAIMARGIN);
              for (k = 0; k < 4; k++)
                {
                  int pai;
                  hflag = 0;
                  if (k == 3 && (hora_opened_kind[j] == H_TIE
                                 || hora_opened_kind[j] == H_PON))
                    {
                      break;
                    }
                  pai = hora_opened[j][k];
                  if (hora_opened_kind[j] == H_TIE && k == 0)
                    {
                      hflag = 1;
                    }
                  if (hora_opened_kind[j] == H_PON &&
                      hora_opened_from[j] != hora_who &&
                      k == (hora_who - hora_opened_from[j] + 3) % 4)
                    {
                      hflag = 1;
                    }
                  if (hora_opened_kind[j] == H_KAN_OPENED &&
                      hora_opened_from[j] != hora_who &&
                      k == (hora_who - hora_opened_from[j] + 3) % 4 + 1)
                    {
                      hflag = 1;
                    }
                  if (hora_opened_kind[j] == H_KAN_CLOSED
                      && (k == 1 || k == 2))
                    {
                      pai = P_INV;
                    }
                  if (k == 0 && pai == 0)
                    {
                      break;
                    }           /* avoid bug */
                  print_colorpixmap2 (pai2pix (pai, 3, hflag, PAIOPEN));
                }
            }
          pmcontainer_set_margin (canvas, 0);
        }
    }

  widget_display (hora_canvas);
  widget_flush (hora_canvas);
}
