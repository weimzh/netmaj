/*
 * NETMAJ - network mahjongg -
 * Copyright (C) 1994, 1995 Koji Suzuki (suz@d2.bs1.fc.nec.co.jp)
 *
 * NETMAJ is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * NETMAJ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * 5-7-2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"

void
result_cvt_to_int (result_t *yp, int *iy)
{
  iy[0] = 0;
  iy[1] = 0;
  iy[2] = 0;
  iy[3] = 0;

  if (yp->yakuman)
    Y_SET (iy, 0);
  if (yp->kokusi)
    Y_SET (iy, 1);
  if (yp->daisangen)
    Y_SET (iy, 2);
  if (yp->suankou)
    Y_SET (iy, 3);
  if (yp->sukantu)
    Y_SET (iy, 4);
  if (yp->syosusi)
    Y_SET (iy, 5);
  if (yp->daisusi)
    Y_SET (iy, 6);
  if (yp->tuisou)
    Y_SET (iy, 7);
  if (yp->ryuisou)
    Y_SET (iy, 8);
  if (yp->cyurenpotou)
    Y_SET (iy, 9);
  if (yp->tenhou)
    Y_SET (iy, 10);
  if (yp->chihou)
    Y_SET (iy, 11);
  if (yp->chinraotou)
    Y_SET (iy, 12);
  if (yp->renhou)
    Y_SET (iy, 13);
  if (yp->nagasimangan)
    Y_SET (iy, 14);
  if (yp->double_reach)
    Y_SET (iy, 15);
  if (yp->reach)
    Y_SET (iy, 16);
  if (yp->ippatu)
    Y_SET (iy, 17);
  if (yp->tumo)
    Y_SET (iy, 18);
  if (yp->pinfu)
    Y_SET (iy, 19);
  if (yp->tanyao)
    Y_SET (iy, 20);
  if (yp->iipeikou)
    Y_SET (iy, 21);
  if (yp->haitei)
    Y_SET (iy, 22);
  if (yp->jikaze)
    Y_SET (iy, 23);
  if (yp->bakaze)
    Y_SET (iy, 24);
  if (yp->haku)
    Y_SET (iy, 25);
  if (yp->hatu)
    Y_SET (iy, 26);
  if (yp->chun)
    Y_SET (iy, 27);
  if (yp->chankan)
    Y_SET (iy, 28);
  if (yp->rinshan)
    Y_SET (iy, 29);
  if (yp->sansiki_naki)
    Y_SET (iy, 30);
  if (yp->ittu_naki)
    Y_SET (iy, 31);
  if (yp->chanta_naki)
    Y_SET (iy, 32);
  if (yp->sansiki)
    Y_SET (iy, 33);
  if (yp->ittu)
    Y_SET (iy, 34);
  if (yp->chanta)
    Y_SET (iy, 35);
  if (yp->sanankou)
    Y_SET (iy, 36);
  if (yp->sankantu)
    Y_SET (iy, 37);
  if (yp->toitoi)
    Y_SET (iy, 38);
  if (yp->syosangen)
    Y_SET (iy, 39);
  if (yp->honraotou)
    Y_SET (iy, 40);
  if (yp->sansiki_doukoku)
    Y_SET (iy, 41);
  if (yp->honitu_naki)
    Y_SET (iy, 42);
  if (yp->junchan_naki)
    Y_SET (iy, 43);
  if (yp->honitu)
    Y_SET (iy, 44);
  if (yp->junchan)
    Y_SET (iy, 45);
  if (yp->ryanpeikou)
    Y_SET (iy, 46);
  if (yp->chinitu_naki)
    Y_SET (iy, 47);
  if (yp->chinitu)
    Y_SET (iy, 48);
  if (yp->chitoitu)
    Y_SET (iy, 49);
  if (yp->furiten)
    Y_SET (iy, 50);
  if (yp->doujun)
    Y_SET (iy, 51);
  if (yp->kyusukyupai)
    Y_SET (iy, 52);
  if (yp->sufurenta)
    Y_SET (iy, 53);
  if (yp->sukannagare)
    Y_SET (iy, 54);
  if (yp->sancyahou)
    Y_SET (iy, 55);
  if (yp->noten_reach)
    Y_SET (iy, 56);
}

void
result_cvt_from_int (result_t *yp, int *iy)
{
  yp->yakuman = Y_GET (iy, 0);
  yp->kokusi = Y_GET (iy, 1);
  yp->daisangen = Y_GET (iy, 2);
  yp->suankou = Y_GET (iy, 3);
  yp->sukantu = Y_GET (iy, 4);
  yp->syosusi = Y_GET (iy, 5);
  yp->daisusi = Y_GET (iy, 6);
  yp->tuisou = Y_GET (iy, 7);
  yp->ryuisou = Y_GET (iy, 8);
  yp->cyurenpotou = Y_GET (iy, 9);
  yp->tenhou = Y_GET (iy, 10);
  yp->chihou = Y_GET (iy, 11);
  yp->chinraotou = Y_GET (iy, 12);
  yp->renhou = Y_GET (iy, 13);
  yp->nagasimangan = Y_GET (iy, 14);
  yp->double_reach = Y_GET (iy, 15);
  yp->reach = Y_GET (iy, 16);
  yp->ippatu = Y_GET (iy, 17);
  yp->tumo = Y_GET (iy, 18);
  yp->pinfu = Y_GET (iy, 19);
  yp->tanyao = Y_GET (iy, 20);
  yp->iipeikou = Y_GET (iy, 21);
  yp->haitei = Y_GET (iy, 22);
  yp->jikaze = Y_GET (iy, 23);
  yp->bakaze = Y_GET (iy, 24);
  yp->haku = Y_GET (iy, 25);
  yp->hatu = Y_GET (iy, 26);
  yp->chun = Y_GET (iy, 27);
  yp->chankan = Y_GET (iy, 28);
  yp->rinshan = Y_GET (iy, 29);
  yp->sansiki_naki = Y_GET (iy, 30);
  yp->ittu_naki = Y_GET (iy, 31);
  yp->chanta_naki = Y_GET (iy, 32);
  yp->sansiki = Y_GET (iy, 33);
  yp->ittu = Y_GET (iy, 34);
  yp->chanta = Y_GET (iy, 35);
  yp->sanankou = Y_GET (iy, 36);
  yp->sankantu = Y_GET (iy, 37);
  yp->toitoi = Y_GET (iy, 38);
  yp->syosangen = Y_GET (iy, 39);
  yp->honraotou = Y_GET (iy, 40);
  yp->sansiki_doukoku = Y_GET (iy, 41);
  yp->honitu_naki = Y_GET (iy, 42);
  yp->junchan_naki = Y_GET (iy, 43);
  yp->honitu = Y_GET (iy, 44);
  yp->junchan = Y_GET (iy, 45);
  yp->ryanpeikou = Y_GET (iy, 46);
  yp->chinitu_naki = Y_GET (iy, 47);
  yp->chinitu = Y_GET (iy, 48);
  yp->chitoitu = Y_GET (iy, 49);
  yp->furiten = Y_GET (iy, 50);
  yp->doujun = Y_GET (iy, 51);
  yp->kyusukyupai = Y_GET (iy, 52);
  yp->sufurenta = Y_GET (iy, 53);
  yp->sukannagare = Y_GET (iy, 54);
  yp->sancyahou = Y_GET (iy, 55);
  yp->noten_reach = Y_GET (iy, 56);
}


#ifdef ASCII_MESSAGE

char *result_str[Y_MAX] = {
  "LIMIT",
  "Thirteen orphans",
  "Big tree dragons",
  "Four concealed triplets",
  "Four kongs",
  "Little four winds",
  "Big four winds",
  "All honors",
  "All green",
  "Nine gates",
  "Heavenly hand",
  "Earthly hand",
  "All terminals",
  "Humanly hand(Half limit)?",
  "All terminals/honors with river",
  "Double reach",
  "Reach",
  "One chance?",
  "Self drawn",
  "No Points",
  "All simples",
  "Same sequence?",
  "Last tile",
  "Own wind",
  "Prevailing wind",
  "White dragon",
  "Green dragon",
  "Red dragon",
  "Robbing a kong",
  "Supplemental Tile",
  "Same sequence with all suits?(1)",
  "3 consecutive sequences(1)",
  "Terminal/honor in each set(1)",
  "Same sequence with all suits?(2)",
  "3 consecutive sequences(2)",
  "3 consecutive sequences(2)",
  "3 concealed triplets(2)",
  "3 kongs(2)",
  "4 triplets(2)",
  "Little three dragons(2)",
  "All terminals/honors",
  "Same triplets with all suits",
  "One suit with honors(2)",
  "Terminal in each set(2)",
  "One suit with honors(3)",
  "Terminal in each set(3)",
  "2 same sequence(3)",
  "One suit only(5)",
  "One suit only(6)",
  "7 pairs(2)",
  "furiten?",
  "doujun?",
  "9 terminals/honors in initial hand",
  "4 same wind in first discard",
  "Four kongs",
  "3 players go",
  "not ready reach",
};
#else

char *result_str[Y_MAX] = {
  "役満",
  "国士無双",
  "大三元",
  "四暗刻",
  "四槓子",
  "小四喜",
  "大四喜",
  "字一色",
  "緑一色",
  "九連宝燈",
  "天和",
  "地和",
  "清老頭",
  "人和(倍満)",
  "流し満貫",
  "ダブリー",
  "リーチ",
  "一発",
  "ツモ",
  "平和",
  "タンヤオ",
  "イイペイコ",
  "ハイテイ",
  "自風",
  "場風",
  "白",
  "発",
  "中",
  "チャンカン",
  "リンシャン",
  "三色(1)",
  "一通(1)",
  "チャンタ(1)",
  "三色(2)",
  "一通(2)",
  "チャンタ(2)",
  "三暗刻(2)",
  "三槓子(2)",
  "対々(2)",
  "小三元(2)",
  "混老頭",
  "三色同刻",
  "混一(2)",
  "純チャン(2)",
  "混一(3)",
  "純チャン(3)",
  "二ペイコウ(3)",
  "清一(5)",
  "清一(6)",
  "七対子(2)",
  "フリテン",
  "同巡",
  "九種九牌",
  "四風連打",
  "四槓流れ",
  "三家和",
  "ノーテンリーチ",
};
#endif
