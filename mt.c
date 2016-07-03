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
 * 5/7/2006 Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"

/* common functions */

int
mt_dora (global_t *gp, int n)
{
  return (n > mt_top_cnt) ? 0 : mt[n + 122];
}

int
mt_uradora (global_t *gp, int n)
{
  return (n > mt_top_cnt) ? 0 : mt[n + 127];
}

int
mt_rest (global_t *gp)
{
  if (mt_next_cnt2 == 0)
    {                           /* server */
      return 122 - mt_top_cnt - mt_normal_cnt;
    }
  else
    {                           /* client */
      return 122 - mt_next_cnt2;
    }
}

/* client functions */

void
mt_new (global_t *gp)
{
  int i;

  for (i = 0; i < 136; i++)
    {
      mt[i] = P_NULL;
    }
  mt_top_cnt = 0;
  mt_normal_cnt = 0;
}

void
mt_next_new (global_t *gp)
{
  int i;

  mt_next_cnt = 0;
  mt_next_cnt2 = 13 * 4;
  for (i = 0; i < 256; i++)
    mt_next[i] = 0;
}

void
mt_setdora (global_t *gp, int n, int v)
{
  if (v)
    {
      if (n > mt_top_cnt)
        mt_top_cnt = n;
      mt[n + 122] = v;
    }
}

void
mt_seturadora (global_t *gp, int n, int v)
{
  if (v)
    {
      if (n > mt_top_cnt)
        mt_top_cnt = n;
      mt[n + 127] = v;
    }
}

/* server functions */

int
mt_get (global_t *gp)
{
  if (mt_normal_cnt + mt_top_cnt >= 122)
    return 0;
  return mt[mt_normal_cnt++];
}

int
mt_gettop (global_t *gp)
{
  int n;

  n = mt_top_cnt;
  if (n >= 4)
    return 0;
  mt_top_cnt++;
  return mt[n + 132];
}

void
mt_make (global_t *gp)
{
  int i, j;
  int tmp;

  for (i = 0; i < 256; i++)
    mt_push (gp, i, 0);

  if (mt_next_cnt != 136)
    {
      printf ("error in mt_make\n");
      fclose (stdin);
      exit (1);
    }
  for (i = 0; i < 136; i++)
    {
      mt[i] = mt_next[i + 4] + 3;
    }

  for (i = 0; i < 136; i++)
    {
      j = random () % 136;
      tmp = mt[i];
      mt[i] = mt[j];
      mt[j] = tmp;
    }

  mt_top_cnt = 0;
  mt_normal_cnt = 0;
  mt_next_new (gp);
  mt_next_cnt2 = 0;
}

void
mt_haipai (global_t *gp, pai_t *buf)
{
  int x;

  for (x = 0; x < 13; x++)
    {
      buf[x] = mt_get (gp);
    }
}

int
mt_push (global_t *gp, pai_t p, int mode)
{
  if (mode)
    mt_next_cnt2++;
  if (K_MAN_1 <= P_KIND (p) && P_KIND (p) < K_MAX && !mt_next[p])
    {
      mt_next[p] = ++mt_next_cnt;
      return mt_next_cnt;
    }
  return 0;
}

int
mt_rest_kind (global_t *gp, pai_t p)
{
  int i;
  int ret = 4;

  if (p <= 0 || p >= K_MAX)
    return 0;
  for (i = 0; i < 4; i++)
    {
      if (mt_next[p * 4 + i])
        ret--;
    }
  return ret;
}
