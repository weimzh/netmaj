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

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "comm.h"
#include "ss.h"
#include "pai.h"
#include "ahand.h"
#include "result.h"

#ifdef DEF_OPTARG
extern char *optarg;
extern int optind;
#endif

#ifdef USE_RAND
#define random	rand
#define srandom srand
#endif

#ifndef USE_BCOPY
#define bcopy(s,d,n)	memcpy(d,s,n)
#define bzero(d,n)	memset(d,0,n)
#endif

#ifdef SYSV
#define index 	strchr
#define rindex 	strrchr
#endif
/* extern char *index();
 * extern char *rindex(); */

extern FILE *plog;              /* protocol logging */

#ifndef	NETMAJ_LIB
#define NETMAJ_LIB	"/usr/local/lib/netmaj"
#endif /* NETMAJ_LIB */

/* game status */

#define PNAME_MAX	10
typedef struct player
{
  int rpoint;                   /* result point */
  int tpoint;                   /* chip count */
  int gpoint;                   /* game point */
  int ppoint;                   /* play point */
  int flags;                    /* player attribute */
  char name[PNAME_MAX];         /* player name */
  SOCKETIO *sp;                 /* connection */
} player_t;

/* attribute of player */
#define M_SUPER		001
#define M_TALK		002

typedef struct river
{
  pai_t in;                     /* get pai */
  pai_t out;                    /* put pai */
  attr_t attr;                  /* attr (how get, how put, how ...) */
  struct river *prev;
  struct river *next;
} river_t;

#define RIVER_MAX	25

/* hand status */

typedef struct hand
{
  int reach;
  int naki;
  int closed_num;
  pai_t closed[14];
  pai_t opened_kind[4];
  int opened_from[4];
  pai_t opened[4][4];
} hand_t;

/* result status */

typedef struct result
{
  int flag;
  int pao;
  int who;
  int points[4];
  int fu;
  int fan;
  int dora;
  int uradora;
  int akadora;

  unsigned yakuman:1;
  unsigned kokusi:1;
  unsigned daisangen:1;
  unsigned suankou:1;
  unsigned sukantu:1;
  unsigned syosusi:1;
  unsigned daisusi:1;
  unsigned tuisou:1;
  unsigned ryuisou:1;
  unsigned cyurenpotou:1;
  unsigned tenhou:1;
  unsigned chihou:1;
  unsigned chinraotou:1;
  unsigned renhou:1;
  unsigned nagasimangan:1;

  unsigned double_reach:1;
  unsigned reach:1;
  unsigned ippatu:1;
  unsigned tumo:1;
  unsigned pinfu:1;
  unsigned tanyao:1;
  unsigned iipeikou:1;
  unsigned haitei:1;
  unsigned jikaze:1;
  unsigned bakaze:1;
  unsigned haku:1;
  unsigned hatu:1;
  unsigned chun:1;
  unsigned chankan:1;
  unsigned rinshan:1;
  unsigned sansiki_naki:1;
  unsigned ittu_naki:1;
  unsigned chanta_naki:1;
  unsigned sansiki:1;
  unsigned ittu:1;
  unsigned chanta:1;
  unsigned sanankou:1;
  unsigned sankantu:1;
  unsigned toitoi:1;
  unsigned syosangen:1;
  unsigned honraotou:1;
  unsigned sansiki_doukoku:1;
  unsigned honitu_naki:1;
  unsigned junchan_naki:1;
  unsigned honitu:1;
  unsigned junchan:1;
  unsigned ryanpeikou:1;
  unsigned chinitu_naki:1;
  unsigned chinitu:1;
  unsigned chitoitu:1;
  /* other messages */
  unsigned furiten:1;
  unsigned doujun:1;
  unsigned kyusukyupai:1;
  unsigned sufurenta:1;
  unsigned sukannagare:1;
  unsigned sancyahou:1;
  unsigned noten_reach:1;
} result_t;

#define RESULT_NULL		0
#define RESULT_RYUKYOKU		1
#define RESULT_RYUKYOKU_CONT	2
#define RESULT_CYONBO		3
#define RESULT_CYONBO_CONT	4
#define RESULT_AGARI_TUMO	5
#define RESULT_AGARI_RON	6

#define PL_NEXT		000
#define PL_BREAK	001
#define PL_CONTINUE	002

#define PL_COUNTUP	010
#define PL_COUNTSAME	020

#define ACT_RES		1
#define ACT_SEL		2
#define ACT_PEND	3
#define ACT_GEND	4
#define ACT_COMMENT	5
#define ACT_PLAY	6
#define ACT_RESULT	7
#define ACT_CONNECT	8
#define ACT_REACH	9
#define ACT_FIX		10
#define ACT_MSG		11

extern char *bturn_names[];
extern char *sturn_sfx;
extern char *sturn_cnt_sfx;
extern char *rest_str;
extern char *dora_str;
extern char *uradora_str;
extern char *akadora_str;
extern char *home_names[];
extern char *result_names[];
extern char *fu_sfx;
extern char *fan_sfx;

extern player_t player[4];
extern int first_player;        /* first_player of game (pplayer) */
extern int play_over;           /* play end flag (for server only) */
extern int big_turn;            /* place wind  */
extern int small_turn;          /* kyoku */
extern int small_turn_cnt;      /* kyoku count */
extern int rest_point;          /* place point */
extern int pplayer[4];          /* convert table vplayer -> pplayer */
extern int cur_player;          /* current player (vplayer) */
extern pai_t mt[136];
extern int mt_top_cnt;          /* rinsyan pai */
extern int mt_normal_cnt;       /* tumo pai */
extern pai_t mt_next[256];      /* for next mt (server) */
extern int mt_next_cnt;
extern int mt_next_cnt2;
extern river_t rv[4][RIVER_MAX];
extern int rv_cur[4];
extern river_t *rvp;
extern hand_t hand[4];
extern hand_t init_hand[4];
extern result_t result[5];
extern int self;                /* pplayer */
extern int vself;               /* vplayer */
extern int ch_game;
extern int in_game;
extern int ch_play;
extern int ch_dora;
extern int ch_river[4];
extern int ch_hand[4];
extern int ch_point;
extern int ask_attr;
extern int ask_pai;
extern int comment_from;
extern char comment_body[256];

typedef struct global
{
  player_t _player[4];
  int _first_player;            /* first_player of game (pplayer) */
  int _play_over;               /* play end flag (for server only) */
  int _big_turn;                /* place wind  */
  int _small_turn;              /* kyoku */
  int _small_turn_cnt;          /* kyoku count */
  int _rest_point;              /* place point */
  int _pplayer[4];              /* convert table vplayer -> pplayer */
  int _cur_player;              /* current player (vplayer) */
  pai_t _mt[136];
  int _mt_top_cnt;              /* rinsyan pai */
  int _mt_normal_cnt;           /* tumo pai */
  pai_t _mt_next[256];          /* for next mt (server) */
  int _mt_next_cnt;
  int _mt_next_cnt2;
  river_t _rv[4][RIVER_MAX];
  int _rv_cur[4];
  river_t *_rvp;
  hand_t _hand[4];
  hand_t _init_hand[4];
  result_t _result[5];
  int _self;                    /* pplayer */
  int _vself;                   /* vplayer */
  int _ch_game;
  int _in_game;
  int _ch_play;
  int _ch_dora;
  int _ch_river[4];
  int _ch_hand[4];
  int _ch_point;
  int _ask_attr;
  int _ask_pai;
  int _comment_from;
  char _comment_body[256];
  void *ext[10];
} global_t;

global_t *global_alloc ();

#define	player 			(gp->_player)
#define	first_player 		(gp->_first_player)
#define	play_over 		(gp->_play_over)
#define	big_turn 		(gp->_big_turn)
#define	small_turn 		(gp->_small_turn)
#define	small_turn_cnt 		(gp->_small_turn_cnt)
#define	rest_point 		(gp->_rest_point)
#define	pplayer 		(gp->_pplayer)
#define	cur_player 		(gp->_cur_player)
#define	mt 			(gp->_mt)
#define	mt_top_cnt 		(gp->_mt_top_cnt)
#define	mt_normal_cnt 		(gp->_mt_normal_cnt)
#define	mt_next 		(gp->_mt_next)
#define	mt_next_cnt 		(gp->_mt_next_cnt)
#define	mt_next_cnt2 		(gp->_mt_next_cnt2)
#define	rv 			(gp->_rv)
#define	rv_cur 			(gp->_rv_cur)
#define	rvp 			(gp->_rvp)
#define	hand 			(gp->_hand)
#define	init_hand 		(gp->_init_hand)
#define	result 			(gp->_result)
#define	self 			(gp->_self)
#define	vself 			(gp->_vself)
#define	ch_game 		(gp->_ch_game)
#define	in_game 		(gp->_in_game)
#define	ch_play 		(gp->_ch_play)
#define	ch_dora 		(gp->_ch_dora)
#define	ch_river 		(gp->_ch_river)
#define	ch_hand 		(gp->_ch_hand)
#define	ch_point 		(gp->_ch_point)
#define	ask_attr 		(gp->_ask_attr)
#define	ask_pai 		(gp->_ask_pai)
#define	comment_from 		(gp->_comment_from)
#define	comment_body 		(gp->_comment_body)

#ifndef NO_PROTOTYPE

/* ahand.c functions... */
void ah_new (ahand_t *a, hand_t *h, int rm);
void ah_remove (ahand_t *a, int i);
int ah_index (ahand_t *a, int pai);
void ah_sort (ahand_t *a);
void ah_result_add (ahand_t *a);
void ah_analize (ahand_t *a, int from);
void ah_print (ahand_t *a, char *name);

/* analize.c functions... */
char *analize_message (global_t *gp);
void analize_init (global_t *gp);
void analize (global_t *gp, int p);
int analize_res (global_t *gp, int *howp);

/* cvt.c functions... */
void result_cvt_to_int (result_t *yp, int *iy);
void result_cvt_from_int (result_t *yp, int *iy);

/* game.c functions... */
void game (global_t *gp);
int play (global_t *gp);
void ask (global_t *gp, int p);
void wait_repl (global_t *gp, int p);
int check_ron (global_t *gp, int *pp);
int check_kan (global_t *gp, int *pp);
int check_pon (global_t *gp, int *pp);
int check_tie (global_t *gp, int *pp, int *howp);
void sort_player (global_t *gp, int *s);

/* global.c functions... */
global_t *global_alloc ();
void global_init (global_t *gp);
void send_newgame (global_t *gp);
void send_endgame (global_t *gp, int *order);
void send_newplay (global_t *gp);
void send_endplay (global_t *gp);
void send_dora (global_t *gp);
void send_uradora (global_t *gp);
void send_river (global_t *gp);
void send_result (global_t *gp);
void send_riverin (global_t *gp, int p);
void send_hand_closed (global_t *gp, int p);
void send_hand_opened (global_t *gp, int p, int j);
void send_inithand (global_t *gp, int p);
void send_point (global_t *gp);
int recv_message (global_t *gp, char *buf);
void set_pplayer (global_t *gp);

/* hand.c functions ... */
void hand_new (global_t *gp);
void hand_sort (global_t *gp, int p);
void hand_insert (global_t *gp, int p, int c);
void hand_delete (global_t *gp, int p, pai_t c, int sort);
int hand_delete_kind (global_t *gp, int p, pai_t c, int sort);
int hand_find (global_t *gp, int p, pai_t pc);
int hand_find_kind (global_t *gp, int p, pai_t c);
int hand_tie (global_t *gp, int p, int pc, int how);
int hand_pon (global_t *gp, int p, int pc, int from);
int hand_kan (global_t *gp, int p, int pc, int from);
int hand_can_tie (global_t *gp, int p, int pc, int how);
int hand_can_tiex (global_t *gp, int p, int pc);
int hand_can_pon (global_t *gp, int p, int pc);
int hand_can_kan (global_t *gp, int p, int pc);
int hand_can_kanx (global_t *gp, int p);
int hand_check_kyushukyupai (global_t *gp, int p);
int hand_check_kuikae (global_t *gp, int p, int pc);
int hand_check_pao (global_t *gp, int p);

/* log.c functions ... */
char *pai2str (pai_t n);
char *attr2str (attr_t n);
void log_play (global_t *gp, FILE *fp);
void log_hand (global_t *gp, FILE *fp, int p);
void log_result (global_t *gp, FILE *fp);
void log_yaku (global_t *gp, FILE *fp, result_t *yp);
void log_game (global_t *gp, FILE *fp);

/* mt.c functions ... */
int mt_dora (global_t *gp, int n);
int mt_uradora (global_t *gp, int n);
int mt_rest (global_t *gp);
void mt_new (global_t *gp);
void mt_next_new (global_t *gp);
void mt_setdora (global_t *gp, int n, int v);
void mt_seturadora (global_t *gp, int n, int v);
int mt_get (global_t *gp);
int mt_gettop (global_t *gp);
void mt_make (global_t *gp);
void mt_haipai (global_t *gp, pai_t *buf);
int mt_push (global_t *gp, pai_t p, int mode);
int mt_rest_kind (global_t *gp, pai_t p);

/* pf.c functions... */
void pf_init (global_t *gp, char *file, char *prog);
void pf_term (global_t *gp);
void pf_gend (global_t *gp);
void pf_pend (global_t *gp);
void add_yakus (global_t *gp, int i);
void print_yakus (global_t *gp, FILE *fp);
void pf_add_param (global_t *gp, char *name, int value, char *str);
int pf_param (global_t *gp, char *name, int *vp);
int pf_param_str (global_t *gp, char *name, char *buf);
int pf_param_strn (global_t *gp, char *name, char *buf, int n);
void pf_clear (global_t *gp);

/* xsleep.c functions... */
void xsleep (int time_out);
void ysleep (int time_out);

/* rv.c functions... */
void rv_new (global_t *gp);
void rv_push (global_t *gp, int to);
int rv_find_player (global_t *gp, river_t *p);
int rv_find_cur (global_t *gp, river_t *p);

/* ruledef.c functions... */
void expand_rule (int *r1);
void pack_rule (int *r1);
void rule_init (global_t *gp);
char *rule_message ();

/* result.c functions... */
void result_new (global_t *gp);
int result_check_sufurenta (global_t *gp);
int result_check_nagasimangan (global_t *gp, int p);
void result_calc_nagasimangan (global_t *gp, int p, result_t *yp);
int result_check_kyushukyupai (global_t *gp);
int result_check_sukan (global_t *gp);
void result_calc (global_t *gp, int mode, int *pp, int pp_num);
void calc_tumibo (global_t *gp, result_t *yr, int furi);
void result_calc_ryukyoku (global_t *gp, result_t *yr);
int result_calc_yaku (global_t *gp, int p, result_t *yr);
int result_check_furiten (global_t *gp, int p, result_t *yp);
int result_calc_from_ahand (global_t *gp, int p, result_t *yp, ahand_t *ap);
int fu_calc_koutu (int pai, int shift);
int result_calc_from_hand (global_t *gp, int p, result_t *yp);
int result_calc_point (global_t *gp, int p, result_t *yp);

#endif

#endif
