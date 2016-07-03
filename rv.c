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
 * 5-7-2006 Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"

void
rv_new (global_t *gp)
{
  int i, j;

  rvp = 0;
  cur_player = 0;
  for (i = 0; i < 4; i++)
    {
      rv_cur[i] = 0;
      for (j = 0; j < RIVER_MAX; j++)
        {
          rv[i][j].next = 0;
          rv[i][j].prev = 0;
          rv[i][j].attr = 0;
          rv[i][j].in = P_NULL;
          rv[i][j].out = P_NULL;
        }
    }
}

void
rv_push (global_t *gp, int to)
{
  if (rvp)
    {
      if (cur_player != to)
        rvp->next = rv[to] + rv_cur[to];
      else
        rvp->next = rv[to] + rv_cur[to] + 1;
      rvp->next->prev = rvp;
      rvp = rvp->next;
      rv_cur[cur_player]++;
    }
  else
    {
      rvp = rv[to] + rv_cur[to];
    }
  cur_player = to;
  if (plog)
    fprintf (plog, "rv_push: rvp = rv[%d][%d]\n", to, rv_cur[to]);
}

int
rv_find_player (global_t *gp, river_t *p)
{
  if (p < &rv[0][0] || p > &rv[3][RIVER_MAX])
    return 0;
  return (p - &rv[0][0]) / RIVER_MAX;
}

int
rv_find_cur (global_t *gp, river_t *p)
{
  if (p < &rv[0][0] || p > &rv[3][RIVER_MAX])
    return 0;
  return (p - &rv[0][0]) % RIVER_MAX;
}
