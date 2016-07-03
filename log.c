/*
 * NETMAJ - network mahjongg -  Copyright Notice
 *     Copyright (C) 1994, 1995, 1996 Koji Suzuki (suz@kt.rim.or.jp)
 *
 *  You can redistribute it and/or modify this software under the terms 
 * of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 *  This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * 5/7/2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"
#include "result.h"

char *
pai2str (pai_t n)
{
  static char buf[4];
  extern char *pai_strings[];
  return pai_strings[n];
}

char *
attr2str (attr_t n)
{
  static char buf[4];
  buf[0] = F_CHARS[F_GET (n) >> F_SHFT];
  buf[1] = T_CHARS[T_GET (n) >> T_SHFT];
  buf[2] = R_CHARS[R_GET (n) >> R_SHFT];
  buf[3] = 0;
  return buf;
}

void
log_play (global_t *gp, FILE *fp)
{
  int i, j, k;
  int rule;
  pack_rule (&rule);

#ifdef USE_TIME_STUMP
#include <time.h>
#define FORMAT "%a %b %e %H:%M:%S %Z %Y"
  {
    time_t tval;
    char buf[80];
    time (&tval);
    strftime (buf, sizeof (buf), FORMAT, localtime (&tval));
    fprintf (fp, "Date: %s\n\n", buf);
  }
#endif

  fprintf (fp, "rule: %d\n", rule);
  /* play */
  fprintf (fp, "%s %d %s %d %s %s %d\n", bturn_names[big_turn],
           small_turn + 1, sturn_sfx, small_turn_cnt, sturn_cnt_sfx, rest_str,
           rest_point);

  /* doras */
  fprintf (fp, "\t%s %s%s%s%s%s\n\t%s %s%s%s%s%s\n\n", dora_str,
           pai2str (mt_dora (gp, 0)), pai2str (mt_dora (gp, 1)),
           pai2str (mt_dora (gp, 2)), pai2str (mt_dora (gp, 3)),
           pai2str (mt_dora (gp, 4)), uradora_str,
           pai2str (mt_uradora (gp, 0)), pai2str (mt_uradora (gp, 1)),
           pai2str (mt_uradora (gp, 2)), pai2str (mt_uradora (gp, 3)),
           pai2str (mt_uradora (gp, 4)));

  for (i = 0; i < 4; i++)
    {
      /* name */
      fprintf (fp, "%s %s\t%5d(%d)\n", home_names[i], player[pplayer[i]].name,
               player[pplayer[i]].gpoint, player[pplayer[i]].ppoint);
      /* init hand */
      fprintf (fp, "\t");
      for (j = 0; j < init_hand[i].closed_num; j++)
        fprintf (fp, "%s", pai2str (init_hand[i].closed[j]));
      fprintf (fp, "\n");

      /* river.in */
      fprintf (fp, "\t");

      for (j = 0; j <= rv_cur[i] && rv[i][j].out; j++)
        {
          if (!rv[i][j].out)
            break;
          if (rv[i][j].in == rv[i][j].out)
            fprintf (fp, "%s", pai2str (3));
          else
            fprintf (fp, "%s", pai2str (rv[i][j].in));
        }

      fprintf (fp, "\n");

      /* river.out */
      fprintf (fp, "\t");
      for (j = 0; j <= rv_cur[i] && rv[i][j].out; j++)
        fprintf (fp, "%s", pai2str (rv[i][j].out));
      fprintf (fp, "\n");

      /* river.attr */
      fprintf (fp, "\t");
      for (j = 0; j <= rv_cur[i] && rv[i][j].out; j++)
        fprintf (fp, "%s", attr2str (rv[i][j].attr));
      fprintf (fp, "\n");

      /* hand */
      fprintf (fp, "\t");
      for (j = 0; j < hand[i].closed_num; j++)
        fprintf (fp, "%s", pai2str (hand[i].closed[j]));
      for (j = 0; j < 4; j++)
        if (hand[i].opened_kind[j])
          {
            fprintf (fp, " ");
            for (k = 0; k < 4; k++)
              fprintf (fp, "%s", pai2str (hand[i].opened[j][k]));
          }
      fprintf (fp, "\n");
      fprintf (fp, "\n");
    }
  log_result (gp, fp);
  fprintf (fp, "\n");
  fflush (fp);
}

void
log_hand (global_t *gp, FILE *fp, int p)
{
  int j, k;
  int kind;

  fprintf (fp, "\t");
  for (j = 0; j < hand[p].closed_num; j++)
    fprintf (fp, "%s", pai2str (hand[p].closed[j]));
  for (j = 0; j < 4; j++)
    if (kind = hand[p].opened_kind[j])
      {
        fprintf (fp, " ");
        for (k = 0; k < 4; k++)
          {
            if (k == 3 && (kind == H_TIE || kind == H_PON))
              break;
            fprintf (fp, "%s", pai2str (hand[p].opened[j][k]));
          }
      }
  fprintf (fp, "\n");
}

void
log_result (global_t *gp, FILE *fp)
{
  int i;
  for (i = 0; i < 5; i++)
    if (result[i].flag)
      {
        fprintf (fp, "-- %s\n", result_names[result[i].flag]);
        log_yaku (gp, fp, result + i);
      }
}

void
log_yaku (global_t *gp, FILE *fp, result_t *yp)
{
  int j;
  int iy[10];

  fprintf (fp, "\n\t%s %d %s %d %s %d %s %d\n", home_names[0], yp->points[0],
           home_names[1], yp->points[1], home_names[2], yp->points[2],
           home_names[3], yp->points[3]);
  if (yp->fu && yp->fan)
    fprintf (fp, "\t%d %s %d %s\n", yp->fu, fu_sfx, yp->fan, fan_sfx);
  result_cvt_to_int (yp, iy);
  for (j = 0; j < Y_MAX; j++)
    {
      if (result_str[j] && Y_GET (iy, j))
        fprintf (fp, "\t\t%s\n", result_str[j]);
    }
  if (yp->dora)
    fprintf (fp, "\t\t%s %d\n", dora_str, yp->dora);
  if (yp->uradora)
    fprintf (fp, "\t\t%s %d\n", uradora_str, yp->uradora);
  if (yp->akadora)
    fprintf (fp, "\t\t%s %d\n", akadora_str, yp->akadora);
}

void
log_game (global_t *gp, FILE *fp)
{
  int i;
  fprintf (fp, "\ngame result:\n");
  fprintf (fp, "\t%-10s       %5s (%3s)         %s\n", "name", "point", "tip",
           "result");
  for (i = 0; i < 4; i++)
    {
      fprintf (fp, "\t%-10s(%s) %5d (%3d)         %6d\n", player[i].name,
               home_names[(i - first_player + 4) % 4], player[i].rpoint,
               player[i].tpoint,
               player[i].rpoint * 100 + player[i].tpoint * 500);
    }
  fprintf (fp, "\n");
  fflush (fp);
}
