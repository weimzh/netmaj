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
 * advice2.c --
 *
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Sat Sep 23 04:34:23 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Fri Sep 29 06:06:51 1995
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */
#include "xnet.h"
#include "xnetmaj.h"
#include "pixread.h"
#include "pmutil.h"

#include "bitmap/attr/252/tiny0.xpm"
#include "bitmap/attr/252/tiny1.xpm"
#include "bitmap/attr/252/tiny2.xpm"
#include "bitmap/attr/252/tiny3.xpm"
#include "bitmap/attr/252/tiny4.xpm"
#include "bitmap/attr/252/tinyn.xpm"
#include "bitmap/attr/252/tinys.xpm"
#include "bitmap/attr/252/tinyw.xpm"
#include "bitmap/attr/252/tinye.xpm"
#include "bitmap/attr/252/tiny_sp.xpm"

int pai_genbutu[4][36];
int pai_reveal[36];

static struct gpixmap2 pm_tiny_digit[5];
static struct gpixmap2 pm_tiny_home[4];
static struct gpixmap2 pm_tiny_space;

void
read_tiny_pixmaps ()
{
  read_pix_idx (tiny0_xpm, pm_tiny_digit[0]);
  read_pix_idx (tiny1_xpm, pm_tiny_digit[1]);
  read_pix_idx (tiny2_xpm, pm_tiny_digit[2]);
  read_pix_idx (tiny3_xpm, pm_tiny_digit[3]);
  read_pix_idx (tiny4_xpm, pm_tiny_digit[4]);
  read_pix_idx (tinyE_xpm, pm_tiny_home[0]);
  read_pix_idx (tinyS_xpm, pm_tiny_home[1]);
  read_pix_idx (tinyW_xpm, pm_tiny_home[2]);
  read_pix_idx (tinyN_xpm, pm_tiny_home[3]);
  read_pix (tiny_space, pm_tiny_space);
}

static void
collect_reveal_pai_info (global_t *gp)
{
  integer_t i, j, p, k;
  river_t *rp, *lastrv = 0;
  integer_t paikind;

  memset (pai_genbutu, 0, sizeof (pai_genbutu));
  memset (pai_reveal, 0, sizeof (pai_reveal));

  /* in river */
  for (p = 0; p < 4; p++)
    {
      integer_t reach = -1;
      for (i = 0; i <= rv_cur[p] && rv[p][i].out; i++)
        {
          if (T_GET (rv[p][i].attr) != T_HN_KAN_C)
            {
              paikind = P_KIND (rv[p][i].out);
              pai_genbutu[p][paikind] = 1;
            }
          if (T_GET (rv[p][i].attr) == T_RV_RCH)
            {
              reach = i;
            }
          if (R_GET (rv[p][i].attr) == R_NUL)
            {
              paikind = P_KIND (rv[p][i].out);
              pai_reveal[paikind]++;
            }
          lastrv = &rv[p][i];
        }                       /* for */

      if (reach > 0)
        {
          rp = &(rv[p][reach]);
        }
      else
        {
          rp = lastrv;
        }
#if 1
      while (rp && rp->next)
        {
          if (T_GET (rp->attr) != T_HN_KAN_C)
            {
              paikind = P_KIND (rp->out);
              pai_genbutu[p][paikind] = 1;
            }
          rp = rp->next;
        }
#endif
      /* opened furo-pai */
      for (j = 3; j >= 0; j--)
        {
          if (hand[p].opened_kind[j])
            {
              for (k = 0; k < 4; k++)
                {
                  if (k == 3 && (hand[p].opened_kind[j] == H_TIE
                                 || hand[p].opened_kind[j] == H_PON))
                    {
                      break;
                    }
                  paikind = P_KIND (hand[p].opened[j][k]);
                  pai_reveal[paikind]++;
                }
            }
        }
    }                           /* for */
/* dora */
  for (i = 0; i < 5; i++)
    {
      paikind = P_KIND (mt_dora (gp, i));
      pai_reveal[paikind]++;
    }
/* in hand */
  for (i = 0; i < hand[vself].closed_num; i++)
    {
      paikind = P_KIND (hand[vself].closed[i]);
      pai_reveal[paikind]++;
    }
}


void
hand_pai_advice_info_clear ()
{
  widget_t canvas;
  pmcontainer_clear (hand_pai_advice_info);
  widget_display (hand_pai_advice_info);
  widget_flush (hand_pai_advice_info);
}

void
hand_pai_advice_info_display (global_t *gp)
{
  integer_t i, j, k;
  widget_t canvas = hand_pai_advice_info;
  canvas_clear ();
  widget_configure (canvas, resource_pixmapstart, HANDPAISTART);

  collect_reveal_pai_info (gp);

  for (i = 0; i < hand[vself].closed_num; i++)
    {
      integer_t pai = P_KIND (hand[vself].closed[i]);
      integer_t reveal = pai_reveal[pai];
      integer_t margin;
      gpixmap2_t pm;
      if (i == hand[vself].closed_num - 1)
        margin = HANDPAIMARGIN;
      else
        margin = 0;
      if (reveal < 0 || reveal > 4)
        reveal = 4;

      if (flag_advicedigit)
        {
          pm = &pm_tiny_digit[4 - reveal];
        }
      else
        {
          pm = &pm_tiny_space;
        }
      print_colorpixmap2_withmargin (pm, margin);

      if (flag_advicepai)
        {
          for (j = 0; j < 4; j++)
            {
              if (j != vself)
                {
                  if (pai_genbutu[j][pai])
                    {
                      print_colorpixmap (pm_tiny_home[j]);
                    }
                  else
                    {
                      print_colorpixmap (pm_tiny_space);
                    }
                }
              else
                {
                  print_colorpixmap (pm_tiny_space);
                }
            }
        }
      else
        {
          for (j = 0; j < 4; j++)
            {
              print_colorpixmap (pm_tiny_space);
            }
        }
    }
}
