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
#include "ruledef.h"

extern FILE *df;
extern int tonpu;

#define ALL_LAST	\
	(tonpu? small_turn == 3: big_turn==1 && small_turn == 3)

int param_chitoi = 700;
int param_kokushi = 700;
int param_toitoi = 700;
int param_tanyao = 700;
int param_color = 700;
int param_yaku = 700;

int param_random = 50;
int param_dora = 100;
int param_zone = 300;
int param_suji = 100;
int param_yomi = 500;

int param_reach = 700;
int param_kan = 300;
int param_pon = 300;
int param_tie = 300;

int param_strategy = 50;

#define POL_NULL        0
#define POL_CHITOI      1
#define POL_KOKUSHI     2
#define POL_TOITOI      3
#define POL_TANYAO      4
#define POL_COLOR_MAN   5
#define POL_COLOR_PIN   6
#define POL_COLOR_SOU   7
#define POL_YAKU   	8

static int policy;
static int pol_point, yaku_point, dora_point;
static int pol_kind;
static int yomi_pai[K_MAX];     /* kiken % * 100 */
static int yomi_rank[K_MAX];
static int reach_count;

static int myposition;          /* 1: TOP 0: 2-3 -1:LAST */
static int pointdiff;

typedef struct
{
  int pai_num;
  pai_t pai[14];
  int pnt[14];
} pnt_t;

static void calc_myposition (global_t *gp);
static int analize_policy (global_t *gp);
static void analize_base (global_t *gp, int p, int max, int *rp);
static void filter_random (global_t *gp, int p, int max, int *rp);
static void filter_dora (global_t *gp, int p, int max, int *rp);
static void filter_zone (global_t *gp, int p, int max, int *rp);
static void filter_color (global_t *gp, int p, int max, int *rp);
static void filter_suji (global_t *gp, int p, int max, int *rp);
static void pnt_calc_kokushi (global_t *gp, int *r, int *rests);
static void pnt_calc_chitoi (global_t *gp, int *r, int *rests);
static void pnt_calc (global_t *gp, int *r, int *rests);
static void pnt_sort (pnt_t *p);
static int analize_kan (global_t *gp);
static int analize_pon (global_t *gp);
static int analize_tie (global_t *gp, int *howp);
static void filter_yomi (global_t *gp, int p, int max, int *rp);
static int has_yakupai (global_t *gp);
static char *polstr (int p);
static void print_pai (global_t *gp);
static void print_point (global_t *gp, char *name, int *r);
static int naki_look (global_t *gp, int p);

static void
calc_myposition (global_t *gp)
{
  int i, j, tmp;
  int s[4];

  for (i = 0; i < 4; i++)
    s[i] = (i + first_player) % 4;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 3; j++)
      {
        if (player[s[j]].gpoint < player[s[j + 1]].gpoint)
          {
            tmp = s[j];
            s[j] = s[j + 1];
            s[j + 1] = tmp;
          }
      }
  for (i = 0; i < 4; i++)
    {
      if (s[i] == vself)
        break;
    }
  switch (i)
    {
    case 0:
      myposition = 1;
      break;
    case 3:
      myposition = -1;
      break;
    default:
      myposition = 0;
    }
  pointdiff = player[s[0]].gpoint - player[vself].gpoint;
}


char *
analize_message (global_t *gp)
{
  static char buf[1024];
  extern char *pai_strings[];
  int i;
  char *p, *s[5];

  p = buf;

  sprintf (p, "\nここは%sで、%s%s。\n",
           policy == POL_CHITOI ? "チートイ狙い"
           : policy == POL_KOKUSHI ? "国士狙い"
           : policy == POL_TOITOI ? "トイトイ狙い"
           : policy == POL_TANYAO ? "タンヤオ狙い"
           : policy == POL_COLOR_MAN ? "マンズ狙い"
           : policy == POL_COLOR_PIN ? "ピンズ狙い"
           : policy == POL_COLOR_SOU ? "ソウズ狙い"
           : policy == POL_YAKU ? "役牌狙い"
           : "手なり",
           reach_count && yomi_pai[P_KIND (ask_pai)] == 0 ? "堅く"
           : reach_count && yomi_pai[P_KIND (ask_pai)] >= 1000 ? "強く"
           : reach_count && yomi_pai[P_KIND (ask_pai)] >= 3000 ? "玉砕覚悟で"
           : "",
           result_rest < 3 ? "攻めましょう"
           : result_rest < 4 ? "行きましょう" : "様子を見ましょう");
  return buf;
}

void
analize_init (global_t *gp)
{
  pf_param (gp, "kokushi", &param_kokushi);
  pf_param (gp, "chitoi", &param_chitoi);
  pf_param (gp, "toitoi", &param_toitoi);
  pf_param (gp, "tanyao", &param_tanyao);
  pf_param (gp, "color", &param_color);
  pf_param (gp, "yaku", &param_yaku);

  pf_param (gp, "random", &param_random);
  pf_param (gp, "dora", &param_dora);
  pf_param (gp, "zone", &param_zone);
  pf_param (gp, "suji", &param_suji);
  pf_param (gp, "yomi", &param_yomi);

  pf_param (gp, "reach", &param_reach);
  pf_param (gp, "kan", &param_kan);
  pf_param (gp, "pon", &param_pon);
  pf_param (gp, "tie", &param_tie);

  pf_param (gp, "strategy", &param_strategy);
}

pnt_t result_pnt;

void
analize (global_t *gp, int p)
{
  int r[14];
  int i, n, doff;
  int low, low_i = 0;
  result_t y;
  ahand_t a;

  calc_myposition (gp);
  ah_new (&a, &hand[p], -1);
  ah_analize (&a, 0);
  analize_policy (gp);
  n = hand[p].closed_num;
  if (result_check_kyushukyupai (gp) == 1 && policy == POL_NULL)
    {
      ask_attr = T_HN_TSM;
      ask_pai = P_NULL;
      return;
    }

  doff = (70 - mt_rest (gp)) * 10;
  print_pai (gp);
  analize_base (gp, p, 1000 + doff, r);
  print_point (gp, "base", r);
  filter_random (gp, p, param_random, r);
  filter_dora (gp, p, param_dora, r);
  filter_color (gp, p, param_color / 3, r);
  filter_zone (gp, p, param_zone, r);
  filter_suji (gp, p, param_suji, r);
  filter_yomi (gp, p, param_yomi, r);
  print_point (gp, "last", r);

  low = 10000;
  for (i = 0; i < n; i++)
    {
      if (r[i] < low)
        {
          low = r[i];
          low_i = i;
        }
    }
  ask_attr = T_RV;
  ask_pai = hand[p].closed[low_i];
  if ((result_rest == 0) &&
      (!hand[p].naki || result_calc_yaku (gp, vself, &y) > 0))
    {
      ask_attr = T_HN_TSM;
      ask_pai = P_NULL;
      return;
    }

  /* analize for REACH */
  if (ask_attr == T_RV)
    {
      for (i = 0; i < n; i++)
        {
          if (hand[p].closed[i] == ask_pai)
            break;
        }
      ah_new (&a, &hand[p], i);
      ah_analize (&a, 0);
      if (!hand[p].naki && result_rest == 1
          && ((random () % 1000) * 70 <= param_reach * (mt_rest (gp))))
        ask_attr = T_RV_RCH;
    }
  if (hand_can_kan (gp, vself, ask_pai))
    {
      ah_new (&a, &hand[p], -1);
      ah_analize (&a, 0);
      analize_policy (gp);
      if (policy != POL_CHITOI && policy != POL_KOKUSHI &&
          (result_rest == 1
           || (result_rest <= 2 && random () < 1000 < param_kan)))
        ask_attr = T_HN_KAN;
    }
}

static int
analize_policy (global_t *gp)
{
  int n, i, j, k, x;
  int pai, paik, kind;
  int v2_num, v3_num, yao_num, yao_kinds;
  int h3x_num, v3x_num;
  int manx_num, pinx_num, soux_num;
  int kinds[K_MAX];
  int pol_chitoi = 1;
  int pol_kokushi = 1;
  int pol_toitoi = 1;
  int pol_tanyao = 1;
  int pol_color = 1;
  int pol_yaku = 1;
  int dora[5];
  int dora_point_tan, dora_point_man, dora_point_pin, dora_point_sou;
  int stage;
  ahand_t *a;
  int m3[2], m2[2], p3[2], p2[2], s3[2], s2[2], j3[2], j2[2];
  int t3f, t3, t2, tn;

  stage = mt_rest (gp) / 24;

  policy = 0;
  yaku_point = dora_point = pol_point = 0;
  v2_num = v3_num = yao_num = yao_kinds = 0;
  h3x_num = v3x_num = 0;
  manx_num = pinx_num = soux_num = 0;
  dora_point_tan = dora_point_man = dora_point_pin = dora_point_sou = 0;
  m3[0] = m2[0] = p3[0] = p2[0] = s3[0] = s2[0] = j3[0] = j2[0] = 0;
  m3[1] = m2[1] = p3[1] = p2[1] = s3[1] = s2[1] = j3[1] = j2[1] = 0;
  t3f = 0;

  for (i = 0; i < K_MAX; i++)
    kinds[i] = 0;
  for (i = 0; i < 5; i++)
    dora[i] = K_NEXT (P_KIND (mt_dora (gp, i)));

  for (i = 0; i < 4; i++)
    if (kind = hand[vself].opened_kind[i])
      {
        t3f++;
        switch (kind)
          {
          case H_TIE:
            kinds[P_KIND (hand[vself].opened[i][0])]++;
            kinds[P_KIND (hand[vself].opened[i][1])]++;
            kinds[P_KIND (hand[vself].opened[i][2])]++;
            h3x_num++;
            break;
          case H_PON:
          case H_KAN_OPENED:
          case H_KAN_CLOSED:
            kinds[P_KIND (hand[vself].opened[i][0])] += 3;
            v3x_num++;
            break;
          }
        paik = P_KIND (hand[vself].opened[i][0]);
        if (paik == K_HAKU || paik == K_HATU || paik == K_CHUN ||
            paik == (K_TON + vself) || paik == (K_TON + big_turn))
          yaku_point += 15;
        if (paik == (K_TON + vself) && paik == (K_TON + big_turn))
          yaku_point += 15;

        if (IS_MAN (paik))
          m3[0]++, m3[1]++;
        else if (IS_PIN (paik))
          p3[0]++, p3[1]++;
        else if (IS_SOU (paik))
          s3[0]++, s3[1]++;
        else
          j3[0]++, j3[1]++;

        for (j = 0; j < 4; j++)
          {
            pai = hand[vself].opened[i][j];
            paik = P_KIND (pai);
            if (!RL_NOAKADORA)
              {
                if (pai == (K_MAN_5 << 2) || pai == (K_PIN_5 << 2)
                    || pai == (K_SOU_5 << 2))
                  {
                    dora_point += 15;
                    dora_point_tan += 15;
                  }
                if (pai == (K_MAN_5 << 2))
                  dora_point_man += 15;
                if (pai == (K_PIN_5 << 2))
                  dora_point_pin += 15;
                if (pai == (K_SOU_5 << 2))
                  dora_point_sou += 15;
              }
            for (k = 0; k < 5; k++)
              if (dora[k])
                {
                  if (paik == dora[k])
                    dora_point += 10;
                  if (paik == dora[k] && IS_TAN (paik))
                    dora_point_tan += 10;
                  if (paik == dora[k] && IS_MAN (paik))
                    dora_point_man += 10;
                  if (paik == dora[k] && IS_PIN (paik))
                    dora_point_pin += 10;
                  if (paik == dora[k] && IS_SOU (paik))
                    dora_point_sou += 10;
                }
          }
      }
  for (i = 1; i < K_MAX; i++)
    {
      if (kinds[i] && IS_YAO (i))
        pol_tanyao = 0;
      if (IS_MAN (i))
        manx_num += kinds[i];
      else if (IS_PIN (i))
        pinx_num += kinds[i];
      else if (IS_SOU (i))
        soux_num += kinds[i];
    }

  tn = 0;
  for (x = 0; x < result_cnt; x++)
    {
      a = result_ahand + x;
      t3 = t2 = 0;
      for (i = 0; i < a->part_num; i++)
        {
          switch (a->part_kind[i])
            {
            case AH_V3:
              if (x < 2)
                {
                  if (IS_MAN (a->part[i][0]))
                    m3[x]++;
                  else if (IS_PIN (a->part[i][0]))
                    p3[x]++;
                  else if (IS_SOU (a->part[i][0]))
                    s3[x]++;
                  else
                    j3[x]++;
                }
              if (IS_TAN (a->part[i][0]))
                t3++;
              else
                pol_tanyao = 0;
              break;
            case AH_H3:
              if (x < 2)
                {
                  if (IS_MAN (a->part[i][0]))
                    m3[x]++;
                  else if (IS_PIN (a->part[i][0]))
                    p3[x]++;
                  else if (IS_SOU (a->part[i][0]))
                    s3[x]++;
                  else
                    j3[x]++;
                }
              if (IS_TAN (a->part[i][0]) && IS_TAN (a->part[i][2]))
                t3++;
              break;
            case AH_V2:
            case AH_H2a:
            case AH_H2b:
            case AH_H2c:
              if (x < 2)
                {
                  if (IS_MAN (a->part[i][0]))
                    m2[x]++;
                  else if (IS_PIN (a->part[i][0]))
                    p2[x]++;
                  else if (IS_SOU (a->part[i][0]))
                    s2[x]++;
                  else
                    j2[x]++;
                }
              if (IS_TAN (a->part[i][0]) && IS_TAN (a->part[i][1]))
                t2++;
              break;
            }
        }
      if (t3 * 2 + t2 > tn)
        tn = (t3 * 2 + t2);
    }

  if (hand[vself].naki)
    {
      pol_chitoi = 0;
      pol_kokushi = 0;
      if (RL_NONAKITAN)
        {
          pol_tanyao = 0;
        }
      if (h3x_num)
        {
          pol_toitoi = 0;
        }
    }

  n = hand[vself].closed_num;
  for (i = 0; i < n; i++)
    {
      pai = hand[vself].closed[i];
      paik = P_KIND (pai);
      kinds[paik]++;
      if (!RL_NOAKADORA)
        {
          if (pai == (K_MAN_5 << 2) || pai == (K_PIN_5 << 2)
              || pai == (K_SOU_5 << 4))
            {
              dora_point += 15;
              dora_point_tan += 15;
            }
          if (pai == (K_MAN_5 << 2))
            dora_point_man += 15;
          if (pai == (K_PIN_5 << 2))
            dora_point_pin += 15;
          if (pai == (K_SOU_5 << 2))
            dora_point_sou += 15;
        }
      for (k = 0; k < 5; k++)
        {
          if (paik == dora[k])
            dora_point += 10;
          if (paik == dora[k] && IS_TAN (paik))
            dora_point_tan += 10;
          if (paik == dora[k] && IS_MAN (paik))
            dora_point_man += 10;
          if (paik == dora[k] && IS_PIN (paik))
            dora_point_pin += 10;
          if (paik == dora[k] && IS_SOU (paik))
            dora_point_sou += 10;
        }
    }

  for (i = 1; i < K_MAX; i++)
    {
      if (kinds[i] == 2)
        v2_num++;
      if (kinds[i] == 3)
        v3_num++;
      if (IS_YAO (i))
        {
          yao_num += kinds[i];
          if (kinds[i])
            yao_kinds++;
          if (kinds[i] == 0 && mt_rest_kind (gp, i) == 0)
            pol_kokushi = 0;
        }
      else
        {
          if (kinds[i] >= 3 || kinds[i] == 2 && mt_rest_kind (gp, i) >= 1)
            {
              if (i == K_HAKU || i == K_HATU || i == K_CHUN ||
                  i == (K_TON + vself) || i == (K_TON + big_turn))
                yaku_point += kinds[i] >= 3 ? 15 : 10;
              if (i == (K_TON + vself) && i == (K_TON + big_turn))
                yaku_point += kinds[i] >= 3 ? 15 : 10;
            }
        }
    }

  if (pol_chitoi)
    {
      pol_chitoi = (v2_num + v3_num) * param_chitoi / (4 + stage);
      pol_chitoi += (yaku_point + dora_point) * 5;
      pol_chitoi += myposition ? param_strategy : 0;

      if (pol_chitoi > pol_point)
        {
          pol_point = pol_chitoi;
          policy = POL_CHITOI;
        }
    }
  if (pol_kokushi)
    {
      pol_kokushi = yao_kinds * param_kokushi / (10 - stage);
      pol_kokushi -= myposition * param_strategy;

      if (pol_kokushi > pol_point)
        {
          pol_point = pol_kokushi;
          policy = POL_KOKUSHI;
        }
    }
  if (result_rest < 3 && yaku_point)
    {
      pol_yaku = (dora_point + yaku_point) * param_yaku / 35;
      pol_yaku += (3 - result_rest) * 100;
      pol_yaku += myposition * param_strategy;

      if (pol_yaku > pol_point)
        {
          pol_point = pol_yaku;
          policy = POL_YAKU;
        }
    }
  if (pol_toitoi)
    {
      int n;
      n = (v2_num > 5 - v3_num) ? (5 - v3_num) : v2_num;
      pol_toitoi = (v3_num * 2 + n) * param_toitoi / 6;
      pol_toitoi += (yaku_point + dora_point) * 5;
      pol_toitoi += myposition ? 0 : param_strategy;

      if (pol_toitoi > pol_point)
        {
          pol_point = pol_toitoi;
          policy = POL_TOITOI;
        }
    }
  if (!pinx_num && !soux_num && !p3[0] && !p3[1] && !s3[0] && !s3[1])
    {
      int n;
      if ((m3[0] + j3[0]) * 2 + m2[0] + j2[0] >
          (m3[1] + j3[1]) * 2 + m2[1] + j2[1])
        n = (m3[0] + j3[0]) * 2 + m2[0] + j2[0];
      else
        n = (m3[1] + j3[1]) * 2 + m2[1] + j2[1];
      pol_color = n * param_color / 6;
      pol_color += (yaku_point + dora_point_man) * 5;
      pol_color += myposition ? 0 : param_strategy;

      if (pol_color > pol_point)
        {
          dora_point = dora_point_man;
          pol_point = pol_color;
          policy = POL_COLOR_MAN;
        }
    }
  if (!soux_num && !manx_num && !s3[0] && !s3[1] && !m3[0] && !m3[1])
    {
      int n;
      if ((p3[0] + j3[0]) * 2 + p2[0] + j2[0] >
          (p3[1] + j3[1]) * 2 + p2[1] + j2[1])
        n = (p3[0] + j3[0]) * 2 + p2[0] + j2[0];
      else
        n = (p3[1] + j3[1]) * 2 + p2[1] + j2[1];
      pol_color = n * param_color / 6;
      pol_color += (yaku_point + dora_point_man) * 5;
      pol_color += myposition ? 0 : param_strategy;

      if (pol_color > pol_point)
        {
          dora_point = dora_point_pin;
          pol_point = pol_color;
          policy = POL_COLOR_PIN;
        }
    }
  if (!manx_num && !pinx_num && !m3[0] && !m3[1] && !p3[0] && !p3[1])
    {
      int n;
      if ((s3[0] + j3[0]) * 2 + s2[0] + j2[0] >
          (s3[1] + j3[1]) * 2 + s2[1] + j2[1])
        n = (s3[0] + j3[0]) * 2 + s2[0] + j2[0];
      else
        n = (s3[1] + j3[1]) * 2 + s2[1] + j2[1];
      pol_color = n * param_color / 6;
      pol_color += (yaku_point + dora_point_man) * 5;
      pol_color += myposition ? 0 : param_strategy;

      if (pol_color > pol_point)
        {
          dora_point = dora_point_sou;
          pol_point = pol_color;
          policy = POL_COLOR_SOU;
        }
    }
  if (pol_tanyao)
    {
      pol_tanyao = (t3f * 2 + tn) * param_tanyao / 7;
      pol_tanyao += (dora_point_tan) * 5;
      pol_tanyao -= myposition >= 0 ? 0 : param_strategy;

      if (pol_tanyao > pol_point)
        {
          dora_point = dora_point_tan;
          pol_point = pol_tanyao;
          policy = POL_TANYAO;
        }
    }
  pol_kind = policy;

  if (pol_point < 700)
    policy = 0;

  return policy ? pol_point : 0;
}


static void
analize_base (global_t *gp, int p, int max, int *rp)
{
  ahand_t a;
  int rests[14];
  int r[14];
  int r_max;
  int i, j, x, n;

  n = hand[p].closed_num;
  for (i = 0; i < n; i++)
    {
      ah_new (&a, &hand[p], i);
      ah_analize (&a, 0);
      rests[i] = result_rest;
    }
  ah_new (&a, &hand[p], -1);
  ah_analize (&a, 0);

  switch (policy)
    {
    case POL_KOKUSHI:
      pnt_calc_kokushi (gp, r, rests);
      break;
    case POL_CHITOI:
      pnt_calc_chitoi (gp, r, rests);
      break;
    case POL_TOITOI:
    case POL_TANYAO:
    case POL_COLOR_MAN:
    case POL_COLOR_PIN:
    case POL_COLOR_SOU:
    default:
      pnt_calc (gp, r, rests);
    }

  r_max = 0;
  for (i = j = 0; i < n; i++)
    {
      if (r_max < r[i])
        r_max = r[i];
    }
  for (i = 0; i < n; i++)
    {
      if (r_max)
        x = (int) ((double) r[i] * max / r_max);
      else
        x = 0;
      *rp++ = x;
    }
  return;
}

static void
filter_random (global_t *gp, int p, int max, int *rp)
{
  int i, x;
  for (i = 0; i < hand[p].closed_num; i++)
    {
      x = random () % max;
      *rp++ += x;
    }
}

static void
filter_dora (global_t *gp, int p, int max, int *rp)
{
  int i, j, x, n;
  int r[14];
  int pai, paik, dora;
  int r_max = 0;

  n = hand[p].closed_num;
  for (i = 0; i < n; i++)
    {
      r[i] = 0;
      pai = hand[p].closed[i];

      if (!RL_NOAKADORA)
        {
          if (pai == (K_MAN_5 << 2))
            r[i] += 8;
          else if (pai == (K_PIN_5 << 2))
            r[i] += 8;
          else if (pai == (K_SOU_5 << 2))
            r[i] += 8;
        }

      paik = P_KIND (pai);

      for (j = 0; j < 5; j++)
        {
          dora = K_NEXT (P_KIND (mt_dora (gp, j)));
          if (dora == paik)
            r[i] += 8;
          if (K_NEXT2 (dora) == paik)
            r[i] += 4;
          if (K_PREV2 (dora) == paik)
            r[i] += 4;
          if (K_NEXT2 (K_NEXT2 (dora)) == paik)
            r[i] += 2;
          if (K_PREV2 (K_PREV2 (dora)) == paik)
            r[i] += 2;
        }

      r[i] = r[i] * rp[i];
      if (r_max < r[i])
        r_max = r[i];
    }

  for (i = 0; i < hand[p].closed_num; i++)
    {
      if (r_max)
        x = (int) ((double) r[i] * max / r_max);
      else
        x = 0;

      *rp++ += x;
    }
}

static void
filter_zone (global_t *gp, int p, int max, int *rp)
{
  int i, j, x, n;
  int r[14];
  int paik;
  int r_max;
  int zone[7];
  int b = max / 200;
  int d = max / 100;

  if (policy == POL_TOITOI || policy == POL_CHITOI)
    d /= 2;

  r_max = max;
  for (i = 0; i < 7; i++)
    zone[i] = b;

  n = hand[p].closed_num;
  for (i = 0; i < n; i++)
    {
      paik = P_KIND (hand[p].closed[i]);

      if (IS_PIN (paik))
        paik += K_MAN_1 - K_PIN_1;
      else if (IS_SOU (paik))
        paik += K_MAN_1 - K_SOU_1;

      if (paik == K_MAN_1 || paik == K_MAN_2 || paik == K_MAN_3)
        zone[0] += d;
      if (paik == K_MAN_2 || paik == K_MAN_3 || paik == K_MAN_4)
        zone[1] += d;
      if (paik == K_MAN_3 || paik == K_MAN_4 || paik == K_MAN_5)
        zone[2] += d;
      if (paik == K_MAN_4 || paik == K_MAN_5 || paik == K_MAN_6)
        zone[3] += d;
      if (paik == K_MAN_5 || paik == K_MAN_6 || paik == K_MAN_7)
        zone[4] += d;
      if (paik == K_MAN_6 || paik == K_MAN_7 || paik == K_MAN_8)
        zone[5] += d;
      if (paik == K_MAN_7 || paik == K_MAN_8 || paik == K_MAN_9)
        zone[6] += d;
    }
  for (i = 0; i < 7; i++)
    {
      zone[i] = zone[i] * zone[i] / 3;
    }

  for (i = 0; i < n; i++)
    {
      r[i] = 0;
      paik = P_KIND (hand[p].closed[i]);

      if (IS_PIN (paik))
        paik += K_MAN_1 - K_PIN_1;
      else if (IS_SOU (paik))
        paik += K_MAN_1 - K_SOU_1;

      switch (paik)
        {
        case K_MAN_1:
          r[i] = (zone[0]);
          break;
        case K_MAN_2:
          r[i] = (zone[0] + zone[1]);
          break;
        case K_MAN_3:
          r[i] = (zone[0] + zone[1] + zone[2]);
          break;
        case K_MAN_4:
          r[i] = (zone[1] + zone[2] + zone[3]);
          break;
        case K_MAN_5:
          r[i] = (zone[2] + zone[3] + zone[4]);
          break;
        case K_MAN_6:
          r[i] = (zone[3] + zone[4] + zone[5]);
          break;
        case K_MAN_7:
          r[i] = (zone[4] + zone[5] + zone[6]);
          break;
        case K_MAN_8:
          r[i] = (zone[5] + zone[6]);
          break;
        case K_MAN_9:
          r[i] = (zone[6]);
          break;
        default:
          r[i] = (zone[0] + zone[6]);
        }
      if (r[i] > r_max)
        r_max = r[i];
    }

  for (i = 0; i < hand[p].closed_num; i++)
    {
      if (r_max)
        x = (int) ((double) r[i] * max / r_max);
      else
        x = 0;

      *rp++ += x;
    }
}

static void
filter_color (global_t *gp, int p, int max, int *rp)
{
  int i, j, x, n;
  int r[14];
  int man, pin, sou, ji;
  int paik;
  int r_max;
  int b = max / 200;
  int d = max / 100;

  r_max = max;
  man = b;
  pin = b;
  sou = b;
  ji = 0;

  n = hand[p].closed_num;
  for (i = 0; i < n; i++)
    {
      r[i] = 0;
      paik = P_KIND (hand[p].closed[i]);

      if (IS_MAN (paik))
        man += d;
      else if (IS_PIN (paik))
        pin += d;
      else if (IS_SOU (paik))
        sou += d;
      else
        ji += d;
    }

  for (i = 0; i < 4; i++)
    if (hand[p].opened_kind[i])
      {
        paik = P_KIND (hand[p].opened[i][0]);

        if (IS_MAN (paik))
          pin = sou = b;
        else if (IS_PIN (paik))
          sou = man = b;
        else if (IS_SOU (paik))
          man = pin = b;
      }

  if (man > pin && man > sou)
    {
      man += ji / 2;
      ji += man / 2;
    }
  else if (pin > sou && pin > man)
    {
      pin += ji / 2;
      ji += pin / 2;
    }
  else if (sou > man && sou > pin)
    {
      sou += ji / 2;
      ji += sou / 2;
    }

  man = man * man * (hand[p].naki ? 2 : 1);
  pin = pin * pin * (hand[p].naki ? 2 : 1);
  sou = sou * sou * (hand[p].naki ? 2 : 1);
  ji = ji * ji * (hand[p].naki ? 2 : 1);

  for (i = 0; i < n; i++)
    {
      paik = P_KIND (hand[p].closed[i]);

      if (IS_MAN (paik))
        r[i] = man / 3;
      else if (IS_PIN (paik))
        r[i] = pin / 3;
      else if (IS_SOU (paik))
        r[i] = sou / 3;
      else
        r[i] = ji / 3;

      if (r[i] > r_max)
        r_max = r[i];
    }
  for (i = 0; i < hand[p].closed_num; i++)
    {
      if (r_max)
        x = (int) ((double) r[i] * max / r_max);
      else
        x = 0;

      *rp++ += x;
    }
}

static void
filter_suji (global_t *gp, int p, int max, int *rp)
{
  int i, j, x, n;
  int r[14];
  int paik;
  int r_max;
  int suji[6];
  int b = max / 200;
  int d = max / 100;

  if (policy == POL_TOITOI || policy == POL_CHITOI)
    d *= 2;

  r_max = max;
  for (i = 0; i < 6; i++)
    suji[i] = b;

  n = hand[p].closed_num;
  for (i = 0; i < n; i++)
    {
      paik = P_KIND (hand[p].closed[i]);

      if (IS_PIN (paik))
        paik += K_MAN_1 - K_PIN_1;
      else if (IS_SOU (paik))
        paik += K_MAN_1 - K_SOU_1;

      if (paik == K_MAN_1 || paik == K_MAN_4)
        suji[0] += d;
      if (paik == K_MAN_2 || paik == K_MAN_5)
        suji[1] += d;
      if (paik == K_MAN_3 || paik == K_MAN_6)
        suji[2] += d;
      if (paik == K_MAN_4 || paik == K_MAN_7)
        suji[3] += d;
      if (paik == K_MAN_5 || paik == K_MAN_8)
        suji[4] += d;
      if (paik == K_MAN_6 || paik == K_MAN_9)
        suji[5] += d;
    }
  for (i = 0; i < 6; i++)
    {
      suji[i] *= suji[i];
    }

  for (i = 0; i < n; i++)
    {
      r[i] = 0;
      paik = P_KIND (hand[p].closed[i]);

      if (IS_PIN (paik))
        paik += K_MAN_1 - K_PIN_1;
      else if (IS_SOU (paik))
        paik += K_MAN_1 - K_SOU_1;

      switch (paik)
        {
        case K_MAN_1:
          r[i] = (suji[0]);
          break;
        case K_MAN_2:
          r[i] = (suji[1]);
          break;
        case K_MAN_3:
          r[i] = (suji[2]);
          break;
        case K_MAN_4:
          r[i] = (suji[3] + suji[0]);
          break;
        case K_MAN_5:
          r[i] = (suji[4] + suji[1]);
          break;
        case K_MAN_6:
          r[i] = (suji[5] + suji[2]);
          break;
        case K_MAN_7:
          r[i] = (suji[3]);
          break;
        case K_MAN_8:
          r[i] = (suji[4]);
          break;
        case K_MAN_9:
          r[i] = (suji[5]);
          break;
        default:
          r[i] = max / 10;
        }
      if (r[i] > r_max)
        r_max = r[i];
    }

  for (i = 0; i < hand[p].closed_num; i++)
    {
      if (r_max)
        x = (int) ((double) r[i] * max / r_max);
      else
        x = 0;

      *rp++ += x;
    }
}

static void
pnt_calc_kokushi (global_t *gp, int *r, int *rests)
{
  int n, i, pk;
  int kinds[K_MAX];

  n = hand[vself].closed_num;
  for (i = 0; i < K_MAX; i++)
    kinds[i] = 0;
  for (i = 0; i < n; i++)
    {
      kinds[P_KIND (hand[vself].closed[i])]++;
    }

  for (i = 0; i < n; i++)
    {
      pk = P_KIND (hand[vself].closed[i]);
      if (IS_YAO (pk))
        {
          if (kinds[pk] == 1)
            r[i] = 2000;
          else
            r[i] = (result_rest < rests[i]) ? 1000 : 500;
        }
      else
        {
          r[i] = (result_rest < rests[i]) ? 250 : 0;
        }
    }
}

static void
pnt_calc_chitoi (global_t *gp, int *r, int *rests)
{
  int n, i, x;
  int kinds[K_MAX];

  n = hand[vself].closed_num;
  for (i = 0; i < K_MAX; i++)
    kinds[i] = 0;
  for (i = 0; i < n; i++)
    {
      kinds[P_KIND (hand[vself].closed[i])]++;
    }
  for (i = 0; i < n; i++)
    {
      if (kinds[P_KIND (hand[vself].closed[i])] == 2)
        r[i] = 2000;
      else if (result_rest < rests[i])
        {
          r[i] = 1000;
        }
      else
        {
          x = mt_rest_kind (gp, P_KIND (hand[vself].closed[i]));
          r[i] = x * x * 60;
        }
    }
}

static void
pnt_calc (global_t *gp, int *r, int *rests)
{
  int n, x, i, j;
  pnt_t p;
  ahand_t *a;
  int pai_rest;
  int naki, pk;
  int v2_num;
  int kinds[K_MAX];
  int rest = result_rest;
  int yaku_fixed;

  for (i = 0; i < 14; i++)
    {
      result_pnt.pnt[i] = 0;
      result_pnt.pai[i] = 0;
    }
  yaku_fixed = 0;
  for (i = 0; i < 4; i++)
    {
      pk = P_KIND (hand[vself].opened[i][0]);
      if (pk == K_HAKU || pk == K_HATU || pk == K_CHUN ||
          pk == (K_TON + vself) || pk == (K_TON + big_turn))
        yaku_fixed = 1;
    }
  for (x = 0; x < result_cnt; x++)
    {
      a = result_ahand + x;
      p.pai_num = 0;

      v2_num = 0;
      for (i = 0; i < a->part_num; i++)
        {
          if (a->part_kind[i] == AH_V2)
            v2_num++;
        }
      for (i = 0; i < a->part_num; i++)
        {
          switch (a->part_kind[i])
            {
            case AH_H3:
              for (j = 0; j < 3; j++)
                {
                  p.pai[p.pai_num] = a->part[i][j];
                  p.pnt[p.pai_num++] = 1000;
                }
              break;
            case AH_V3:
              for (j = 0; j < 3; j++)
                {
                  p.pai[p.pai_num] = a->part[i][j];
                  p.pnt[p.pai_num++] = 1000;
                }
              break;
            case AH_V2:
              pai_rest = mt_rest_kind (gp, a->part[i][0]);
              for (j = 0; j < 2; j++)
                {
                  p.pai[p.pai_num] = a->part[i][j];
                  if (v2_num == 1)
                    p.pnt[p.pai_num++] = 500;
                  else
                    p.pnt[p.pai_num++] = 250 + 30 * pai_rest;
                }
              break;
            case AH_H2a:
              pai_rest = mt_rest_kind (gp, K_PREV2 (a->part[i][0]));
              pai_rest += mt_rest_kind (gp, K_NEXT2 (a->part[i][1]));
              for (j = 0; j < 2; j++)
                {
                  p.pai[p.pai_num] = a->part[i][j];
                  p.pnt[p.pai_num++] = 250 + 30 * pai_rest;
                }
              break;
            case AH_H2b:
              pai_rest = mt_rest_kind (gp, K_PREV2 (a->part[i][0]));
              pai_rest += mt_rest_kind (gp, K_NEXT2 (a->part[i][1]));
              for (j = 0; j < 2; j++)
                {
                  p.pai[p.pai_num] = a->part[i][j];
                  p.pnt[p.pai_num++] = 250 + 30 * pai_rest;
                }
              break;
            case AH_H2c:
              pai_rest = mt_rest_kind (gp, K_NEXT2 (a->part[i][0]));
              for (j = 0; j < 2; j++)
                {
                  p.pai[p.pai_num] = a->part[i][j];
                  p.pnt[p.pai_num++] = 250 + 30 * pai_rest;
                }
              break;
            }
        }
      for (i = 0; i < a->rest_num; i++)
        {
          int pnt;
          int pk_nn, pk_n, pk_pp, pk_p;
          int pk;

          pnt = 0;
          pk_p = K_PREV2 (a->rest[i]);
          pk_pp = K_PREV2 (pk_p);
          pk_n = K_NEXT2 (a->rest[i]);
          pk_nn = K_NEXT2 (pk_n);

          for (j = 0; j < a->part_num; j++)
            {
              switch (a->part_kind[j])
                {
                case AH_V2:
                  pk = a->part[j][0];
                  if (pk == pk_p || pk == pk_n || pk == pk_pp || pk == pk_nn)
                    pnt = 250;
                  break;
                case AH_H2a:
                case AH_H2b:
                case AH_H2c:
                  if (a->part[j][0] == a->rest[i] ||
                      a->part[j][1] == a->rest[i] ||
                      a->part[j][0] == pk_nn ||
                      a->part[j][0] == pk_n ||
                      a->part[j][1] == pk_p || a->part[j][1] == pk_pp)
                    pnt = 250;
                  break;
                }
            }
          p.pai[p.pai_num] = a->rest[i];
          if (pnt)
            {
              p.pnt[p.pai_num++] = pnt;
            }
          else
            {
              pai_rest = mt_rest_kind (gp, a->rest[i]) * 2;
              pai_rest += mt_rest_kind (gp, K_NEXT2 (a->rest[i]));
              pai_rest += mt_rest_kind (gp, K_PREV2 (a->rest[i]));
              p.pnt[p.pai_num++] = 30 * pai_rest;
            }
        }
      pnt_sort (&p);
      if (x == 0)
        bcopy (&p, &result_pnt, sizeof (pnt_t));
      else
        for (i = 0; i < p.pai_num; i++)
          result_pnt.pnt[i] += p.pnt[i];
    }

  n = hand[vself].closed_num;
  for (i = 0; i < n; i++)
    {
      for (j = 0; j < result_pnt.pai_num; j++)
        {
          if (P_KIND (hand[vself].closed[i]) == result_pnt.pai[j])
            break;
        }
      r[i] = result_pnt.pnt[j] / result_cnt;
    }
  naki = hand[vself].naki;

  switch (policy)
    {
    case POL_TANYAO:
      for (i = 0; i < n; i++)
        {
          if (IS_TAN (P_KIND (hand[vself].closed[i])))
            {
              r[i] += (naki ? 500 : 300);
            }
          else
            {
              r[i] += (naki ? 0 : 100);
            }
        }
      break;
    case POL_TOITOI:
      for (i = 0; i < K_MAX; i++)
        kinds[i] = 0;
      for (i = 0; i < n; i++)
        {
          kinds[P_KIND (hand[vself].closed[i])]++;
        }
      for (i = 0; i < n; i++)
        {
          pk = P_KIND (hand[vself].closed[i]);
          switch (kinds[pk])
            {
            case 1:
              r[i] += (mt_rest_kind (gp, pk) > 2) ? 100 : 0;
              break;
            case 2:
              r[i] += mt_rest_kind (gp, pk) ? 300 : 100;
              break;
            case 3:
            case 4:
              r[i] += 500;
              break;
            }
        }
    case POL_COLOR_MAN:
      for (i = 0; i < n; i++)
        {
          pk = P_KIND (hand[vself].closed[i]);
          if (IS_JI (pk) || IS_MAN (pk))
            {
              r[i] += 500;
            }
          else
            {
              r[i] += (naki ? 0 : 250);
            }
        }
      break;
    case POL_COLOR_PIN:
      for (i = 0; i < n; i++)
        {
          pk = P_KIND (hand[vself].closed[i]);
          if (IS_JI (pk) || IS_PIN (pk))
            {
              r[i] += 500;
            }
          else
            {
              r[i] += (naki ? 0 : 250);
            }
        }
      break;
    case POL_COLOR_SOU:
      for (i = 0; i < n; i++)
        {
          pk = P_KIND (hand[vself].closed[i]);
          if (IS_JI (pk) || IS_SOU (pk))
            {
              r[i] += 500;
            }
          else
            {
              r[i] += (naki ? 0 : 250);
            }
        }
      break;
    case POL_YAKU:
      if (!yaku_fixed)
        for (i = 0; i < n; i++)
          {
            pk = P_KIND (hand[vself].closed[i]);
            if (pk == K_HAKU || pk == K_HATU || pk == K_CHUN ||
                pk == (K_TON + vself) || pk == (K_TON + big_turn))
              r[i] += 500;
          }
      break;
    }
  for (i = 0; i < n; i++)
    {
      if (rests[i] > rest)
        {
          if (rest == 1)
            r[i] += 500;
          else if (rest == 2)
            r[i] += 300;
          else if (rest == 3)
            r[i] += 100;
        }
    }
}

static void
pnt_sort (pnt_t *p)
{
  int i, j;
  int t;
  for (i = 0; i < p->pai_num; i++)
    for (j = i; j < p->pai_num; j++)
      {
        if (p->pai[i] > p->pai[j] ||
            p->pai[i] == p->pai[j] && p->pnt[i] > p->pnt[j])
          {

            t = p->pai[i];
            p->pai[i] = p->pai[j];
            p->pai[j] = t;
            t = p->pnt[i];
            p->pnt[i] = p->pnt[j];
            p->pnt[j] = t;
          }
      }
}

int
analize_res (global_t *gp, int *howp)
{
  result_t y;
  int ret = R_ACK;
  *howp = 0;

  calc_myposition (gp);
  hand_insert (gp, vself, rvp->out);
  if (result_calc_yaku (gp, vself, &y) > 0)
    {
      ret = R_RON;
    }
  hand_delete (gp, vself, rvp->out, 0);

  if (ret == R_ACK && hand_can_kan (gp, vself, rvp->out) == 2)
    {
      ret = analize_kan (gp);
    }
  if (ret == R_ACK && hand_can_pon (gp, vself, rvp->out))
    {
      ret = analize_pon (gp);
    }
  if (ret == R_ACK && (cur_player + 1) % 4 == vself &&
      hand_can_tiex (gp, vself, rvp->out))
    {
      ret = analize_tie (gp, howp);
    }

  return ret;
}


static int
analize_kan (global_t *gp)
{
  int pk;
  hand_t save;
  ahand_t a;
  int rest1, rest2;
  int num1, num2;
  int pol1, pol2;

  save = hand[vself];
  ah_new (&a, &hand[vself], -1);
  ah_analize (&a, 0);
  pol1 = analize_policy (gp);
  num1 = result_cnt;
  rest1 = result_rest;

  if (pol1 == POL_CHITOI || pol1 == POL_KOKUSHI)
    return R_ACK;

  hand_kan (gp, vself, rvp->out, 0);
  ah_new (&a, &hand[vself], -1);
  ah_analize (&a, 0);
  pol2 = analize_policy (gp);
  num2 = result_cnt;
  rest2 = result_rest;
  hand[vself] = save;

  if (rest2 > 2 || random () % 1000 >= param_kan)
    return R_ACK;

  pk = P_KIND (rvp->out);
  if ((rest2 == rest1 && num1 == num2 && pol2 && pol2 > pol1)
      || (pk == K_HAKU || pk == K_HATU || pk == K_CHUN ||
          pk == (K_TON + vself) || pk == (K_TON + big_turn))
      || (has_yakupai (gp) && rest2 == rest1 && num1 == num2))
    return R_KAN;

  return R_ACK;
}


static int
analize_pon (global_t *gp)
{
  int pk;
  hand_t save;
  ahand_t a;
  int rest1, rest2;
  int pol1, pol2;

  save = hand[vself];
  ah_new (&a, &hand[vself], -1);
  ah_analize (&a, 0);
  pol1 = analize_policy (gp);
  rest1 = result_rest;

  if (pol1 == POL_CHITOI || pol1 == POL_KOKUSHI)
    return R_ACK;

  hand_pon (gp, vself, rvp->out, 0);
  analize (gp, vself);
  hand_delete (gp, vself, ask_pai, 0);

  ah_new (&a, &hand[vself], -1);
  ah_analize (&a, 0);
  pol2 = analize_policy (gp);
  rest2 = result_rest;

  hand[vself] = save;

  if (pol1 && ((pol2 >= pol1 && rest2 < rest1) || pol2 > pol1))
    return R_PON;

  pk = P_KIND (rvp->out);
  if ((random () % 1000 < param_pon || hand[vself].naki) &&
      ((pk == K_HAKU || pk == K_HATU || pk == K_CHUN ||
        pk == (K_TON + vself) || pk == (K_TON + big_turn))
       || (has_yakupai (gp) && rest2 < rest1)))
    return R_PON;

  return R_ACK;
}


static int
analize_tie (global_t *gp, int *howp)
{
  int i;
  hand_t save;
  ahand_t a;
  int how;
  int rest1, rest2;
  int pol1, pol2, tmp;
  int bad;

  save = hand[vself];
  ah_new (&a, &hand[vself], -1);
  ah_analize (&a, 0);
  pol1 = analize_policy (gp);
  rest1 = result_rest;

  if (pol1 == POL_CHITOI || pol1 == POL_KOKUSHI)
    return R_ACK;

  pol2 = 0;
  rest2 = rest1;

  for (i = 0; i < hand[vself].closed_num; i++)
    {
      bad = 0;
      how = hand[vself].closed[i];
      if (!hand_can_tie (gp, vself, rvp->out, how))
        continue;
      hand_tie (gp, vself, rvp->out, how);
      analize (gp, vself);
      hand_delete (gp, vself, ask_pai, 0);
      if (hand_check_kuikae (gp, vself, ask_pai))
        bad = 1;
      ah_new (&a, &hand[vself], -1);
      ah_analize (&a, 0);
      tmp = analize_policy (gp);
      hand[vself] = save;
      if (!bad && (result_rest < rest2 ||
                   (result_rest == rest2 && tmp > pol2)))
        {
          rest2 = result_rest;
          pol2 = tmp;
          *howp = how;
        }
    }

  if (pol1 && ((pol2 >= pol1 && rest2 < rest1) || pol2 > pol1))
    return R_TIE;

  if ((random () % 1000 < param_tie || hand[vself].naki)
      && has_yakupai (gp) && rest2 < rest1)
    return R_TIE;

  return R_ACK;
}

static void
filter_yomi (global_t *gp, int p, int max, int *rp)
{
  int i, j, x, n, m, paik, pp, reachf, off, tot, sute2_cnt = 0;
  river_t *riverp;
  int yp[K_MAX];
  int sute[K_MAX], sute2[K_MAX];
  int rest_kind[K_MAX];

  for (i = 1; i < K_MAX; i++)
    rest_kind[i] = mt_rest_kind (gp, i);
  n = hand[p].closed_num;
  for (i = 0; i < n; i++)
    {
      rest_kind[P_KIND (hand[p].closed[i])]--;
    }

  reach_count = 0;
  for (i = 0; i < K_MAX; i++)
    {
      yomi_pai[i] = sute[i] = sute2[i] = 0;
      yomi_rank[i] = i;
    }

  for (pp = 0; pp < 4; pp++)
    if (pp != p && (hand[pp].reach || naki_look (gp, pp)))
      {
        reach_count++;
        for (i = 0; i < 3; i++)
          {
            yp[K_MAN_1 + i * 9] = 250;
            yp[K_MAN_2 + i * 9] = 440;
            yp[K_MAN_3 + i * 9] = 700;
            yp[K_MAN_4 + i * 9] = 800;
            yp[K_MAN_5 + i * 9] = 870;
            yp[K_MAN_6 + i * 9] = 800;
            yp[K_MAN_7 + i * 9] = 700;
            yp[K_MAN_8 + i * 9] = 440;
            yp[K_MAN_9 + i * 9] = 250;
          }
        for (i = 0; i < 7; i++)
          {
            yp[K_TON + i] = 50;
          }
        x = rv_cur[pp];
        reachf = 0;
        for (i = 0; i <= x; i++)
          {
            paik = P_KIND (rv[pp][i].out);
            yp[paik] = 0;
            if (!reachf)
              sute[paik] = 1;
            if (T_GET (rv[pp][i].attr) == T_RV_RCH)
              reachf = 1;
            sute2[paik] = ++sute2_cnt;
          }
        for (i = 0; i < 3; i++)
          {
            off = i * 9;
            if (sute[K_MAN_1 + off])
              {
                yp[K_MAN_2 + off] *= 4.2 / 4.4;
                yp[K_MAN_3 + off] *= 6.8 / 7.0;
                yp[K_MAN_4 + off] *= 6.9 / 8.0;
                yp[K_MAN_5 + off] *= 9.0 / 8.7;
                yp[K_MAN_6 + off] *= 8.5 / 8.0;
              }
            if (sute[K_MAN_9 + off])
              {
                yp[K_MAN_8 + off] *= 4.2 / 4.4;
                yp[K_MAN_7 + off] *= 6.8 / 7.0;
                yp[K_MAN_6 + off] *= 6.9 / 8.0;
                yp[K_MAN_5 + off] *= 9.0 / 8.7;
                yp[K_MAN_4 + off] *= 8.5 / 8.0;
              }
            if (sute[K_MAN_2 + off])
              {
                yp[K_MAN_1 + off] *= 1.4 / 2.5;
                yp[K_MAN_3 + off] *= 5.9 / 7.0;
                yp[K_MAN_4 + off] *= 8.2 / 8.0;
                yp[K_MAN_5 + off] *= 7.5 / 8.7;
                yp[K_MAN_6 + off] *= 9.3 / 8.0;
              }
            if (sute[K_MAN_8 + off])
              {
                yp[K_MAN_9 + off] *= 1.4 / 2.5;
                yp[K_MAN_7 + off] *= 5.9 / 7.0;
                yp[K_MAN_6 + off] *= 8.2 / 8.0;
                yp[K_MAN_5 + off] *= 7.5 / 8.7;
                yp[K_MAN_4 + off] *= 9.3 / 8.0;
              }
            if (sute[K_MAN_3 + off])
              {
                yp[K_MAN_1 + off] *= 0.9 / 2.5;
                yp[K_MAN_2 + off] *= 1.9 / 4.4;
                yp[K_MAN_4 + off] *= 6.6 / 8.0;
                yp[K_MAN_5 + off] *= 8.6 / 8.7;
                yp[K_MAN_6 + off] *= 6.5 / 8.0;
              }
            if (sute[K_MAN_7 + off])
              {
                yp[K_MAN_9 + off] *= 0.9 / 2.5;
                yp[K_MAN_8 + off] *= 1.9 / 4.4;
                yp[K_MAN_6 + off] *= 6.6 / 8.0;
                yp[K_MAN_5 + off] *= 8.6 / 8.7;
                yp[K_MAN_4 + off] *= 6.5 / 8.0;
              }
            if (sute[K_MAN_4 + off])
              {
                yp[K_MAN_2 + off] *= 2.4 / 4.4;
                yp[K_MAN_3 + off] *= 3.5 / 7.0;
                yp[K_MAN_5 + off] *= 7.6 / 8.0;
                yp[K_MAN_6 + off] *= 8.0 / 8.7;
                yp[K_MAN_7 + off] *= 5.3 / 7.0;
              }
            if (sute[K_MAN_6 + off])
              {
                yp[K_MAN_8 + off] *= 2.4 / 4.4;
                yp[K_MAN_7 + off] *= 3.5 / 7.0;
                yp[K_MAN_5 + off] *= 7.6 / 8.0;
                yp[K_MAN_4 + off] *= 8.0 / 8.7;
                yp[K_MAN_3 + off] *= 5.3 / 7.0;
              }
            if (sute[K_MAN_5 + off])
              {
                yp[K_MAN_1 + off] *= 2.5 / 2.5;
                yp[K_MAN_2 + off] *= 1.9 / 4.4;
                yp[K_MAN_3 + off] *= 5.7 / 7.0;
                yp[K_MAN_4 + off] *= 5.4 / 8.0;
                yp[K_MAN_6 + off] *= 5.4 / 8.0;
                yp[K_MAN_7 + off] *= 5.7 / 7.0;
                yp[K_MAN_8 + off] *= 1.9 / 4.4;
                yp[K_MAN_9 + off] *= 2.5 / 2.5;
              }
          }
        for (i = 0; i < 3; i++)
          {
            off = i * 9;
            for (j = 0; j < 5; j++)
              {
                if (!sute2[K_MAN_1 + j + off]
                    && !sute2[K_MAN_2 + j + off] && !sute2[K_MAN_3 + j + off])
                  {
                    yp[K_MAN_2 + j + off] *= 1.2;
                  }
              }
            for (j = 0; j < 4; j++)
              {
                if (!sute2[K_MAN_1 + j + off]
                    && !sute2[K_MAN_2 + j + off]
                    && !sute2[K_MAN_3 + j + off] && !sute2[K_MAN_4 + j + off])
                  {
                    yp[K_MAN_1 + j + off] *= 1.3;
                    yp[K_MAN_4 + j + off] *= 1.3;
                  }
              }
          }
        for (i = 0; i < 3; i++)
          {
            off = i * 9;
            if (rest_kind[K_MAN_2 + off] == 0)
              {
                yp[K_MAN_1 + off] *= 0.3;
              }
            if (rest_kind[K_MAN_2 + off] == 1)
              {
                yp[K_MAN_1 + off] *= 0.6;
              }
            if (rest_kind[K_MAN_8 + off] == 0)
              {
                yp[K_MAN_9 + off] *= 0.3;
              }
            if (rest_kind[K_MAN_8 + off] == 1)
              {
                yp[K_MAN_9 + off] *= 0.6;
              }
            if (rest_kind[K_MAN_3 + off] == 0)
              {
                yp[K_MAN_1 + off] *= 0.3;
                yp[K_MAN_2 + off] *= 0.3;
              }
            if (rest_kind[K_MAN_3 + off] == 1)
              {
                yp[K_MAN_1 + off] *= 0.6;
                yp[K_MAN_2 + off] *= 0.6;
              }
            if (rest_kind[K_MAN_7 + off] == 0)
              {
                yp[K_MAN_9 + off] *= 0.3;
                yp[K_MAN_8 + off] *= 0.3;
              }
            if (rest_kind[K_MAN_7 + off] == 1)
              {
                yp[K_MAN_9 + off] *= 0.6;
                yp[K_MAN_8 + off] *= 0.6;
              }
            for (j = 0; j < 6; j++)
              {
                n = rest_kind[K_MAN_1 + j + off]
                  + rest_kind[K_MAN_4 + j + off];
                if (n < 4)
                  {
                    yp[K_MAN_1 + j + off] *= 1.3;
                    yp[K_MAN_4 + j + off] *= 1.3;
                  }
              }
          }
        for (i = K_TON; i < K_MAX; i++)
          {
            switch (rest_kind[i])
              {
              case 0:
                yp[i] = 0.0;
                break;
              case 1:
                yp[i] *= 0.3;
                break;
              case 2:
                yp[i] *= 1.0;
                break;
              case 3:
                yp[i] *= 3.0;
                break;
              case 4:
                yp[i] *= 10.0;
                break;
              }
          }
        for (i = 1; i < K_MAX; i++)
          {
            int tmp;
            tmp = yp[i];
            if (pp == 0)
              tmp = tmp * 1.5;
            if (vself == 0)
              tmp = tmp / 1.5;
            if (param_strategy && ALL_LAST)
              tmp = tmp * 1.5;
            yomi_pai[i] += tmp;
          }
      }

  riverp = rvp;
  if (reachf)
    while (riverp)
      {
        yomi_pai[P_KIND (riverp->out)] = 0;
        if (T_GET (riverp->attr) == T_RV_RCH)
          break;
        riverp = riverp->prev;
      }
  tot = 0;
  for (i = 1; i < K_MAX; i++)
    {
      tot += yomi_pai[i];
    }
  for (i = 1; i < K_MAX; i++)
    {
      if (tot)
        yomi_pai[i] *= 16120.0 * reach_count / tot;
      else
        yomi_pai[i] = 0;
    }

  n = hand[p].closed_num;
  for (i = 0; i < n; i++)
    {
      paik = P_KIND (hand[p].closed[i]);
      rp[i] += yomi_pai[paik] * max / 3000;
    }
  for (i = 1; i < K_MAX - 1; i++)
    for (j = 1; j < K_MAX - i; j++)
      {
        if (yomi_pai[yomi_rank[j]] < yomi_pai[yomi_rank[j + 1]])
          {
            int tmp;
            tmp = yomi_rank[j];
            yomi_rank[j] = yomi_rank[j + 1];
            yomi_rank[j + 1] = tmp;
          }
      }
}

static int
has_yakupai (global_t *gp)
{
  int i, n, pk, kinds[K_MAX];

  n = hand[vself].closed_num;
  for (i = 0; i < 4; i++)
    if (hand[vself].opened_kind[i])
      {
        pk = P_KIND (hand[vself].opened[i][0]);
        if (pk == K_HAKU || pk == K_HATU || pk == K_CHUN ||
            pk == (K_TON + vself) || pk == (K_TON + big_turn))
          return 1;
      }
  for (i = 0; i < K_MAX; i++)
    kinds[i] = 0;
  for (i = 0; i < n; i++)
    {
      kinds[P_KIND (hand[vself].closed[i])]++;
    }
  for (i = 0; i < n; i++)
    {
      pk = P_KIND (hand[vself].closed[i]);
      if (kinds[i] >= 3 &&
          (pk == K_HAKU || pk == K_HATU || pk == K_CHUN ||
           pk == (K_TON + vself) || pk == (K_TON + big_turn)))
        return 1;
    }
  return 0;
}

static char *
polstr (int n)
{
  switch (n)
    {
    case POL_NULL:
      return "POL_NULL";
    case POL_CHITOI:
      return "POL_CHITOI";
    case POL_KOKUSHI:
      return "POL_KOKUSHI";
    case POL_TOITOI:
      return "POL_TOITOI";
    case POL_TANYAO:
      return "POL_TANYAO";
    case POL_COLOR_MAN:
      return "POL_COLOR_MAN";
    case POL_COLOR_PIN:
      return "POL_COLOR_PIN";
    case POL_COLOR_SOU:
      return "POL_COLOR_SOU";
    case POL_YAKU:
      return "POL_YAKU";
    default:
      return "";
    }
}

static void
print_pai (global_t *gp)
{
  int i;
  if (df)
    {
      fprintf (df, "policy %s (%s %d y %d d %d)\n", polstr (policy),
               polstr (pol_kind), pol_point, yaku_point, dora_point);
      fprintf (df, "\t");
      for (i = 0; i < hand[vself].closed_num; i++)
        {
          fprintf (df, "%s  ", pai2str (hand[vself].closed[i]));
        }
      fprintf (df, "\n");
    }
}

static void
print_point (global_t *gp, char *name, int *r)
{
  int i;
  if (df)
    {
      fprintf (df, "%s\t", name);
      for (i = 0; i < hand[vself].closed_num; i++)
        {
          fprintf (df, "%-5d", r[i]);
        }
      fprintf (df, "\n");
    }
}

static int
naki_look (global_t *gp, int p)
{
  int i, j, k, n, d;
  int ret = 0;
  int toitoi = 0;
  int dora = 0;
  int yaku = 0;
  int naki = 0;
  int pk, pai;
  int col_man, col_sou, col_pin;

  col_man = col_sou = col_pin = 0;
  for (j = 0; j < 4; j++)
    {
      k = hand[p].opened_kind[j];
      if (!k)
        break;
      if (toitoi >= 0 &&
          (k == H_PON || k == H_KAN_CLOSED || k == H_KAN_OPENED))
        toitoi++;
      else
        toitoi = -1;
      n = 3;
      if (k == H_KAN_CLOSED || k == H_KAN_OPENED)
        n = 4;
      for (i = 0; i < n; i++)
        {
          pai = hand[p].opened[j][i];
          pk = P_KIND (pai);
          for (d = 0; d < 5; d++)
            {
              if (mt_dora (gp, d) == pk)
                dora++;
            }
          if (!RL_NOAKADORA && pai == K_MAN_5 * 4)
            dora++;
          if (!RL_NOAKADORA && pai == K_PIN_5 * 4)
            dora++;
          if (!RL_NOAKADORA && pai == K_SOU_5 * 4)
            dora++;
        }
      if (pk == K_HAKU || pk == K_HATU || pk == K_CHUN
          || pk == K_TON + big_turn || pk == K_TON + p)
        yaku++;
      if (pk == K_TON + big_turn && pk == K_TON + p)
        yaku++;
      if (col_man >= 0 && (IS_JI (pk) || IS_MAN (pk)))
        col_man++;
      else
        col_man = -1;
      if (col_pin >= 0 && (IS_JI (pk) || IS_PIN (pk)))
        col_pin++;
      else
        col_pin = -1;
      if (col_sou >= 0 && (IS_JI (pk) || IS_SOU (pk)))
        col_sou++;
      else
        col_sou = -1;
    }
  if (dora + yaku >= 3)
    ret++;
  if (toitoi >= 3 && dora + yaku >= 1)
    ret++;
  if (col_man >= 3 && (dora + yaku >= 1 || toitoi >= 3))
    ret++;
  if (col_pin >= 3 && (dora + yaku >= 1 || toitoi >= 3))
    ret++;
  if (col_sou >= 3 && (dora + yaku >= 1 || toitoi >= 3))
    ret++;
  return ret;
}
