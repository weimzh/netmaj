/*
 * I) 役に関するもの
 */

#ifndef RULEDEF_H
#define RULEDEF_H

/* ピンヅモなし: つもったときは、ピンフがつかない */
extern int RL_NOPINTUMO;

/* 赤ドラなし: 赤ドラは、ドラ扱いにしない */
extern int RL_NOAKADORA;

/* なきタンなし: ないた時はタンヤオがつかない */
extern int RL_NONAKITAN;

/* カンウラなし: ウラドラは１枚のみ */
extern int RL_NOKANURA;

/*
 * II) ゲーム運びに関するもの
 */

extern int tonpu;

/* とびなし: 最後までゲームをする (あがりやめもなし/箱下計算もあり) */
extern int RL_NOFLY;

/* 箱下計算あり: 0 点以下の分を 0 点扱いにせず計算する */
extern int RL_UNDER;

/* ゲーム開始時の持ち点 */
extern int RL_STARTPOINT;       /* 25000 */

/* 頭ハネあり(ダブロンなし) */
extern int RL_ATAMAHANE;

/* 三家和なし（頭ハネなしの場合はトリロンあり）*/
extern int RL_NOSANCYAHO;

/* 子の 7700/親の11600 は、満貫扱い */
extern int RL_77MANGAN;

/* 喰い替えなし: 喰ったとき、喰った牌と喰い替えになる牌は切れない */
extern int RL_NOKUIKAE;

/* 包（パオ）あり: 大三元、大四喜を確定させると責任払い */
extern int RL_PAO;

/*
 * III) 御祝儀(チップ)に関するもの
 */


/* 役満御祝儀の枚数  */
extern int RLC_YAKUMAN;         /* 2 */

/* 一発御祝儀の枚数  */
extern int RLC_IPPATU;          /* 1 */

/* ウラドラ御祝儀の枚数  */
extern int RLC_URADORA;         /* 1 */

/* 赤ドラ御祝儀の枚数 */
extern int RLC_AKADORA;         /* 1 */

/* 赤ドラは面前のみ: ないた時は御祝儀がつかない */
extern int RLC_NONAKIAKA;

/* オールスターなし: 赤ドラを３枚集めたときの御祝儀(2枚)なし */
extern int RLC_NOALLSTARS;

/*
 * IV) ウマ(順位点) に関するもの
 */

typedef int uma_t[12];

/* ウマなし */
static uma_t RLU_NONE = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/* ゴットー */
static uma_t RLU_5_10 = { 10, 5, -5, -10, 10, 5, -5, -10, 10, 5, -5, -10 };

/* ワンツー */
static uma_t RLU_10_20 =
  { 20, 10, -10, -20, 20, 10, -10, -20, 20, 10, -10, -20 };
/* ワンスリー */
static uma_t RLU_10_30 =
  { 30, 10, -10, -30, 30, 10, -10, -30, 30, 10, -10, -30 };
/* シチゴサン */
static uma_t RLU_7_5_3 = { 15, -3, -5, -7, 10, 5, -5, -10, 7, 5, 3, -15 };

/* 沈みウマ */
static uma_t RLU_UNDER =
  { 30, -10, -10, -10, 20, 0, -10, -10, 10, 0, 0, -10 };

extern uma_t uma;

#endif
