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
#include "ruledef.h"

int tonpu = 0;
int RL_NOPINTUMO = 0;
int RL_NOAKADORA = 1;
int RL_NONAKITAN = 0;
int RL_NOKANURA = 0;
int RL_NOFLY = 0;
int RL_UNDER = 1;
int RL_ATAMAHANE = 1;
int RL_77MANGAN = 0;
int RL_NOKUIKAE = 0;
int RL_NOSANCYAHO = 0;
int RL_PAO = 0;
int RLC_NONAKIAKA = 0;
int RLC_NOALLSTARS = 0;
int RLC_IPPATU = 1;
int RLC_URADORA = 1;
int RLC_AKADORA = 1;
int RLC_YAKUMAN = 2;
int RL_STARTPOINT = 25000;
uma_t uma = { 20, 10, -10, -20, 20, 10, -10, -20, 20, 10, -10, -20 };

static int expand4[16] =
  { 0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1 };

static void
uma_set (uma_t a, uma_t b)
{
  int i;
  for (i = 0; i < 12; i++)
    {
      a[i] = b[i];
    }
}

static int
uma_cmp (uma_t a, uma_t b)
{
  int i;
  for (i = 0; i < 12; i++)
    {
      if (a[i] != b[i])
        return 1;
    }
  return 0;
}

void
expand_rule (int *r1)
{
  RL_NOPINTUMO = (*r1 & 0x00000001) ? 1 : 0;
  RL_NOAKADORA = (*r1 & 0x00000002) ? 1 : 0;
  RL_NONAKITAN = (*r1 & 0x00000004) ? 1 : 0;
  RL_NOKANURA = (*r1 & 0x00000008) ? 1 : 0;
  RL_NOFLY = (*r1 & 0x00000010) ? 1 : 0;
  RL_UNDER = (*r1 & 0x00000020) ? 1 : 0;

  RLC_NONAKIAKA = (*r1 & 0x00000040) ? 1 : 0;
  RLC_NOALLSTARS = (*r1 & 0x00000080) ? 1 : 0;

  RLC_IPPATU = (*r1 & 0x00000100) ? 0 : 1;
  RLC_URADORA = (*r1 & 0x00000200) ? 0 : 1;
  RLC_AKADORA = (*r1 & 0x00000400) ? 0 : 1;
  tonpu = (*r1 & 0x00000800) ? 0 : 1;
  RLC_YAKUMAN = expand4[(*r1 & 0x0000f000) >> 12] + 2;
  RL_STARTPOINT = (expand4[(*r1 & 0x000f0000) >> 16] + 25) * 1000;
  switch ((*r1 & 0x00f00000) >> 20)
    {
    case 0:
      uma_set (uma, RLU_10_20);
      break;
    case 1:
      uma_set (uma, RLU_10_30);
      break;
    case 2:
      uma_set (uma, RLU_5_10);
      break;
    case 3:
      uma_set (uma, RLU_7_5_3);
      break;
    case 4:
      uma_set (uma, RLU_UNDER);
      break;
    case 5:
      uma_set (uma, RLU_NONE);
      break;
    }
  RL_ATAMAHANE = (*r1 & 0x01000000) ? 1 : 0;
  RL_77MANGAN = (*r1 & 0x02000000) ? 1 : 0;
  RL_NOKUIKAE = (*r1 & 0x04000000) ? 1 : 0;
  RL_PAO = (*r1 & 0x08000000) ? 1 : 0;
  RL_NOSANCYAHO = (*r1 & 0x10000000) ? 1 : 0;
}

void
pack_rule (int *r1)
{
  *r1 = 0;
  if (RL_NOPINTUMO)
    *r1 |= 0x00000001;
  if (RL_NOAKADORA)
    *r1 |= 0x00000002;
  if (RL_NONAKITAN)
    *r1 |= 0x00000004;
  if (RL_NOKANURA)
    *r1 |= 0x00000008;
  if (RL_NOFLY)
    *r1 |= 0x00000010;
  if (RL_UNDER)
    *r1 |= 0x00000020;
  if (RLC_NONAKIAKA)
    *r1 |= 0x00000040;
  if (RLC_NOALLSTARS)
    *r1 |= 0x00000080;
  if (!RLC_IPPATU)
    *r1 |= 0x00000100;
  if (!RLC_URADORA)
    *r1 |= 0x00000200;
  if (!RLC_AKADORA)
    *r1 |= 0x00000400;
  if (!tonpu)
    *r1 |= 0x00000800;
  *r1 |= (RLC_YAKUMAN - 2 & 0xf) << 12;
  *r1 |= ((RL_STARTPOINT / 1000 - 25) & 0xf) << 16;
  if (!uma_cmp (uma, RLU_10_30))
    *r1 |= 0x00100000;
  if (!uma_cmp (uma, RLU_5_10))
    *r1 |= 0x00200000;
  if (!uma_cmp (uma, RLU_7_5_3))
    *r1 |= 0x00300000;
  if (!uma_cmp (uma, RLU_UNDER))
    *r1 |= 0x00400000;
  if (!uma_cmp (uma, RLU_NONE))
    *r1 |= 0x00500000;
  if (RL_ATAMAHANE)
    *r1 |= 0x01000000;
  if (RL_77MANGAN)
    *r1 |= 0x02000000;
  if (RL_NOKUIKAE)
    *r1 |= 0x04000000;
  if (RL_PAO)
    *r1 |= 0x08000000;
  if (RL_NOSANCYAHO)
    *r1 |= 0x10000000;
}

void
rule_init (global_t *gp)
{
  char buf[20];

  buf[0] = 0;
  pf_param (gp, "tonpu", &tonpu);
  pf_param (gp, "nopintumo", &RL_NOPINTUMO);
  pf_param (gp, "noakadora", &RL_NOAKADORA);
  pf_param (gp, "nonakitan", &RL_NONAKITAN);
  pf_param (gp, "nokanura", &RL_NOKANURA);
  pf_param (gp, "nofly", &RL_NOFLY);
  pf_param (gp, "under", &RL_UNDER);
  pf_param (gp, "startpoint", &RL_STARTPOINT);
  pf_param (gp, "yakuman-chip", &RLC_YAKUMAN);
  pf_param (gp, "ippatu-chip", &RLC_IPPATU);
  pf_param (gp, "uradora-chip", &RLC_URADORA);
  pf_param (gp, "akadora-chip", &RLC_AKADORA);
  pf_param (gp, "nonakiaka", &RLC_NONAKIAKA);
  pf_param (gp, "noallstars", &RLC_NOALLSTARS);
  pf_param_strn (gp, "uma", buf, 20);
  if (!strcmp (buf, "none"))
    uma_set (uma, RLU_NONE);
  else if (!strcmp (buf, "5-10"))
    uma_set (uma, RLU_5_10);
  else if (!strcmp (buf, "10-20"))
    uma_set (uma, RLU_10_20);
  else if (!strcmp (buf, "10-30"))
    uma_set (uma, RLU_10_30);
  else if (!strcmp (buf, "7-5-3"))
    uma_set (uma, RLU_7_5_3);
  else if (!strcmp (buf, "under"))
    uma_set (uma, RLU_UNDER);
  pf_param (gp, "atamahane", &RL_ATAMAHANE);
  pf_param (gp, "77mangan", &RL_77MANGAN);
  pf_param (gp, "nokuikae", &RL_NOKUIKAE);
  pf_param (gp, "pao", &RL_PAO);
  pf_param (gp, "nosancyaho", &RL_NOSANCYAHO);
}

char *
rule_message ()
{
  static char buf[1024];
  sprintf (buf, "\n\
%s ウマは%s\n\
%d 持ち / 30000点 返し\n\
なきタン%s 赤ドラ%s ピンヅモ%s カンウラ%s\n\
とび%s 箱下計算%s 頭ハネ%s %s\n\
子の7700/親の11600を満貫扱いに%s\n\
喰い替え%s 包（パオ） %s\n\
役満御祝儀 %d 枚オール (放縦は %d 枚)\n\
一発御祝儀%s ウラドラ御祝儀%s 赤ドラ御祝儀%s\n\
赤ドラ御祝儀は%s\n\
オールスター(赤ドラ３枚)は%s\n", tonpu ? "東風戦" : "東南回し", (uma_cmp (uma, RLU_NONE) == 0) ? "なし" : (uma_cmp (uma, RLU_5_10) == 0) ? "ゴットー" : (uma_cmp (uma, RLU_10_20) == 0) ? "ワンツー" : (uma_cmp (uma, RLU_10_30) == 0) ? "ワンスリー" : (uma_cmp (uma, RLU_7_5_3) == 0) ? "シチゴサン" : (uma_cmp (uma, RLU_UNDER) == 0) ? "沈みウマ" : "?", RL_STARTPOINT, RL_NONAKITAN ? "なし" : "あり", RL_NOAKADORA ? "なし" : "あり", RL_NOPINTUMO ? "なし" : "あり", RL_NOKANURA ? "なし" : "あり", RL_NOFLY ? "なし" : "あり", RL_UNDER ? "あり" : "なし", RL_ATAMAHANE ? "あり" : "なし", RL_NOSANCYAHO ? RL_ATAMAHANE ? "三家和なし" : "トリロンあり" : "三家和あり", RL_77MANGAN ? "する" : "しない", RL_NOKUIKAE ? "なし" : "あり", RL_PAO ? "あり" : "なし", RLC_YAKUMAN, RLC_YAKUMAN * 3 / 2, RLC_IPPATU ? "あり" : "なし", RLC_URADORA ? "あり" : "なし", RLC_AKADORA ? "あり" : "なし", RLC_NONAKIAKA ? "面前のみ" : "ないても付く", RLC_NOALLSTARS ? "なし" : "あり(チップ２枚増し)");
  return buf;
}
