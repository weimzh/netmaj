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
 * 5-7-2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"
#include "ruledef.h"

extern int tonpu;

#define G2R(x)	(((x)+1000400)/1000 - 1000)

extern FILE *log_server;

void
game (global_t *gp)
{
  int top_point;
  int order[4];
  int i;
  int st;
  int uki_cnt = 0;

  for (i = 0; i < 4; i++)
    {
      player[i].gpoint = RL_STARTPOINT;
      player[i].ppoint = 0;
      player[i].tpoint = 0;
    }

  /* deside the first player */
  first_player = random () % 4;

  send_newgame (gp);

  small_turn_cnt = 0;
  rest_point = 0;
  for (big_turn = 0; big_turn < (tonpu ? 1 : 2); big_turn++)
    {
      for (small_turn = 0; small_turn < 4;)
        {
          st = play (gp);
          for (i = 0; i < 4; i++)
            {
              player[i].gpoint += player[i].ppoint;
              player[i].ppoint = 0;
            }
          /* tobi check */
          for (i = 0; i < 4; i++)
            {
              if (player[i].gpoint < 0 && !RL_NOFLY)
                goto flied;
            }
          if (st & PL_BREAK)
            goto flied;
          if (st & PL_COUNTUP)
            {
              small_turn_cnt++;
              /* agari yame ? */
              if (small_turn == 3 && big_turn == (tonpu ? 0 : 1))
                {
                  sort_player (gp, order);
                  if (order[0] == (first_player + 3) % 4 && !RL_NOFLY)
                    {
                      goto flied;
                    }
                }
            }
          else if (!(st & PL_COUNTSAME))
            small_turn_cnt = 0;
          if (st & PL_CONTINUE)
            continue;
          small_turn++;
        }
    }

flied:
  sort_player (gp, order);
  top_point = 0;
  for (i = 0; i < 4; i++)
    {
      player[i].gpoint -= 30000;
      if (player[i].gpoint > 0)
        uki_cnt++;
      if (i != order[0])
        {
          top_point += player[i].gpoint;
        }
    }
  if (uki_cnt == 0)
    uki_cnt = 1;

  player[order[0]].gpoint = 0 - top_point;

  player[order[0]].rpoint = 0;
  for (i = 1; i < 4; i++)
    {
      player[order[i]].rpoint = G2R (player[order[i]].gpoint);
      if (player[order[i]].rpoint < -30 && !(RL_NOFLY || RL_UNDER))
        player[order[i]].rpoint = -30;
      player[order[0]].rpoint -= player[order[i]].rpoint;
    }

  for (i = 0; i < 4; i++)
    {
      player[order[i]].rpoint += uma[i + (uki_cnt - 1) * 4];
    }

  if (log_server)
    log_game (gp, log_server);
  send_endgame (gp, order);
}

int
play (global_t *gp)
{
  int i, j, x, ret;
  int c, attr, how;
  int p0, p1, p2, p3;
  int next_player;
  int t_attr;
  int rf = 0;
  int ron_player[4];
  int ron_player_num;
  int reach = 0;
  int chips, yakuman;
  int pos, pai;

  play_over = 0;
  if (log_server)
    fprintf (log_server,
             "play %d %d %d\n", big_turn, small_turn, small_turn_cnt);
  mt_new (gp);
  rv_new (gp);
  hand_new (gp);
  result_new (gp);
  set_pplayer (gp);
  send_newplay (gp);
  mt_make (gp);
  send_dora (gp);

  for (i = 0; i < 4; i++)
    {
      mt_haipai (gp, hand[i].closed);
      hand_sort (gp, i);
      bcopy (&hand[i], &init_hand[i], sizeof (hand_t));
      send_hand_closed (gp, i);
    }

  next_player = 0;
  c = mt_get (gp);
  attr = F_MT;
  hand_insert (gp, cur_player, c);
  for (;;)
    {
      rv_push (gp, next_player);
      rvp->in = c;
      rvp->attr = attr;
      p0 = cur_player;
      p1 = (cur_player + 1) % 4;
      p2 = (cur_player + 2) % 4;
      p3 = (cur_player + 3) % 4;

      send_hand_closed (gp, p0);
      ask (gp, p0);

      t_attr = T_GET (rvp->attr);
      if (hand[p0].reach)
        rvp->out = hand[p0].closed[hand[p0].closed_num - 1];
      if (!hand_find (gp, p0, rvp->out))
        rvp->out = hand[p0].closed[hand[p0].closed_num - 1];
      if ((t_attr == T_RV_RCH && (hand[p0].reach || hand[p0].naki))
          || (t_attr == T_RV_RCH && (mt_rest (gp) < 4))
          || (t_attr == T_RV_RCH && (player[pplayer[p0]].gpoint < 1000))
          || (t_attr == T_HN_KAN && (mt_rest (gp) == 0)))
        t_attr = T_RV;
      if ((t_attr == T_HN_TSM || t_attr == T_HN_KAN) &&
          ((F_GET (rvp->attr) == F_RV_PON) ||
           (F_GET (rvp->attr) == F_RV_TIE)))
        {
          t_attr = T_RV;
        }
      if (t_attr == T_HN_KAN)
        {
          if (mt_top_cnt >= 4)
            t_attr = T_RV;
          else if (hand[p0].reach)
            {
              if (!reach_can_kan (gp, p0, rvp->out))
                {
                  t_attr = T_RV;
                }
              else
                {
                  t_attr = T_HN_KAN_C;
                }
            }
          else
            {
              x = hand_can_kan (gp, p0, rvp->out);
              switch (x)
                {
                case 1:
                  break;
                case 3:
                  t_attr = T_HN_KAN_C;
                  break;
                default:
                  t_attr = T_RV;
                }
            }
        }
      if (((F_GET (rvp->attr) == F_RV_PON) || (F_GET (rvp->attr) == F_RV_TIE))
          && (pai = hand_check_kuikae (gp, cur_player, rvp->out)))
        {
          rvp->out = pai;
        }
      T_SET (rvp->attr, t_attr);

      if (t_attr == T_HN_KAN || t_attr == T_HN_KAN_C)
        {
          pos = hand_kan (gp, cur_player, rvp->out, cur_player);
          send_hand_opened (gp, cur_player, pos);
        }
      mt_push (gp, rvp->out, 0);        /* set next */
      send_river (gp);

      switch (t_attr)
        {
        case T_RV_RCH:
          hand[p0].reach = 1;
          reach = 1;
        case T_RV:
          hand_delete (gp, p0, rvp->out, 1);
          break;
        case T_HN_KAN:
        case T_HN_KAN_C:
          break;
        case T_HN_TSM:
          result_calc (gp, RESULT_AGARI_TUMO, &p0, 1);
          goto play_end;
          break;
        }
      wait_repl (gp, p0);
      if (ron_player_num = check_ron (gp, ron_player))
        {
          rvp->attr |= R_RON;
          send_river (gp);
          for (i = 0; i < ron_player_num; i++)
            {
              hand_insert (gp, ron_player[i], rvp->out);
            }
          result_calc (gp, RESULT_AGARI_RON, ron_player, ron_player_num);
          for (i = 0; i < ron_player_num; i++)
            {
              hand_delete (gp, ron_player[i], rvp->out, 0);
            }
          goto play_end;
        }
      if (mt_rest (gp) == 0)
        {
          send_river (gp);
          break;
        }
      if (t_attr == T_HN_KAN || t_attr == T_HN_KAN_C)
        {
          c = mt_gettop (gp);
          if (result_check_sukan (gp))
            break;
          attr = F_MT_TOP;
          send_dora (gp);
          send_river (gp);
          next_player = cur_player;
          hand_insert (gp, cur_player, c);
        }
      else if (check_kan (gp, &next_player))
        {
          attr = F_RV_KAN;
          rvp->attr |= R_KAN;
          send_river (gp);
          pos = hand_kan (gp, next_player, rvp->out, cur_player);
          send_hand_opened (gp, next_player, pos);
          if (result_check_sukan (gp))
            break;
          c = mt_gettop (gp);
          send_dora (gp);
          hand_insert (gp, next_player, c);
        }
      else if (check_pon (gp, &next_player))
        {
          c = rvp->out;
          attr = F_RV_PON;
          rvp->attr |= R_PON;
          send_river (gp);
          pos = hand_pon (gp, next_player, rvp->out, cur_player);
          send_hand_opened (gp, next_player, pos);
        }
      else if (check_tie (gp, &next_player, &how))
        {
          c = rvp->out;
          attr = F_RV_TIE;
          rvp->attr |= R_TIE;
          send_river (gp);
          pos = hand_tie (gp, next_player, rvp->out, how);
          send_hand_opened (gp, next_player, pos);
        }
      else
        {
          send_river (gp);
          c = mt_get (gp);
          if (c == 0)
            break;
          next_player = p1;
          attr = F_MT;
          hand_insert (gp, p1, c);
        }
      if (reach)
        {
          reach = 0;
          rest_point += 1000;
          player[pplayer[p0]].ppoint -= 1000;
          send_point (gp);
        }
      if (result_check_sufurenta (gp))
        break;
    }
  result_calc (gp, RESULT_RYUKYOKU, 0, 0);

play_end:
  play_over = 1;

  for (i = 0; i < 5; i++)
    {
      if (result[i].flag)
        {
          rf = result[i].flag;
          if (rf != RESULT_AGARI_TUMO && rf != RESULT_AGARI_RON)
            break;
          yakuman = result[i].yakuman * RLC_YAKUMAN;
          chips = result[i].ippatu * RLC_IPPATU
            + result[i].uradora * RLC_URADORA;
          if (!(RLC_NONAKIAKA && hand[result[i].who].naki))
            {
              chips += result[i].akadora * RLC_AKADORA;
              if (result[i].akadora == 3 && !RLC_NOALLSTARS)
                {
                  chips += 2 * RLC_AKADORA;
                }
            }
          if (rf == RESULT_AGARI_TUMO)
            {
              for (j = 0; j < 4; j++)
                {
                  if (result[i].points[j] < 0)
                    {
                      player[pplayer[j]].tpoint -= chips + yakuman;
                    }
                  if (result[i].points[j] > 0)
                    {
                      player[pplayer[j]].tpoint += (chips + yakuman) * 3;
                    }
                }
            }
          else if (rf == RESULT_AGARI_RON)
            {
              for (j = 0; j < 4; j++)
                {
                  if (result[i].points[j] < 0)
                    {
                      player[pplayer[j]].tpoint -= chips + yakuman * 2;
                    }
                  if (result[i].points[j] > 0)
                    {
                      player[pplayer[j]].tpoint += chips + yakuman * 2;
                    }
                }
            }
        }
    }
  switch (rf)
    {
    case RESULT_AGARI_TUMO:
    case RESULT_AGARI_RON:
      for (i = 0; i < 4; i++)
        {
          if (result[0].points[i] > 0)
            {
              result[0].points[i] += rest_point;
              rest_point = 0;
            }
        }
      if (result[0].points[0] > 0)
        ret = PL_COUNTUP | PL_CONTINUE;
      else
        ret = PL_NEXT;          /* next player */
      break;
    case RESULT_RYUKYOKU:
      ret = PL_COUNTUP;
      break;
    case RESULT_RYUKYOKU_CONT:
      ret = PL_COUNTUP | PL_CONTINUE;
      break;
    case RESULT_CYONBO:
      for (i = 0; i < 4; i++)
        {
          if (hand[i].reach)
            {
              player[pplayer[i]].ppoint += 1000;
              rest_point -= 1000;
            }
        }
      ret = PL_COUNTSAME;
      break;
    case RESULT_CYONBO_CONT:
      for (i = 0; i < 4; i++)
        {
          if (hand[i].reach)
            {
              player[pplayer[i]].ppoint += 1000;
              rest_point -= 1000;
            }
        }
      ret = PL_COUNTSAME | PL_CONTINUE;
      break;
    default:
      ret = PL_NEXT;
    }
  for (i = 0; i < 5; i++)
    if (result[i].flag)
      {
        for (j = 0; j < 4; j++)
          player[pplayer[j]].ppoint += result[i].points[j];
      }

  send_result (gp);
  send_uradora (gp);

  for (i = 0; i < 4; i++)
    {
      send_inithand (gp, i);
      send_riverin (gp, i);
      send_hand_closed (gp, i);
    }
  send_point (gp);
  send_endplay (gp);
  if (log_server)
    log_play (gp, log_server);
  return ret;
}

void
ask (global_t *gp, int p)
{
  int i, a, b;
  int break_flag = 0;
  char buf[MSG_MAX];
  char buf2[MSG_MAX];

  sprintf (buf, MSG_ASK, rvp->attr, rvp->in);
  msgWrites (gp, buf, pplayer[p]);

  while (!break_flag)
    {
      msgSelect (gp, -1);
      for (i = 0; i < 4; i++)
        {
          if (!msgIsRead (gp, pplayer[i]))
            continue;
          if (!msgGets (gp, buf, sizeof (buf) - 1, pplayer[i]))
            {
              term ();
            }
          if (i == p && sscanf (buf, MSG_ASK, &a, &b) >= 2)
            {
              rvp->attr |= (a & T_MASK);
              rvp->out = b;
              break_flag = 1;
            }
          else if (strncmp (MSG_COMMENTX, buf, strlen (MSG_COMMENTX)) == 0)
            {
              talk (gp, pplayer[i], buf + strlen (MSG_COMMENTX));
            }
          else if (strncmp (MSG_AUTO0, buf, 4) == 0)
            {
              runauto (buf + 4);
            }
          else if (!strcmp (buf, MSG_KILL))
            {
              term ();
            }
        }
    }
}

int repls[4];
int hows[4];

void
wait_repl (global_t *gp, int p)
{
  int i, b, c;
  char buf[MSG_MAX];

  for (i = 0; i < 4; i++)
    repls[i] = 0;
  repls[p] = R_ACK;

  while (1)
    {
      msgSelect (gp, -1);
      for (i = 0; i < 4; i++)
        {
          if (!msgIsRead (gp, pplayer[i]))
            continue;
          if (!msgGets (gp, buf, sizeof (buf) - 1, pplayer[i]))
            {
              term ();
            }
          if (sscanf (buf, MSG_REPL, &b, &c) >= 2)
            {
              repls[i] = b;
              hows[i] = c;
            }
          else if (strncmp (MSG_COMMENTX, buf, strlen (MSG_COMMENTX)) == 0)
            {
              talk (gp, pplayer[i], buf + strlen (MSG_COMMENTX));
            }
          else if (strncmp (MSG_AUTO0, buf, 4) == 0)
            {
              runauto (buf + 4);
            }
          else if (!strcmp (buf, MSG_KILL))
            {
              term ();
            }
        }
      if (repls[0] && repls[1] && repls[2] && repls[3])
        break;
    }
}

int
check_ron (global_t *gp, int *pp)
{
  int i;
  int p;
  int ret = 0;

  p = cur_player;
  for (i = 0; i < 4; i++)
    {
      if (repls[p] == R_RON)
        {
          *pp++ = p;
          ret++;
        }
      p = (p + 1) % 4;
    }
  return ret;
}

int
check_kan (global_t *gp, int *pp)
{
  int p;

  for (p = 0; p < 4; p++)
    {
      if (repls[p] == R_KAN && hand_can_kan (gp, p, rvp->out) == 2)
        {
          *pp = p;
          return 1;
        }
    }
  return 0;
}

int
check_pon (global_t *gp, int *pp)
{
  int p;

  for (p = 0; p < 4; p++)
    {
      if (repls[p] == R_PON && hand_can_pon (gp, p, rvp->out))
        {
          *pp = p;
          return 1;
        }
    }
  return 0;
}

int
check_tie (global_t *gp, int *pp, int *howp)
{
  int p;

  p = (cur_player + 1) % 4;

  if (repls[p] == R_TIE && hand_can_tie (gp, p, rvp->out, hows[p]))
    {
      *howp = hows[p];
      *pp = p;
      return 1;
    }
  return 0;
}

void
sort_player (global_t *gp, int *s)
{
  int i, j, tmp;
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
}
