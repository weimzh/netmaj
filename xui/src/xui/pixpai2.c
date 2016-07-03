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
 * pixpai.c -- 
 * 
 * Author          : suz@kt.rim.or.jp
 * Created On      : Thu May  2 04:57:22 JST 1996
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 *  Copyright (C) 1996 Koji Suzuki All rights reserved
 */
#include "xnet.h"
#include "pixread.h"

#ifdef HUGEPAI
#define TMP_W    100
#define TMP_H    100
#else
#define TMP_W    42
#define TMP_H    64
#endif

#define N_PAICOLOR (sizeof(paicolor)/sizeof(char*))
/*
   This file contains two global functions;
   pai2pix (n, pos, reach, opened)
   pixpai_init()

   If you'd like to change pai xpm data, please rewrite these functions;

   pai2pix(n, pos, reach, opened)

   n :  pai serial number, detail of pai serial number see below (include Japanese character, sorry);
   pos: 0, 1, 2, 3 means position of game frame. 3 means player(you).
   reach: reach pai or not(0 or 1) if pai is reach pai, then 1;
   opened: 0 or PAIOPEN, PAIDARK or PAIOPEN|PAIDARK
   0 means normal closed pai;
   PAIOPEN means opened pai on river
   PAIDARK means dark pai ( only PAIDARK is not specified )
   PAIOPEN|PAIDARK means opened dark pai ( pon'ed, kan'ed or tie'ed )

   pixpai_init
   pais of number 5 have initialized before starting play.
 */

#ifdef HUGEPAI
#include "bitmap/pai4/paidata.h"
#include "bitmap/pai4/paibase.h"
#else
#include "bitmap/pai3/paidata.h"
#include "bitmap/pai3/paibase.h"
#endif

/*
   type:

      |d|c|rot|
       1 1  2
       | |  |
       | |  +-  rot  0: normal 1: simo 2: toi 3: kami
       | +----  0: open 1: close 
       +------  0: normal 1: dark

   kind:   0 : rev
	 1-9 : man-zu
	10-18: pin-zu
	19-27: sou-zu
	28-34: jihai
	35-37: aka
	38-39: work

*/
static char **
makepai (int type, int kind)
{
  int x, y, c, d;
  int i, j, cnt;
  int base_x, base_y;
  static char *paibuf[TMP_H];
  static char paibase[TMP_H][TMP_W];

  static char **basepais0[] = {
    base_24_xpm, base_24h_xpm, base_24_xpm, base_24h_xpm,
    base_24_xpm, base_24h_xpm, base_24_xpm, base_24h_xpm,
    base_24d_xpm, base_24dh_xpm, base_24d_xpm, base_24dh_xpm,
    base_24_xpm, base_24h_xpm, base_24_xpm, base_24h_xpm,
  };
  static char **revpais0[] = {
    rev_24_xpm, rev_24h_xpm, rev_24_xpm, rev_24h_xpm,
    rev_24_xpm, rev_24h_xpm, rev_24_xpm, rev_24h_xpm,
    rev_24_xpm, rev_24h_xpm, rev_24_xpm, rev_24h_xpm,
    rev_24_xpm, rev_24h_xpm, rev_24_xpm, rev_24h_xpm,
  };
  static char **basepais1[] = {
    base_24o3_xpm, base_24o0_xpm, base_24o3_xpm, base_24o0_xpm, /* OPEN */
    base_24c3_xpm, base_24c0_xpm, base_24c3_xpm, base_24c0_xpm, /* CLOSE */
    base_24do3_xpm, base_24do0_xpm, base_24do3_xpm, base_24do0_xpm,
    base_24c3_xpm, base_24c0_xpm, base_24c3_xpm, base_24c0_xpm,
  };
  static char **revpais1[] = {
    rev_24o3_xpm, rev_24o0_xpm, rev_24o3_xpm, rev_24o0_xpm,
    rev_24c3_xpm, rev_24c0_xpm, rev_24c3_xpm, rev_24c2_xpm,
    rev_24do3_xpm, rev_24do0_xpm, rev_24do3_xpm, rev_24do0_xpm,
    rev_24c3_xpm, rev_24c0_xpm, rev_24c3_xpm, rev_24c0_xpm,
  };
  static char **basepais2[] = {
    base_24o3c_xpm, base_24o0c_xpm, base_24o3c_xpm, base_24o0c_xpm,
    base_24o3c_xpm, base_24o0c_xpm, base_24o3c_xpm, base_24o0c_xpm,
    base_24do3c_xpm, base_24do0c_xpm, base_24do3c_xpm, base_24do0c_xpm,
    base_24o3c_xpm, base_24o0c_xpm, base_24o3c_xpm, base_24o0c_xpm,
  };
  static char **revpais2[] = {
    rev_24o3c_xpm, rev_24o0c_xpm, rev_24o3c_xpm, rev_24o0c_xpm,
    rev_24o3c_xpm, rev_24c0c_xpm, rev_24o3c_xpm, rev_24c2c_xpm,
    rev_24do3c_xpm, rev_24do0c_xpm, rev_24do3c_xpm, rev_24do0c_xpm,
    rev_24o3c_xpm, rev_24o0c_xpm, rev_24o3c_xpm, rev_24o0c_xpm,
  };
  char **pais;
  char ***basepais, ***revpais;

  if (PAICARD3D)
    {
      basepais = basepais1;
      revpais = revpais1;
    }
  else
    {
      basepais = basepais0;
      revpais = revpais0;
    }

  if (kind == 0)
    {
      pais = revpais[type];
    }
  else if (kind == 38)
    {
      pais = revpais2[type];
    }
  else if (kind == 39)
    {
      pais = basepais2[type];
    }
  else
    {
      pais = basepais[type];
    }

  cnt = 0;
  sscanf (pais[0], "%d %d %d %d", &x, &y, &c, &d);
  sprintf (paibase[cnt++], "%d %d %d %d", x, y, c + N_PAICOLOR, d);
  for (i = 1; i <= c; i++)
    {
      strcpy (paibase[cnt++], pais[i]);
    }
  for (j = 0; j < N_PAICOLOR; j++)
    {
      strcpy (paibase[cnt++], paicolor[j]);
    }
  if (PAICARD3D)
    {
      if (type & 4)
        base_y = cnt + 9;
      else
        base_y = cnt;
      if (type & 1)
        base_x = 1;
      else
        base_x = 2;
    }
  else
    {                           /* PAICARD */
      if (type & 1)
        {
          base_x = 1;
          base_y = cnt + 2;
        }
      else
        {
          base_x = 2;
          base_y = cnt + 1;
        }
    }

  for (; i <= c + y; i++)
    {
      strcpy (paibase[cnt++], pais[i]);
    }
#ifdef HUGEPAI
#define PAIDATA_W  37
#define PAIDATA_H  54
#define PAIDATALASTCHAR 'H'
#else
#define PAIDATA_W  21
#define PAIDATA_H  29
#define PAIDATALASTCHAR 'D'
#endif
  if (0 < kind && kind < 38)
    for (y = 0; y < PAIDATA_H; y++)
      for (x = 0; x < PAIDATA_W; x++)
        {
          c = paidata[(kind - 1) * PAIDATA_H + y][x];
          if ('A' <= c && c <= PAIDATALASTCHAR)
            {
              switch (type & 3)
                {
                case 0:
                  paibase[base_y + y][base_x + x] = c;
                  break;
                case 1:
                  paibase[base_y + (PAIDATA_W - 1 - x)][base_x + y] = c;
                  break;
                case 2:
                  paibase[base_y + (PAIDATA_H - 1 - y)][base_x +
                                                        (PAIDATA_W - 1 - x)] =
                    c;
                  break;
                case 3:
                  paibase[base_y + x][base_x + (PAIDATA_H - 1 - y)] = c;
                  break;
                }
            }
        }
  for (j = 0; j < cnt; j++)
    {
      paibuf[j] = paibase[j];
    }
  paibuf[j] = 0;
  return paibuf;
}

static char **
rotpai (char **pai, int mode)
{
  static char *paibuf[TMP_H];
  static char paibase[TMP_H][TMP_W];
  int xs, ys, c, d;
  int i, j, x, y;
  int cnt = 0;

  sscanf (pai[0], "%d %d %d %d", &xs, &ys, &c, &d);
  sprintf (paibase[cnt++], "%d %d %d %d", ys, xs, c, d);
  for (i = 1; i <= c; i++)
    {
      strcpy (paibase[cnt++], pai[i]);
    }
  for (x = 0; x < xs; x++)
    {
      for (y = 0; y < ys; y++)
        {
          c = pai[cnt + y][x];
          if (mode == 0)
            {
              paibase[cnt + (xs - 1 - x)][y] = c;
            }
          else
            {
              paibase[cnt + x][ys - 1 - y] = c;
            }
        }
      paibase[cnt + x][ys] = 0;
    }
  for (i = 0; i < cnt + xs; i++)
    {
      paibuf[i] = paibase[i];
    }
  paibuf[i] = 0;
  return paibuf;
}

static char **
cutpai (char **pai, int n, int shrink)
{
  static char *paibuf[TMP_H];
  static char paibase[TMP_H][TMP_W];
  int xs, ys, c, d;
  int i, j, x, y;
  int cnt = 0;
  int off;

  sscanf (pai[0], "%d %d %d %d", &xs, &ys, &c, &d);
  ys -= n;
  if (shrink)
    {
      off = n;
    }
  else
    {
      off = 0;
    }
  sprintf (paibase[cnt++], "%d %d %d %d", xs, ys, c, d);
  for (i = 1; i <= c; i++)
    {
      strcpy (paibase[cnt++], pai[i]);
    }
  for (x = 0; x < xs; x++)
    {
      for (y = 0; y < ys; y++)
        {
          c = pai[cnt + off + y][x];
          paibase[cnt + y][x] = c;
        }
    }
  for (i = 0; i < cnt + ys; i++)
    {
      paibuf[i] = paibase[i];
    }
  paibuf[i] = 0;
  return paibuf;
}

static int mode_noaka = 0;

void
pixpai_mode (int noaka)
{
  mode_noaka = noaka;
}

static struct gpixmap2 pm_pai[3][16][40];

void
pai2pix_init ()
{
}

#define MAN5R  (5 * 4)
#define PIN5R  (14 * 4)
#define SOU5R  (23 * 4)

gpixmap2_t
pai2pix (integer_t n, integer_t pos, integer_t reach, integer_t opened)
{
  int type;
  int kind;
  char **p;

  kind = n / 4;
  if (!mode_noaka)
    {
      if (n == MAN5R)
        kind = 35;
      if (n == PIN5R)
        kind = 36;
      if (n == SOU5R)
        kind = 37;
    }
  if (n == 256)
    kind = 38;
  if (n > 256)
    kind = 39;

  type = 0;
  if (BUILD4 && PAICARD)
    {
      type = (pos + 1 + (reach != 0)) % 4;
    }
  else if (BUILD4 && PAICARD3D)
    {
      type = (pos + 1 + (reach != 0)) % 4;
    }
  else
    {
      if (reach)
        type |= 1;
    }
  if (!(opened & PAIOPEN))
    type |= 4;
  if (opened & PAIDARK)
    type |= 8;

  if (pos == 3)
    pos = 1;
  if (!pm_pai[pos][type][kind].pm)
    {
      p = makepai (type, kind);
      if (BUILD4 && PAICARD3D && (opened & PAIOPEN))
        {
          if (kind < 38)
            {
              if (pos == 0)
                {
                  p = cutpai (p, 7, !(opened & PAIOPEN));
                }
              else if (pos == 2)
                {
                  p = cutpai (p, 7, !(opened & PAIOPEN));
                }
            }
          else
            {
              if (pos == 0)
                {
                  p = cutpai (p, 2, 1);
                }
              else if (pos == 2)
                {
                  p = cutpai (p, 2, 1);
                }
            }
        }
      read_pix_idx (p, pm_pai[pos][type][kind]);
    }
  return &pm_pai[pos][type][kind];
}

static struct gpixmap2 pm_attr[4][2][2];

gpixmap2_t
attr2pix2 (int n, int pos, int reach)
{
  char *paibuf[TMP_H];
  char paibase[TMP_H][TMP_W];
#ifdef HUGEPAI
  static char *attrn =
    "                                                                               ";
  static char *attrtg =
    "   xxxxxx                                                                      ";
#else
  static char *attrn = "                                         ";
  static char *attrtg = "   xxxxxx                                ";
#endif
  char *p;
  int w, c, i;
  int rot;
  int cnt;

  if (!BUILD4)
    pos = 3;
  if (pm_attr[pos][reach][n].pm)
    {
      return &pm_attr[pos][reach][n];
    }
  if (reach)
    {
      w = PAI_W2;
    }
  else
    {
      w = PAI_W;
    }
  if (n == 0)
    p = attrn;
  else
    p = attrtg;

  cnt = 1;
  strcpy (paibase[cnt++], "       s None  c None");
  strcpy (paibase[cnt++], ".      c black");
  strcpy (paibase[cnt++], "x      c red");

  rot = (pos + 1) % 4;
  for (i = 0; i < w; i++)
    {
      c = p[i];
      switch (rot)
        {
        case 0:
          paibase[cnt][i] = c;
          paibase[cnt + 1][i] = c;
          sprintf (paibase[0], "%d %d %d %d", w, 1, 3, 1);
          break;
        case 1:
          paibase[cnt + w - 1 - i][0] = c;
          paibase[cnt + w - 1 - i][1] = c;
          paibase[cnt + w - 1 - i][2] = 0;
          sprintf (paibase[0], "%d %d %d %d", 1, w, 3, 1);
          break;
        case 2:
          paibase[cnt][w - 1 - i] = c;
          paibase[cnt + 1][w - 1 - i] = c;
          sprintf (paibase[0], "%d %d %d %d", w, 1, 3, 1);
          break;
        case 3:
          paibase[cnt + i][0] = c;
          paibase[cnt + i][1] = c;
          paibase[cnt + i][2] = 0;
          sprintf (paibase[0], "%d %d %d %d", 1, w, 3, 1);
          break;
        }
    }
  switch (rot)
    {
    case 0:
      paibase[cnt][w] = 0;
      sprintf (paibase[0], "%d %d %d %d", w, 2, 3, 1);
      cnt += 2;
      break;
    case 1:
      sprintf (paibase[0], "%d %d %d %d", 2, w, 3, 1);
      cnt += w;
      break;
    case 2:
      paibase[cnt][w] = 0;
      sprintf (paibase[0], "%d %d %d %d", w, 2, 3, 1);
      cnt += 2;
      break;
    case 3:
      sprintf (paibase[0], "%d %d %d %d", 2, w, 3, 1);
      cnt += w;
      break;
    }
  for (i = 0; i < cnt; i++)
    {
      paibuf[i] = paibase[i];
    }
  read_pix_idx (paibuf, pm_attr[pos][reach][n]);
  return &pm_attr[pos][reach][n];
}
