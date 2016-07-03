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
 * 7/7/2006 Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"
#include "ruledef.h"

void
hand_new (global_t *gp)
{
  int i, j, k;

  for (i = 0; i < 4; i++)
    {
      hand[i].reach = 0;
      hand[i].naki = 0;
      hand[i].closed_num = 13;
      for (j = 0; j < 14; j++)
        hand[i].closed[j] = 0;
      for (j = 0; j < 4; j++)
        {
          hand[i].opened_kind[j] = 0;
          for (k = 0; k < 4; k++)
            hand[i].opened[j][k] = 0;
        }
    }
}

void
hand_sort (global_t *gp, int p)
{
  pai_t *q;
  int i, j, n;
  pai_t tmp;

  q = hand[p].closed;
  n = hand[p].closed_num;
  for (i = 0; i < n; i++)
    for (j = i + 1; j < n; j++)
      if (q[i] > q[j])
        {
          tmp = q[i];
          q[i] = q[j];
          q[j] = tmp;
        }
}

void
hand_insert (global_t *gp, int p, int c)
{
  hand_t *h;
  h = &(hand[p]);
  h->closed[h->closed_num++] = c;
}

void
hand_delete (global_t *gp, int p, pai_t c, int sort)
{
  hand_t *h;
  int i, n;

  h = &(hand[p]);
  n = h->closed_num;

  for (i = 0; i < n; i++)
    {
      if (c == h->closed[i])
        {
          h->closed[i] = h->closed[n - 1];
          h->closed[n - 1] = 0;
          h->closed_num--;
          break;
        }
    }
  if (sort)
    hand_sort (gp, p);
}

int
hand_delete_kind (global_t *gp, int p, pai_t c, int sort)
{
  hand_t *h;
  int i, j, n;
  pai_t tmp;
  int ret = 0;

  h = &(hand[p]);
  n = h->closed_num;

  for (i = 0; i < n; i++)
    {
      if (c == P_KIND (h->closed[i]))
        {
          ret = h->closed[i];
          h->closed[i] = h->closed[n - 1];
          break;
        }
    }
  if (ret)
    {
      h->closed[n - 1] = 0;
      h->closed_num--;
    }
  if (ret && sort)
    hand_sort (gp, p);
  return ret;
}

int
hand_find (global_t *gp, int p, pai_t pc)
{
  hand_t *h;
  int i, j, n;
  pai_t tmp;
  int ret = 0;

  h = &(hand[p]);
  n = h->closed_num;

  for (i = 0; i < n; i++)
    {
      if (pc == h->closed[i])
        {
          ret++;
        }
    }
  return ret;
}

int
hand_find_kind (global_t *gp, int p, pai_t c)
{
  hand_t *h;
  int i, j, n;
  pai_t tmp;
  int ret = 0;

  h = &(hand[p]);
  n = h->closed_num;

  for (i = 0; i < n; i++)
    {
      if (c == P_KIND (h->closed[i]))
        {
          ret++;
        }
    }
  return ret;
}

int
hand_tie (global_t *gp, int p, int pc, int how)
{
  pai_t a, b, k, hk;
  int i;
  int ret = 0;

  hk = P_KIND (how);
  k = P_KIND (pc);

  a = hand_delete_kind (gp, p, hk, 1);
  if (k + 1 == hk)
    {                           /* pc how ... */
      b = hand_delete_kind (gp, p, hk + 1, 1);
    }
  else if (hk + 1 == k)
    {                           /* how pc ... */
      b = hand_delete_kind (gp, p, k + 1, 1);
    }
  else
    {                           /* how ... pc */
      b = hand_delete_kind (gp, p, hk + 1, 1);
    }

  for (i = 0; i < 4; i++)
    {
      if (!hand[p].opened_kind[i])
        {
          hand[p].opened_kind[i] = H_TIE;
          hand[p].opened[i][0] = pc;
          hand[p].opened[i][1] = a;
          hand[p].opened[i][2] = b;
          hand[p].opened_from[i] = 0;
          hand[p].naki = 1;
          ret = i;
          break;
        }
    }
  return ret;
}

int
hand_pon (global_t *gp, int p, int pc, int from)
{
  pai_t a, b, k;
  int i;
  int ret = 0;

  k = P_KIND (pc);
  a = hand_delete_kind (gp, p, k, 1);
  b = hand_delete_kind (gp, p, k, 1);
  for (i = 0; i < 4; i++)
    {
      if (!hand[p].opened_kind[i])
        {
          hand[p].opened_kind[i] = H_PON;
          hand[p].opened[i][0] = a;
          hand[p].opened[i][1] = b;
          hand[p].opened[i][2] = pc;
          hand[p].opened_from[i] = from;
          ret = i;
          break;
        }
    }
  hand[p].naki = 1;
  return ret;
}

int
hand_kan (global_t *gp, int p, int pc, int from)
{
  pai_t a, b, c, d, k;
  int i;
  int ret = 0;

  k = P_KIND (pc);
  a = hand_delete_kind (gp, p, k, 1);
  if (b = hand_delete_kind (gp, p, k, 1))
    {
      c = hand_delete_kind (gp, p, k, 1);
      d = hand_delete_kind (gp, p, k, 1);
      for (i = 0; i < 4; i++)
        {
          if (!hand[p].opened_kind[i])
            {
              hand[p].opened_kind[i] = d ? H_KAN_CLOSED : H_KAN_OPENED;
              hand[p].opened[i][0] = a;
              hand[p].opened[i][1] = b;
              hand[p].opened[i][2] = c;
              hand[p].opened[i][3] = d ? d : pc;
              hand[p].opened_from[i] = from;
              if (!d)
                hand[p].naki = 1;
              ret = i;
              break;
            }
        }
    }
  else
    {
      for (i = 0; i < 4; i++)
        {
          if (hand[p].opened_kind[i] == H_PON &&
              P_KIND (hand[p].opened[i][0]) == k)
            {
              hand[p].opened[i][3] = pc;
              hand[p].opened_kind[i] = H_KAN_OPENED;
              ret = i;
              break;
            }
        }
    }
  return ret;
}

int
hand_can_tie (global_t *gp, int p, int pc, int how)
{
  pai_t a, b, k, hk;

  if (hand[p].reach)
    return 0;
  hk = P_KIND (how);
  k = P_KIND (pc);
  if (!(IS_MAN (k) && IS_MAN (hk))
      && !(IS_PIN (k) && IS_PIN (hk)) && !(IS_SOU (k) && IS_SOU (hk)))
    return 0;

  if ((hk == K_MAN_8 && k == K_MAN_9)
      || (hk == K_PIN_8 && k == K_PIN_9)
      || (hk == K_SOU_8 && k == K_SOU_9)
      || (hk == K_MAN_9 && k == K_MAN_8)
      || (hk == K_PIN_9 && k == K_PIN_8) || (hk == K_SOU_9 && k == K_SOU_8))
    return 0;

  a = hand_find_kind (gp, p, hk);
  if (k + 1 == hk)
    {                           /* pc how ... */
      b = hand_find_kind (gp, p, hk + 1);
    }
  else if (hk + 1 == k)
    {                           /* how pc ... */
      b = hand_find_kind (gp, p, k + 1);
    }
  else if (hk + 2 == k)
    {                           /* how ... pc */
      b = hand_find_kind (gp, p, hk + 1);
    }
  else
    {
      return 0;
    }
  if (a == 0 || b == 0)
    return 0;
  return 1;
}

int
hand_can_tiex (global_t *gp, int p, int pc)
{
  return
    hand_can_tie (gp, p, pc, pc - (2 * 4))
    || hand_can_tie (gp, p, pc, pc - (1 * 4))
    || hand_can_tie (gp, p, pc, pc + (1 * 4));
}

int
hand_can_pon (global_t *gp, int p, int pc)
{
  if (hand[p].reach)
    return 0;
  return (hand_find_kind (gp, p, P_KIND (pc)) >= 2);
}

/* 1: kakan 2: minkan 3: ankan */
int
hand_can_kan (global_t *gp, int p, int pc)
{
  int same, i;

  same = hand_find_kind (gp, p, P_KIND (pc));
  if (same == 3 && hand[p].reach)
    return 0;
  if (same >= 3)
    return same - 1;

  if (hand[p].reach)
    return 0;
  for (i = 0; i < 4; i++)
    {
      if (hand[p].opened_kind[i] == H_PON
          && P_KIND (hand[p].opened[i][0]) == P_KIND (pc))
        return 1;
    }
  return 0;
}

int
hand_can_kanx (global_t *gp, int p)
{
  int i, n, x;
  pai_t *h;

  n = hand[p].closed_num;
  h = hand[p].closed;

  for (i = 0; i < n; i++, h++)
    {
      x = hand_can_kan (gp, p, *h);
      if (x == 1 || x == 3)
        return *h;
    }
  return 0;
}

int
hand_check_kyushukyupai (global_t *gp, int p)
{
  int i, k;
  int kinds[K_MAX];

  for (i = 0; i < K_MAX; i++)
    kinds[i] = 0;

  for (i = 0; i < hand[p].closed_num; i++)
    {
      kinds[P_KIND (hand[p].closed[i])]++;
    }
  k = 0;
  for (i = 0; i < K_MAX; i++)
    {
      if (kinds[i] && IS_YAO (i))
        k++;
    }
  return (k >= 9) ? (k - 8) : 0;
}

int
hand_check_kuikae (global_t *gp, int p, int pc)
{
  pai_t a, b, c, d;
  pai_t x, y;
  int i, j, k;

  if (!RL_NOKUIKAE)
    return 0;

  x = y = 0;
  for (j = 0; j < 4; j++)
    {
      k = hand[p].opened_kind[j];
      if (!k)
        break;
    }
  j--;
  if (hand[p].opened_kind[j] == H_TIE)
    {
      a = P_KIND (hand[p].opened[j][0]);
      b = P_KIND (hand[p].opened[j][1]);
      c = P_KIND (hand[p].opened[j][2]);
      x = a;
      if (b + 1 == c)
        {
          if (K_NEXT (a) == b && K_NEXT (c) == c + 1)
            y = a + 3;
          if (K_NEXT (c) == a && K_PREV (b) == b - 1)
            y = a - 3;
        }
    }
  else if (hand[p].opened_kind[j] == H_PON)
    {
      x = P_KIND (hand[p].opened[j][0]);
    }
  if (P_KIND (pc) != x && P_KIND (pc) != y)
    {
      return 0;
    }
  for (i = 0; i < hand[p].closed_num; i++)
    {
      pc = hand[p].closed[i];
      if (P_KIND (pc) != x && P_KIND (pc) != y)
        {
          return pc;
        }
    }
  return 0;
}

int
hand_check_pao (global_t *gp, int p)
{
  int haku, hatu, cyun, ton, nan, sya, pei;
  int i, k;

  if (!RL_PAO)
    return -1;
  haku = hatu = cyun = 0;
  ton = nan = sya = pei = 0;
  for (i = 0; i < 4; i++)
    {
      k = hand[p].opened_kind[i];
      if (k == H_PON || k == H_KAN_OPENED || k == H_KAN_CLOSED)
        {
          switch (P_KIND (hand[p].opened[i][0]))
            {
            case K_TON:
              ton = 1;
              break;
            case K_NAN:
              nan = 1;
              break;
            case K_SHA:
              sya = 1;
              break;
            case K_PEI:
              pei = 1;
              break;
            case K_HAKU:
              haku = 1;
              break;
            case K_HATU:
              hatu = 1;
              break;
            case K_CHUN:
              cyun = 1;
              break;
            }
          if (ton + nan + sya + pei == 4)
            {
              if (k != H_KAN_CLOSED)
                return hand[p].opened_from[i];
              return -1;
            }
          if (haku + hatu + cyun == 3)
            {
              if (k != H_KAN_CLOSED)
                return hand[p].opened_from[i];
              return -1;
            }
        }
      if (!k)
        break;
    }
  return -1;
}
