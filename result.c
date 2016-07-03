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

#include "pai.h"
#include "ahand.h"
#include "global.h"
#include "result.h"
#include "ruledef.h"

#undef MIN
#define MIN(x,y)	((x)<(y)?(x):(y))

static int fu_calc_head (global_t *gp, int pk, int p);
static int find_kind (int kind, int *buf, int n);
static void result_clear_genyakus (result_t *yp);

void
result_new (global_t *gp)
{
  int i;
  for (i = 0; i < 5; i++)
    bzero (result + i, sizeof (result_t));
}

int
result_check_sufurenta (global_t *gp)
{
  if (cur_player == 3 && rvp == &rv[3][0]
      && (P_KIND (rvp->out) >= K_TON) && (P_KIND (rvp->out) <= K_PEI)
      && (P_KIND (rvp->out) == P_KIND (rv[0][0].out))
      && (P_KIND (rvp->out) == P_KIND (rv[1][0].out))
      && (P_KIND (rvp->out) == P_KIND (rv[2][0].out))
      && (T_GET (rv[0][0].attr) == T_RV || T_GET (rv[0][0].attr) == T_RV_RCH)
      && (T_GET (rv[1][0].attr) == T_RV || T_GET (rv[1][0].attr) == T_RV_RCH)
      && (T_GET (rv[2][0].attr) == T_RV || T_GET (rv[2][0].attr) == T_RV_RCH)
      && (T_GET (rv[3][0].attr) == T_RV || T_GET (rv[3][0].attr) == T_RV_RCH)
      && R_GET (rv[0][0].attr) == R_NUL
      && R_GET (rv[1][0].attr) == R_NUL
      && R_GET (rv[2][0].attr) == R_NUL && R_GET (rv[3][0].attr) == R_NUL)
    return 1;
  return 0;
}

int
result_check_nagasimangan (global_t *gp, int p)
{
  int i;
  for (i = 0; i <= rv_cur[p]; i++)
    {
      if (rv[p][i].out)
        {
          if (!IS_YAO (P_KIND (rv[p][i].out))
              || T_GET (rv[p][i].attr) != T_RV
              || R_GET (rv[p][i].attr) != R_NUL)
            return 0;
        }
    }
  return 1;
}

void
result_calc_nagasimangan (global_t *gp, int p, result_t *yp)
{
  yp->flag = RESULT_AGARI_TUMO;
  yp->nagasimangan = 1;
  if (p == 0)
    {
      yp->points[0] = 12000;
      yp->points[1] = -4000;
      yp->points[2] = -4000;
      yp->points[3] = -4000;
    }
  else
    {
      yp->points[0] = -4000;
      yp->points[1] = -2000;
      yp->points[2] = -2000;
      yp->points[3] = -2000;
      yp->points[p] = 8000;
    }
}

int
result_check_kyushukyupai (global_t *gp)
{
  int i;

  if (rv_cur[cur_player] != 0)
    return 0;
  for (i = 0; i < cur_player; i++)
    {
      if ((T_GET (rv[i][0].attr) != T_RV) || R_GET (rv[i][0].attr) != R_NUL)
        return 0;
    }
  return hand_check_kyushukyupai (gp, cur_player);
}

int
result_check_sukan (global_t *gp)
{
  int i, j, n, kn, knmax, x;
  if (mt_top_cnt < 4)
    return 0;
  knmax = 0;
  for (i = 0; i < 4; i++)
    {
      kn = 0;
      for (j = 0; j < 4; j++)
        {
          x = hand[i].opened_kind[j];
          if (x == H_KAN_OPENED || x == H_KAN_CLOSED)
            kn++;
        }
      if (kn > knmax)
        knmax = kn;
    }
  return (knmax == 4) ? 0 : 1;
}

void
result_calc (global_t *gp, int mode, int *pp, int pp_num)
{
  int i, ac, p;
  result_t yr[4];
  int st[4];

  switch (mode)
    {
    case RESULT_NULL:
      break;
    case RESULT_AGARI_TUMO:
      if (result_calc_yaku (gp, pp[0], yr) > 0)
        {
          if (yr[0].pao < 0)
            {
              yr[0].flag = mode;
              result[0] = yr[0];
              result[0].who = *pp;
              calc_tumibo (gp, &result[0], -1);
            }
          else
            {
              yr[0].flag = RESULT_AGARI_RON;
              result[0] = yr[0];
              result[0].who = *pp;
              calc_tumibo (gp, &result[0], yr[0].pao);
            }
        }
      else if (result_check_kyushukyupai (gp))
        {
          result[0].flag = RESULT_RYUKYOKU;
          result[0].kyusukyupai = 1;
          result[0].who = *pp;
        }
      else
        {
          result_set_cyonbo (gp, *pp, 0);
        }
      break;
    case RESULT_AGARI_RON:
      ac = 0;
      for (i = 0; i < pp_num; i++)
        {
          st[i] = result_calc_yaku (gp, pp[i], yr + i);
          if (st[i] > 0)
            ac++;
        }
      if (ac == 3 && !RL_NOSANCYAHO)
        {
          result[0].flag = RESULT_RYUKYOKU;
          result[0].sancyahou = 1;
        }
      else if (ac > 0)
        {
          for (i = 0, p = 0; i < pp_num; i++)
            {
              yr[i].flag = mode;
              if (st[i] > 0)
                {
                  result[p] = yr[i];
                  if (p == 0)
                    calc_tumibo (gp, &result[0], cur_player);
                  result[p++].who = pp[i];
                  if (RL_ATAMAHANE)
                    break;
                }
            }
        }
      else
        {
          for (i = 0, p = 0; i < pp_num; i++)
            {
              result[i] = yr[i];
              result_set_cyonbo (gp, pp[i], i);
            }
        }
      break;
    case RESULT_RYUKYOKU:
      if (result_check_sufurenta (gp))
        {
          result->flag = RESULT_RYUKYOKU;
          result->sufurenta = 1;
          return;
        }
      else if (result_check_sukan (gp))
        {
          result->flag = RESULT_RYUKYOKU;
          result->sukannagare = 1;
          return;
        }
      i = 0;
      for (p = 0; p < 4; p++)
        {
          if (result_check_nagasimangan (gp, p))
            {
              result_calc_nagasimangan (gp, p, result + i);
              i++;
            }
        }
      if (i > 0)
        return;
      result_calc_ryukyoku (gp, result);
      result->who = -1;
      break;
    }
}

void
calc_tumibo (global_t *gp, result_t *yr, int furi)
{
  int i;
  if (furi >= 0)
    yr->points[furi] -= 300 * small_turn_cnt;
  for (i = 0; i < 4; i++)
    {
      if (yr->points[i] > 0)
        {
          yr->points[i] += 300 * small_turn_cnt;
        }
      else if (yr->flag == RESULT_AGARI_TUMO)
        {
          yr->points[i] -= 100 * small_turn_cnt;
        }
    }
}

void
result_calc_ryukyoku (global_t *gp, result_t *yr)
{
  int i, n;
  int tenpai[4];
  int tenpai_cnt = 0;
  ahand_t at;

  for (i = 0; i < 4; i++)
    {
      ah_new (&at, &hand[i], -1);
      ah_analize (&at, 0);
      tenpai[i] = (result_rest <= 1);
      if (tenpai[i])
        tenpai_cnt++;
    }
  n = 0;
  for (i = 0; i < 4; i++)
    {
      if (hand[i].reach && !tenpai[i])
        {
          result[n].noten_reach = 1;
          result_set_cyonbo (gp, i, n);
          n++;
        }
    }
  if (n)
    return;

  switch (tenpai_cnt)
    {
    case 1:
      for (i = 0; i < 4; i++)
        {
          if (tenpai[i])
            yr->points[i] = 3000;
          else
            yr->points[i] = -1000;
        }
      break;
    case 2:
      for (i = 0; i < 4; i++)
        {
          if (tenpai[i])
            yr->points[i] = 1500;
          else
            yr->points[i] = -1500;
        }
      break;
    case 3:
      for (i = 0; i < 4; i++)
        {
          if (tenpai[i])
            yr->points[i] = 1000;
          else
            yr->points[i] = -3000;
        }
    }
  if (tenpai[0])
    yr->flag = RESULT_RYUKYOKU_CONT;
  else
    yr->flag = RESULT_RYUKYOKU;
}

int
result_calc_yaku (global_t *gp, int p, result_t *yr)
{
  int i;
  int max = 0, fan = 0, fu = 0;
  int ret;
  result_t y;
  ahand_t at;

  bzero (yr, sizeof (result_t));
  ah_new (&at, &hand[p], -1);
  ah_analize (&at, 0);

  if (result_rest != 0)
    return 0;
  if (result_check_furiten (gp, p, yr))
    return 0;

  ah_new (&at, &hand[p], hand[p].closed_num - 1);
  ah_analize (&at, 0);

  for (i = 0; i < result_cnt; i++)
    {
      bzero (&y, sizeof (result_t));
      if (!result_calc_from_ahand (gp, p, &y, &(result_ahand[i])))
        continue;
      result_calc_from_hand (gp, p, &y);
      ret = result_calc_point (gp, p, &y);
      if (ret > max
          || (ret == max && y.fan > fan)
          || (ret == max && y.fan == fan && y.fu > fu))
        {
          max = ret;
          fan = y.fan;
          fu = y.fu;
          *yr = y;
        }
    }
  return max;
}

int
result_check_furiten (global_t *gp, int p, result_t *yp)
{
  river_t *rp;
  pai_t save, *last;
  ahand_t at;
  int reach = -1;
  int i;

  last = hand[p].closed + hand[p].closed_num - 1;
  save = *last;

  if (p == cur_player)
    return 0;
  for (i = 0; i <= rv_cur[p] && rv[p][i].out; i++)
    {
      if (T_GET (rv[p][i].attr) != T_HN_KAN_C)
        {
          *last = rv[p][i].out;
          ah_new (&at, &hand[p], -1);
          ah_analize (&at, 0);
          if (result_rest == 0)
            {
              yp->furiten = 1;
              goto bad;
            }
        }
      if (T_GET (rv[p][i].attr) == T_RV_RCH)
        reach = i;
    }
  if (reach > 0)
    {
      rp = &(rv[p][reach]);
      while (rp->next)
        {
          if (T_GET (rp->attr) != T_HN_KAN_C)
            {
              *last = rp->out;
              ah_new (&at, &hand[p], -1);
              ah_analize (&at, 0);
              if (result_rest == 0)
                {
                  yp->furiten = 1;
                  goto bad;
                }
            }
          rp = rp->next;
        }
    }
  rp = rvp->prev;
  while (rp && rv_find_player (gp, rp) != p)
    {
      if (T_GET (rp->attr) != T_HN_KAN_C)
        {
          *last = rp->out;
          ah_new (&at, &hand[p], -1);
          ah_analize (&at, 0);
          if (result_rest == 0)
            {
              yp->doujun = 1;
              goto bad;
            }
        }
      rp = rp->prev;
    }

  *last = save;
  return 0;
bad:
  *last = save;
  return 1;
}

int
result_calc_from_ahand (global_t *gp, int p, result_t *yp, ahand_t *ap)
{
  int i, tmp;
  int fu = 20;
  int tumo = 0;
  int pai, pk, kind;
  int koutu = 0;
  int shuntu = 0;
  int kantu = 0;
  int ankou = 0;
  int naki;
  int head = 0;
  int v2_num;
  int chanta;
  int junchan;
  int koutu_kind[4];
  int shuntu_kind[4];
  int match;

  naki = hand[p].naki;
  pai = hand[p].closed[hand[p].closed_num - 1];
  pk = P_KIND (pai);

  if (ap->part_num == 0)
    {
      return 2;
    }
  if (T_GET (rvp->attr) == T_HN_KAN_C)
    return 0;
  if (ap->part_num == 6)
    {
      for (i = 0; i < 6; i++)
        {
          if (ap->part_kind[i] != AH_V2)
            break;
        }
      if (i == 6)
        {
          yp->chitoitu = 1;
          yp->fu = 25;
          if (pk != ap->rest[0])
            return 0;
          return 1;
        }
    }

  if (cur_player == p)
    {
      tumo = 1;
      fu += 2;
    }
  if (!naki && !tumo)
    fu += 10;

  v2_num = 0;
  match = 0;
  for (i = 0; i < ap->part_num; i++)
    {
      if (AH_V2 == ap->part_kind[i])
        {
          v2_num++;
          if (pk == ap->part[i][0])
            match++;
        }
    }
  if (v2_num == 0)
    {                           /* tanki machi */
      fu += 2;
      head = pk;
      if (pk != ap->rest[0])
        return 0;
    }
  if (v2_num == 2 && !match)
    {
      return 0;
    }
  if (cur_player == p && v2_num == 2)
    {
      ankou++;
    }
  for (i = 0; i < ap->part_num; i++)
    {
      kind = ap->part_kind[i];
      switch (kind)
        {
        case AH_V3_FIXED:
        case AH_H3_FIXED:
          break;
        case AH_V3:
          fu += fu_calc_koutu (ap->part[i][0], 1);
          ankou++;
          koutu_kind[koutu++] = ap->part[i][0];
          break;
        case AH_H3:
          shuntu_kind[shuntu++] = ap->part[i][0];
          break;
        case AH_V2:
          if (v2_num == 2 && pk == ap->part[i][0])
            {
              fu += fu_calc_koutu (ap->part[i][0], tumo);
              koutu_kind[koutu++] = ap->part[i][0];
            }
          else
            {
              head = ap->part[i][0];
            }
          break;
        case AH_H2c:
          if (pk != K_NEXT2 (ap->part[i][0]))
            return 0;
          fu += 2;
          shuntu_kind[shuntu++] = MIN (ap->part[i][0], pk);
          break;
        case AH_H2b:
          fu += 2;
        case AH_H2a:
          if (pk != K_PREV2 (ap->part[i][0])
              && pk != K_NEXT2 (ap->part[i][1]))
            return 0;
          shuntu_kind[shuntu++] = MIN (ap->part[i][0], pk);
          break;
        }
    }
  fu += fu_calc_head (gp, head, p);
  for (i = 0; i < 4; i++)
    if (kind = hand[p].opened_kind[i])
      {
        switch (kind)
          {
          case H_TIE:
            tmp = MIN (hand[p].opened[i][0], hand[p].opened[i][1]);
            tmp = MIN (tmp, hand[p].opened[i][2]);
            shuntu_kind[shuntu++] = P_KIND (tmp);
            break;
          case H_PON:
            fu += fu_calc_koutu (P_KIND (hand[p].opened[i][0]), 0);
            koutu_kind[koutu++] = P_KIND (hand[p].opened[i][0]);
            break;
          case H_KAN_OPENED:
            fu += fu_calc_koutu (P_KIND (hand[p].opened[i][0]), 2);
            kantu++;
            koutu_kind[koutu++] = P_KIND (hand[p].opened[i][0]);
            break;
          case H_KAN_CLOSED:
            fu += fu_calc_koutu (P_KIND (hand[p].opened[i][0]), 3);
            ankou++;
            kantu++;
            koutu_kind[koutu++] = P_KIND (hand[p].opened[i][0]);
            break;
          }
      }
  if (!naki && !tumo && (fu == 30))
    {
      yp->pinfu = 1;
    }
  else if (!naki && tumo && (fu == 22) && !RL_NOPINTUMO)
    {
      fu = 20;
      yp->pinfu = 1;
    }
  else if (fu == 20)
    {
      fu = 30;
    }
  yp->fu = ((fu + 9) / 10) * 10;

  if (ankou == 4)
    yp->suankou = 1;
  if (ankou == 3)
    yp->sanankou = 1;
  if (kantu == 4)
    yp->sukantu = 1;
  if (kantu == 3)
    yp->sankantu = 1;
  if (koutu == 4)
    yp->toitoi = 1;

  if (shuntu >= 3)
    for (i = 0; i < 3; i++)
      {
        if (find_kind (K_MAN_1 + i * 9, shuntu_kind, shuntu)
            && find_kind (K_MAN_4 + i * 9, shuntu_kind, shuntu)
            && find_kind (K_MAN_7 + i * 9, shuntu_kind, shuntu))
          {
            if (naki)
              yp->ittu_naki = 1;
            else
              yp->ittu = 1;
          }
      }
  if (shuntu >= 3)
    for (i = 0; i < 7; i++)
      {
        if (find_kind (K_MAN_1 + i, shuntu_kind, shuntu)
            && find_kind (K_PIN_1 + i, shuntu_kind, shuntu)
            && find_kind (K_SOU_1 + i, shuntu_kind, shuntu))
          {
            if (naki)
              yp->sansiki_naki = 1;
            else
              yp->sansiki = 1;
          }
      }
  if (koutu >= 3)
    for (i = 0; i < 9; i++)
      {
        if (find_kind (K_MAN_1 + i, koutu_kind, koutu)
            && find_kind (K_PIN_1 + i, koutu_kind, koutu)
            && find_kind (K_SOU_1 + i, koutu_kind, koutu))
          yp->sansiki_doukoku = 1;
      }
  if ((shuntu == 4) && !naki)
    {
      if (((shuntu_kind[0] == shuntu_kind[1])
           && (shuntu_kind[2] == shuntu_kind[3]))
          || ((shuntu_kind[0] == shuntu_kind[2])
              && (shuntu_kind[1] == shuntu_kind[3]))
          || ((shuntu_kind[0] == shuntu_kind[3])
              && (shuntu_kind[1] == shuntu_kind[2])))
        yp->ryanpeikou = 1;

    }
  if (shuntu >= 2 && !naki && !yp->ryanpeikou)
    {
      if (shuntu_kind[0] == shuntu_kind[1])
        yp->iipeikou = 1;
    }
  if (shuntu >= 3 && !naki && !yp->ryanpeikou)
    {
      if ((shuntu_kind[0] == shuntu_kind[2])
          || (shuntu_kind[1] == shuntu_kind[2]))
        yp->iipeikou = 1;
    }
  if (shuntu >= 4 && !naki && !yp->ryanpeikou)
    {
      if ((shuntu_kind[0] == shuntu_kind[3])
          || (shuntu_kind[1] == shuntu_kind[3])
          || (shuntu_kind[2] == shuntu_kind[3]))
        yp->iipeikou = 1;
    }
  chanta = IS_YAO (head);
  junchan = IS_JUNYAO (head);
  for (i = 0; i < koutu; i++)
    {
      if (chanta && !IS_YAO (koutu_kind[i]))
        chanta = 0;
      if (junchan && !IS_JUNYAO (koutu_kind[i]))
        junchan = 0;
    }
  for (i = 0; i < shuntu; i++)
    {
      int pk1, pk2;
      pk1 = K_NEXT2 (shuntu_kind[i]);
      pk2 = K_NEXT2 (pk1);

      if (chanta && !(IS_YAO (shuntu_kind[i])
                      || IS_YAO (pk1) || IS_YAO (pk2)))
        chanta = 0;
      if (junchan && !(IS_JUNYAO (shuntu_kind[i])
                       || IS_JUNYAO (pk1) || IS_JUNYAO (pk2)))
        junchan = 0;
    }
  if (junchan)
    {
      if (naki)
        yp->junchan_naki = 1;
      else
        yp->junchan = 1;
    }
  else if (chanta)
    {
      if (naki)
        yp->chanta_naki = 1;
      else
        yp->chanta = 1;
    }
  return 1;
}

int
fu_calc_koutu (int pai, int shift)
{
  int base;
  if (IS_TAN (pai))
    base = 2;
  else
    base = 4;
  return (base << shift);
}

static int
fu_calc_head (global_t *gp, int pk, int p)
{
  int base = 0;

  if ((pk == K_HAKU) || (pk == K_HATU) || (pk == K_CHUN))
    base += 2;
  if (pk == K_TON + big_turn)
    base += 2;
  if (pk == K_TON + p)
    base += 2;
  return base;
}

static int
find_kind (int kind, int *buf, int n)
{
  int i;

  for (i = 0; i < n; i++)
    {
      if (kind == buf[i])
        return 1;
    }
  return 0;
}

int
result_calc_from_hand (global_t *gp, int p, result_t *yp)
{
  int i, j, k;
  int kinds[35], kindsr[35];
  river_t *rp;
  pai_t pai, pk;
  int man, pin, sou, ji, green;

  man = pin = sou = ji = green = 0;

  /* calc kinds */
  for (i = 0; i < 35; i++)
    kinds[i] = kindsr[i] = 0;
  for (i = 0; i < hand[p].closed_num; i++)
    {
      pai = hand[p].closed[i];
      pk = P_KIND (pai);
      kinds[pk]++;
      kindsr[pk]++;
      if (pai == K_MAN_5 * 4)
        yp->akadora += 1;
      if (pai == K_PIN_5 * 4)
        yp->akadora += 1;
      if (pai == K_SOU_5 * 4)
        yp->akadora += 1;
      if (IS_MAN (pk))
        man++;
      else if (IS_PIN (pk))
        pin++;
      else if (IS_SOU (pk))
        sou++;
      else
        ji++;
      if (IS_GREEN (pk))
        green++;
    }
  for (i = 0; i < 4; i++)
    if (k = hand[p].opened_kind[i])
      {
        for (j = 0; j < 4; j++)
          {
            pai = hand[p].opened[i][j];
            if (pai == 0)
              continue;
            pk = P_KIND (pai);
            kindsr[pk]++;
            if (j < 3)
              {
                kinds[pk]++;
                if (IS_MAN (pk))
                  man++;
                else if (IS_PIN (pk))
                  pin++;
                else if (IS_SOU (pk))
                  sou++;
                else
                  ji++;
                if (IS_GREEN (pk))
                  green++;
              }
            if (pai == K_MAN_5 * 4)
              yp->akadora += 1;
            if (pai == K_PIN_5 * 4)
              yp->akadora += 1;
            if (pai == K_SOU_5 * 4)
              yp->akadora += 1;
          }
      }

  /* reach */
  if (hand[p].reach)
    yp->reach = 1;
  /* tumo */
  if (!hand[p].naki && (cur_player == p))
    yp->tumo = 1;

  /* chankan rinshan */
  if ((cur_player != p) && (T_GET (rvp->attr) == T_HN_KAN))
    yp->chankan = 1;
  if ((cur_player == p)
      && (F_GET (rvp->attr) == F_RV_KAN || F_GET (rvp->attr) == F_MT_TOP))
    yp->rinshan = 1;
  /* haitei */
  if (mt_rest (gp) == 0)
    yp->haitei = 1;
  /* ippatu */
  rp = rvp;
  k = 0;
  for (i = 0; rp && i < 5; i++)
    {
      if (F_GET (rp->attr))
        break;
      if (T_GET (rp->attr) == T_HN_KAN)
        break;
      if (T_GET (rp->attr) == T_HN_KAN_C)
        break;
      if ((rv_find_player (gp, rp) == p) && (T_GET (rp->attr) == T_RV_RCH))
        {
          k = 1;
          break;
        }
      if (!rp->prev)
        break;
      rp = rp->prev;
    }
  if (k)
    yp->ippatu = 1;

  /* tenhou chihou renhou + double_reach */
  k = 0;
  rp = &(rv[0][0]);
  for (i = 0; i < 4; i++)
    {
      if (rp == rvp)
        {
          k = 1;
        }
      if (R_GET (rp->attr))
        break;
      if ((T_GET (rp->attr) == T_RV_RCH) && (rv_find_player (gp, rp) == p))
        yp->double_reach = 1;
      if (!rp->next || rp->next == &rv[0][0])
        break;
      rp = rp->next;
    }
  if (k)
    {
      if (rv_find_player (gp, rp) == p)
        if (p == 0)
          yp->tenhou = 1;
        else
          yp->chihou = 1;
      else if (rv_find_player (gp, rp) < p)
        yp->renhou = 1;
    }

  /* kokusi */
  if (kinds[K_MAN_1] && kinds[K_PIN_1] && kinds[K_SOU_1]
      && kinds[K_MAN_9] && kinds[K_PIN_9] && kinds[K_SOU_9]
      && kinds[K_TON] && kinds[K_NAN] && kinds[K_SHA] && kinds[K_PEI]
      && kinds[K_HAKU] && kinds[K_HATU] && kinds[K_CHUN])
    yp->kokusi = 1;
  /* chiraoutou honraotou */
  if ((kinds[K_MAN_1] + kinds[K_PIN_1] + kinds[K_SOU_1]
       + kinds[K_MAN_9] + kinds[K_PIN_9] + kinds[K_SOU_9]) == 14)
    {
      yp->chinraotou = 1;
    }
  else if ((kinds[K_MAN_1] + kinds[K_PIN_1] + kinds[K_SOU_1]
            + kinds[K_MAN_9] + kinds[K_PIN_9] + kinds[K_SOU_9]
            + kinds[K_TON] + kinds[K_NAN] + kinds[K_SHA] + kinds[K_PEI]
            + kinds[K_HAKU] + kinds[K_HATU] + kinds[K_CHUN]) == 14)
    {
      yp->honraotou = 1;
      yp->chanta = 0;
      yp->chanta_naki = 0;
    }

  /* tanyao */
  if (!(kinds[K_MAN_1] || kinds[K_PIN_1] || kinds[K_SOU_1]
        || kinds[K_MAN_9] || kinds[K_PIN_9] || kinds[K_SOU_9]
        || kinds[K_TON] || kinds[K_NAN] || kinds[K_SHA] || kinds[K_PEI]
        || kinds[K_HAKU] || kinds[K_HATU] || kinds[K_CHUN])
      && !(RL_NONAKITAN && hand[p].naki))
    yp->tanyao = 1;

  /* chinitu honitu */
  if ((man == 14) || (pin == 14) || (sou == 14))
    {
      if (hand[p].naki)
        yp->chinitu_naki = 1;
      else
        yp->chinitu = 1;
    }
  else if (ji == 14)
    {
      yp->tuisou = 1;
    }
  else if (((man + ji) == 14) || ((pin + ji) == 14) || ((sou + ji) == 14))
    {
      if (hand[p].naki)
        yp->honitu_naki = 1;
      else
        yp->honitu = 1;
    }
  if (green == 14)
    yp->ryuisou = 1;

  /* calc result hai */
  if (kinds[K_HAKU] >= 3)
    yp->haku = 1;
  if (kinds[K_HATU] >= 3)
    yp->hatu = 1;
  if (kinds[K_CHUN] >= 3)
    yp->chun = 1;
  if (kinds[K_TON + big_turn] >= 3)
    yp->bakaze = 1;
  if (kinds[K_TON + p] >= 3)
    yp->jikaze = 1;
  if ((kinds[K_HAKU] + kinds[K_HATU] + kinds[K_CHUN]) == 9)
    yp->daisangen = 1;
  if ((kinds[K_HAKU] + kinds[K_HATU] + kinds[K_CHUN]) == 8)
    yp->syosangen = 1;
  if ((kinds[K_TON] + kinds[K_NAN] + kinds[K_SHA] + kinds[K_PEI]) == 12)
    yp->daisusi = 1;
  if ((kinds[K_TON] + kinds[K_NAN] + kinds[K_SHA] + kinds[K_PEI]) == 11)
    yp->syosusi = 1;

  /* calc dora */
  for (i = 0; i < 5; i++)
    if (k = mt_dora (gp, i))
      yp->dora += kindsr[K_NEXT (P_KIND (k))];
  if (yp->reach)
    for (i = 0; i < (RL_NOKANURA ? 1 : 5); i++)
      if (k = mt_uradora (gp, i))
        yp->uradora += kindsr[K_NEXT (P_KIND (k))];
}

int
result_calc_point (global_t *gp, int p, result_t *yp)
{
  int yfan = 0;
  int fan = 0;
  int fan2 = 0;
  int point = 0;
  int i;

  yp->pao = -1;
  if (yp->kokusi)
    yfan += 1;
  if (yp->daisangen)
    yfan += 1;
  if (yp->suankou)
    yfan += 1;
  if (yp->sukantu)
    yfan += 1;
  if (yp->syosusi)
    yfan += 1;
  if (yp->daisusi)
    yfan += 1;
  if (yp->tuisou)
    yfan += 1;
  if (yp->ryuisou)
    yfan += 1;
  if (yp->cyurenpotou)
    yfan += 1;
  if (yp->tenhou)
    yfan += 1;
  if (yp->chihou)
    yfan += 1;
  if (yp->chinraotou)
    yfan += 1;

  if (yfan)
    {
      yp->yakuman = 1;
      point = 32000 * yfan;
      result_clear_genyakus (yp);
      goto calc;
    }
  if (yp->renhou)
    {
      result_clear_genyakus (yp);
      point = 16000;
      goto calc;
    }
  if (yp->nagasimangan)
    {
      point = 8000;
      goto calc;
    }

  if (yp->double_reach)
    fan += 1;
  if (yp->reach)
    fan += 1;
  if (yp->ippatu)
    fan += 1;
  if (yp->tumo)
    fan += 1;
  if (yp->pinfu)
    fan += 1;
  if (yp->tanyao)
    fan += 1;
  if (yp->iipeikou)
    fan += 1;
  if (yp->haitei)
    fan += 1;
  if (yp->jikaze)
    fan += 1;
  if (yp->bakaze)
    fan += 1;
  if (yp->haku)
    fan += 1;
  if (yp->hatu)
    fan += 1;
  if (yp->chun)
    fan += 1;
  if (yp->chankan)
    fan += 1;
  if (yp->rinshan)
    fan += 1;
  if (yp->sansiki_naki)
    fan += 1;
  if (yp->ittu_naki)
    fan += 1;
  if (yp->chanta_naki)
    fan += 1;

  if (yp->chitoitu)
    fan += 2;
  if (yp->sansiki)
    fan += 2;
  if (yp->ittu)
    fan += 2;
  if (yp->chanta)
    fan += 2;
  if (yp->sanankou)
    fan += 2;
  if (yp->sankantu)
    fan += 2;
  if (yp->toitoi)
    fan += 2;
  if (yp->syosangen)
    fan += 2;
  if (yp->honraotou)
    fan += 2;
  if (yp->sansiki_doukoku)
    fan += 2;
  if (yp->honitu_naki)
    fan += 2;
  if (yp->junchan_naki)
    fan += 2;

  if (yp->honitu)
    fan += 3;
  if (yp->junchan)
    fan += 3;
  if (yp->ryanpeikou)
    fan += 3;

  if (yp->chinitu_naki)
    fan += 5;
  if (yp->chinitu)
    fan += 6;

  if (fan == 0)
    {
      return 0;
    }

  fan2 += yp->dora;
  fan2 += yp->uradora;
  if (!RL_NOAKADORA)
    fan2 += yp->akadora;

  if ((fan + fan2) >= 13)
    point = 32000;
  else if ((fan + fan2) >= 11)
    point = 24000;
  else if ((fan + fan2) >= 8)
    point = 16000;
  else if ((fan + fan2) >= 6)
    point = 12000;
  if (point)
    goto calc;

  point = yp->fu * 16;
  point <<= (fan + fan2);

  if (RL_77MANGAN && point > 7600)
    point = 8000;
  if (point > 8000)
    point = 8000;

calc:
  yp->fan = fan + fan2;

  yp->points[0] = yp->points[1] = yp->points[2] = yp->points[3] = 0;

  if (cur_player == p)
    {                           /* tumo */
      if (p == 0)
        {
          if ((yp->pao = hand_check_pao (gp, p)) >= 0)
            {
              yp->points[yp->pao] -= 48000;
              point -= 48000;
              if (point > 0)
                yp->pao = -1;
            }
          yp->points[1] += ((point / 2) + 99) / 100 * -100;
          yp->points[2] += ((point / 2) + 99) / 100 * -100;
          yp->points[3] += ((point / 2) + 99) / 100 * -100;
          yp->points[0] = 0 - yp->points[1] - yp->points[2] - yp->points[3];
        }
      else
        {
          if ((yp->pao = hand_check_pao (gp, p)) >= 0)
            {
              yp->points[yp->pao] -= 32000;
              point -= 32000;
              if (point > 0)
                yp->pao = -1;
            }
          yp->points[0] += ((point / 2) + 99) / 100 * -100;
          for (i = 1; i < 4; i++)
            if (i != p)
              {
                yp->points[i] += ((point / 4) + 99) / 100 * -100;
              }
          yp->points[p] = 0 - yp->points[0] - yp->points[1]
            - yp->points[2] - yp->points[3];

        }

    }
  else
    {
      if (p == 0)
        point = point / 2 * 3;
      if ((yp->pao = hand_check_pao (gp, p)) >= 0)
        {
          if (p == 0)
            {
              yp->points[yp->pao] -= 24000;
              point -= 24000;
            }
          else
            {
              yp->points[yp->pao] -= 16000;
              point -= 16000;
            }
          yp->pao = -1;
        }
      yp->points[cur_player] = (point + 99) / 100 * -100;
      for (i = 0; i < 4; i++)
        if (i != p)
          {
            yp->points[p] += 0 - yp->points[i];
          }
    }
  return point;
}

static void
result_clear_genyakus (result_t *yp)
{
  yp->double_reach = 0;
  yp->reach = 0;
  yp->tumo = 0;
  yp->pinfu = 0;
  yp->tanyao = 0;
  yp->iipeikou = 0;
  yp->haitei = 0;
  yp->jikaze = 0;
  yp->bakaze = 0;
  yp->haku = 0;
  yp->hatu = 0;
  yp->chun = 0;
  yp->chankan = 0;
  yp->rinshan = 0;
  yp->sansiki_naki = 0;
  yp->ittu_naki = 0;
  yp->chanta_naki = 0;
  yp->chitoitu = 0;
  yp->sansiki = 0;
  yp->ittu = 0;
  yp->chanta = 0;
  yp->sanankou = 0;
  yp->toitoi = 0;
  yp->syosangen = 0;
  yp->honraotou = 0;
  yp->sansiki_doukoku = 0;
  yp->honitu_naki = 0;
  yp->junchan_naki = 0;
  yp->honitu = 0;
  yp->junchan = 0;
  yp->ryanpeikou = 0;
  yp->chinitu_naki = 0;
  yp->chinitu = 0;
  yp->dora = 0;
}

result_set_cyonbo (gp, p, n)
     global_t *gp;
{
  int i;
  if (p == 0)
    {
      result[n].flag = RESULT_CYONBO;
      result[n].points[0] = -12000;
      for (i = 1; i < 4; i++)
        result[n].points[i] = 4000;
    }
  else
    {
      result[n].flag = RESULT_CYONBO_CONT;
      result[n].points[0] = 4000;
      for (i = 1; i < 4; i++)
        result[n].points[i] = 2000;
      result[n].points[p] = -8000;
    }
  result[n].who = p;
}


result_set_who (y)
     result_t *y;
{
  int p;
  y->who = -1;
  switch (y->flag)
    {
    case RESULT_AGARI_RON:
    case RESULT_AGARI_TUMO:
      for (p = 0; p < 4; p++)
        {
          if (y->points[p] > 0)
            {
              y->who = p;
              break;
            }
        }
      break;
    case RESULT_CYONBO:
    case RESULT_CYONBO_CONT:
      for (p = 0; p < 4; p++)
        {
          if (y->points[p] < 0)
            {
              y->who = p;
              break;
            }
        }
      break;
    }
}

reach_can_kan (gp, p, pai)
     global_t *gp;
{
  ahand_t ah, *ap;
  int i, j, f, pk;

  if (hand_can_kan (gp, p, pai) != 3)
    return 0;
  ah_new (&ah, &hand[p], -1);
  ah_analize (&ah, 0);
  pk = P_KIND (pai);

  for (i = 0; i < result_cnt; i++)
    {
      f = 0;
      ap = &result_ahand[i];
      for (j = 0; j < ap->part_num; j++)
        {
          if (ap->part_kind[j] == AH_V3 && ap->part[j][0] == pk)
            f = 1;
        }
      if (!f)
        return 0;
    }
  return 1;
}
