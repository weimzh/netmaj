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
 * testpai.c -- 
 *
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu May 11 10:27:04 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Mon Jan  8 18:00:03 1996
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */
#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"
#include "ruledef.h"

static widget_t rule;
static widget_t rules[20];

void
ruleview_init ()
{
  widget_t frames[7], canvas, framepoint;
  integer_t i, x, y;
  extern integer_t xwidth, xheight;
  extern widget_t root_widget;
  extern integer_t xwidth, xheight;
  extern integer_t red;

  rule = widget_create ("frame", root_widget);
  widget_configure (rule, resource_name, "rule");
  widget_set_geometry (rule, xwidth, xheight);

  framepoint = create_basic_info_frame ("frame",
                                        rule, frames, PAI_W * 18, 0, 1);
  widget_configure (frames[4], resource_frame_start, 270);
  widget_get_geometry (framepoint, &x, &y);
  widget_set_geometry (frames[0], xwidth, 5);

  if (PAICARD3D || BUILD4)
    {
      text_create (framepoint, xwidth, 54,
                   "    NETMAJ Localrule infomation", red, XLARGETIMESIB, 1);
    }
  for (i = 0; i < 20; i++)
    {
      rules[i] = canvas = widget_create ("text", framepoint);
      widget_set_geometry (canvas, xwidth, 18);
      widget_configure (canvas, resource_textfont, MEDIUMJP);
    }

  widget_pack (rule);
  widget_fix (rule);

  widget_set_geometry (rule, xwidth, xheight);
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

static void
rule_draw ()
{
  char *p;
  char buffer[128];
  int i = 0;
  if (!rule)
    ruleview_init ();

  sprintf (p =
           buffer, "%s、%d 点持ちの 30000 点返し",
           tonpu ? "東風戦" : "東南戦", RL_STARTPOINT);
  widget_configure (rules[i++], resource_text, p);

  sprintf (p = buffer, "ウマ: %s", (uma_cmp (uma, RLU_NONE) == 0) ? "なし"
           : (uma_cmp (uma, RLU_5_10) == 0) ? "ゴットー"
           : (uma_cmp (uma, RLU_10_20) == 0) ? "ワンツー"
           : (uma_cmp (uma, RLU_10_30) == 0) ? "ワンスリー"
           : (uma_cmp (uma, RLU_7_5_3) == 0) ? "シチゴサン"
           : (uma_cmp (uma, RLU_UNDER) == 0) ? "沈みウマ" : "?");
  widget_configure (rules[i++], resource_text, p);


  if (RL_NOPINTUMO)
    {
      p = "ピンヅモなし: つもったときは、ピンフがつかない";
    }
  else
    {
      p = "ピンヅモあり: つもったとき、ピンフがつく";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RL_NOAKADORA)
    {
      p = "赤ドラなし: 赤ドラは、ドラ扱いにしない";
    }
  else
    {
      p = "赤ドラあり: 赤ドラは、ドラ扱いする";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RL_NONAKITAN)
    {
      p = "なきタンなし: ないた時はタンヤオがつかない";
    }
  else
    {
      p = "なきタンあり: ないた時もタンヤオがつく";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RL_NOKANURA)
    {
      p = "カンウラなし: ウラドラは１枚のみ";
    }
  else
    {
      p = "カンウラあり: カンウラもウラドラとする";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RL_NOFLY)
    {
      p = "とびなし: あがりやめもなし/箱下計算もあり";
    }
  else
    {
      p = "とびあり: あがりやめもあり";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RL_UNDER)
    {
      p = "箱下計算あり: 0 点以下の分を 0 点扱いにせず計算する";
    }
  else
    {
      p = "箱下計算なし: 0 点以下の分を 0 点扱いに計算する";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RL_77MANGAN)
    {
      p = "子の 7700/親の11600 は、満貫扱い";
    }
  else
    {
      p = "子の 7700/親の11600 は、満貫扱いにしない";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RL_NOKUIKAE)
    {
      p = "喰い替えなし";
    }
  else
    {
      p = "喰い替えあり";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RL_PAO)
    {
      p = "包（パオ）あり: 大三元、大四喜を確定させると責任払い ";
    }
  else
    {
      p = "包（パオ）なし: 責任払いなし ";
    }
  widget_configure (rules[i++], resource_text, p);


  if (RL_ATAMAHANE && RL_NOSANCYAHO)
    {
      p = "頭ハネあり 三家和なし";
    }
  else if (RL_ATAMAHANE && !RL_NOSANCYAHO)
    {
      p = "頭ハネあり 三家和あり";
    }
  else if (!RL_ATAMAHANE && RL_NOSANCYAHO)
    {
      p = "トリロンあり 三家和なし";
    }
  else
    {
      p = "ダブロンあり 三家和あり";
    }
  widget_configure (rules[i++], resource_text, p);

  sprintf (p = buffer, "役満御祝儀の枚数  %d 枚", RLC_YAKUMAN);
  widget_configure (rules[i++], resource_text, p);

  sprintf (p = buffer, "一発御祝儀の枚数 %d 枚", RLC_IPPATU);
  widget_configure (rules[i++], resource_text, p);

  sprintf (p = buffer, "ウラドラ御祝儀の枚数 %d 枚", RLC_URADORA);
  widget_configure (rules[i++], resource_text, p);

  sprintf (p = buffer, "赤ドラ御祝儀の枚数 %d 枚", RLC_AKADORA);
  widget_configure (rules[i++], resource_text, p);

  if (RLC_NONAKIAKA)
    {
      p = "赤ドラは面前のみ御祝儀: ないた時は御祝儀がつかない";
    }
  else
    {
      p = "赤ドラは常に御祝儀がつく";
    }
  widget_configure (rules[i++], resource_text, p);

  if (RLC_NOALLSTARS)
    {
      p = "オールスターなし: 赤ドラを３枚集めたときの御祝儀(2枚)なし";
    }
  else
    {
      p = "オールスターあり: 赤ドラを３枚集めたときの御祝儀(2枚)あり";
    }
  widget_configure (rules[i++], resource_text, p);

  widget_map (rule);

  widget_display (rule);
  widget_flush (rule);
  wait_key ();
}

void
rule_callback callbackarg_param
{
  extern widget_t board;
  extern widget_t about;

  if (board)
    widget_unmap (board);
  if (about)
    widget_unmap (about);

  rule_draw ();

  widget_unmap (rule);
  board_redraw (1);
}

void
rule_notify ()
{
  pixpai_mode (RL_NOAKADORA == 1 && RLC_AKADORA == 0);
}
