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

FILE *plog;
/* game status */

global_t *
global_alloc ()
{
  global_t *ret;
  ret = (global_t *) malloc (sizeof (global_t));
  bzero (ret, sizeof (global_t));
  return ret;
}

void
global_init (global_t *gp)
{
  set_pplayer (gp);
  vself = (self - first_player - small_turn + 8) % 4;
  mt_new (gp);
  rv_new (gp);
  hand_new (gp);
  result_new (gp);
}

void
send_newgame (global_t *gp)
{
  int i;
  char buf[MSG_MAX];

  sprintf (buf, MSG_GAME, player[0].name, player[0].gpoint, player[0].flags,
           player[1].name, player[1].gpoint, player[1].flags, player[2].name,
           player[2].gpoint, player[2].flags, player[3].name,
           player[3].gpoint, player[3].flags, first_player);
  for (i = 0; i < 4; i++)
    msgWrites (gp, buf, i);
}

void
send_endgame (global_t *gp, int *order)
{
  int i;
  char buf[MSG_MAX];

  sprintf (buf, MSG_GAMEEND, player[0].rpoint, player[1].rpoint,
           player[2].rpoint, player[3].rpoint, player[0].tpoint,
           player[1].tpoint, player[2].tpoint, player[3].tpoint);
  for (i = 0; i < 4; i++)
    msgWrites (gp, buf, i);
}

void
send_newplay (global_t *gp)
{
  int i;
  char buf[MSG_MAX];
  sprintf (buf, MSG_PLAY, big_turn, small_turn, small_turn_cnt, rest_point);
  for (i = 0; i < 4; i++)
    msgWrites (gp, buf, i);
}

void
send_endplay (global_t *gp)
{
  int i;
  char buf[MSG_MAX];

  sprintf (buf, MSG_PLAYEND);
  for (i = 0; i < 4; i++)
    msgWrites (gp, buf, i);
}

void
send_dora (global_t *gp)
{
  int i;
  pai_t x[5];
  char buf[MSG_MAX];
  for (i = 0; i < 5; i++)
    {
      x[i] = mt_dora (gp, i);
      mt_push (gp, x[i], 0);
    }
  sprintf (buf, MSG_DORA, x[0], x[1], x[2], x[3], x[4]);
  for (i = 0; i < 4; i++)
    msgWrites (gp, buf, i);
}

void
send_uradora (global_t *gp)
{
  int i;
  pai_t x[5];
  char buf[MSG_MAX];
  for (i = 0; i < 5; i++)
    {
      x[i] = mt_uradora (gp, i);
      mt_push (gp, x[i], 0);
    }
  sprintf (buf, MSG_URADORA, x[0], x[1], x[2], x[3], x[4]);
  for (i = 0; i < 4; i++)
    msgWrites (gp, buf, i);
}

void
send_river (global_t *gp)
{
  int i, c, pp;
  char buf[MSG_MAX];
  char buf_masked[MSG_MAX];

  if (F_GET (rvp->attr) == F_RV_PON
      || F_GET (rvp->attr) == F_RV_TIE || rvp->in == rvp->out)
    c = rvp->in;
  else
    c = P_NULL;

  sprintf (buf, MSG_RIVER, cur_player, rv_cur[cur_player], rvp->attr, rvp->in,
           rvp->out);
  sprintf (buf_masked, MSG_RIVER, cur_player, rv_cur[cur_player], rvp->attr,
           c, rvp->out);
  for (i = 0; i < 4; i++)
    {
      pp = pplayer[i];
      if (cur_player == i || (player[pp].flags & M_SUPER))
        {
          msgWrites (gp, buf, pp);
        }
      else
        {
          msgWrites (gp, buf_masked, pp);
        }
    }
}

void
send_result (global_t *gp)
{
  result_t *yp;
  int i;
  int p;
  char buf[MSG_MAX];
  int iy[10];

  for (p = 0; p < 5; p++)
    if (result[p].flag)
      {
        yp = &(result[p]);

        result_cvt_to_int (yp, iy);
        sprintf (buf, MSG_RESULT, p, yp->flag, yp->points[0], yp->points[1],
                 yp->points[2], yp->points[3], yp->fu, yp->fan, yp->dora,
                 yp->uradora, yp->akadora, iy[0], iy[1], iy[2], iy[3]);
        for (i = 0; i < 4; i++)
          msgWrites (gp, buf, i);
      }
}

void
send_riverin (global_t *gp, int p)
{
  int i;
  char buf[MSG_MAX];

  sprintf (buf, MSG_RIVERIN, p, rv[p][0].in, rv[p][1].in, rv[p][2].in,
           rv[p][3].in, rv[p][4].in, rv[p][5].in, rv[p][6].in, rv[p][7].in,
           rv[p][8].in, rv[p][9].in, rv[p][10].in, rv[p][11].in, rv[p][12].in,
           rv[p][13].in, rv[p][14].in, rv[p][15].in, rv[p][16].in,
           rv[p][17].in, rv[p][18].in, rv[p][19].in, rv[p][20].in,
           rv[p][21].in, rv[p][22].in, rv[p][23].in, rv[p][24].in);

  for (i = 0; i < 4; i++)
    {
      msgWrites (gp, buf, i);
    }
}

void
send_hand_closed (global_t *gp, int p)
{
  int i, pp;
  pai_t *x;
  char buf[MSG_MAX];

  x = hand[p].closed;
  sprintf (buf, MSG_HANDC, p, hand[p].closed_num, x[0], x[1], x[2], x[3],
           x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12], x[13]);

  pp = pplayer[p];

  for (i = 0; i < 4; i++)
    {
      if (play_over || (pp == i) || (player[i].flags & M_SUPER))
        msgWrites (gp, buf, i);
    }
}

void
send_hand_opened (global_t *gp, int p, int j)
{
  int i, k, a, pp;
  pai_t *x;
  char buf[MSG_MAX];

  pp = pplayer[p];

  k = hand[p].opened_kind[j];
  if (k)
    {
      x = hand[p].opened[j];
      sprintf (buf, MSG_HANDO, p, j, k, x[0], x[1], x[2], x[3],
               hand[p].opened_from[j]);
      for (a = 0; a < 4; a++)
        {
          mt_push (gp, x[a], 0);
        }
      for (i = 0; i < 4; i++)
        {
          msgWrites (gp, buf, i);
        }
    }
}

void
send_inithand (global_t *gp, int p)
{
  int i;
  pai_t *x;
  char buf[245];

  x = init_hand[p].closed;
  sprintf (buf, MSG_HANDI, p, init_hand[p].closed_num, x[0], x[1], x[2], x[3],
           x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12], x[13]);

  for (i = 0; i < 4; i++)
    {
      msgWrites (gp, buf, i);
    }
}

void
send_point (global_t *gp)
{
  int i;
  char buf[245];

  sprintf (buf, MSG_POINT, player[0].ppoint, player[1].ppoint,
           player[2].ppoint, player[3].ppoint, rest_point, player[0].tpoint,
           player[1].tpoint, player[2].tpoint, player[3].tpoint);
  for (i = 0; i < 4; i++)
    msgWrites (gp, buf, i);
}

int
recv_message (global_t *gp, char *buf)
{
  int i, j, k, n;
  int a, b, c;
  int x[40];
  int ret = 0;
  result_t y;

  if ((a =
       sscanf (buf, MSG_CONNECT_PLAYER, player[0].name, player[1].name,
               player[2].name, player[3].name)) >= 1)
    {
      for (i = a; i < 4; i++)
        player[i].name[0] = 0;
      ret = ACT_CONNECT;
    }
  else
    if (sscanf
        (buf, MSG_GAME, player[0].name, &player[0].gpoint, &player[0].flags,
         player[1].name, &player[1].gpoint, &player[1].flags, player[2].name,
         &player[2].gpoint, &player[2].flags, player[3].name,
         &player[3].gpoint, &player[3].flags, &first_player) >= 13)
    {
      for (i = 0; i < 4; i++)
        {
          player[i].tpoint = 0;
          player[i].ppoint = 0;
        }
      ch_game = 1;
      in_game = 1;
    }
  else
    if (sscanf
        (buf, MSG_PLAY, &big_turn, &small_turn, &small_turn_cnt,
         &rest_point) >= 4)
    {
      ch_play = 1;
      set_pplayer (gp);
      vself = (self - first_player - small_turn + 8) % 4;
      mt_new (gp);
      mt_next_new (gp);
      rv_new (gp);
      hand_new (gp);
      result_new (gp);
      for (i = 0; i < 4; i++)
        {
          player[i].ppoint = 0;
        }
      ret = ACT_PLAY;
    }
  else if ((c = sscanf (buf, MSG_DORA, x, x + 1, x + 2, x + 3, x + 4)) >= 1)
    {
      ch_dora = 1;
      for (i = 0; i < c; i++)
        {
          mt_push (gp, x[i], 0);
          mt_setdora (gp, i, x[i]);
        }
    }
  else if ((c = sscanf (buf, MSG_URADORA, x, x + 1, x + 2, x + 3, x + 4)) >=
           1)
    {
      ch_dora = 1;
      for (i = 0; i < c; i++)
        {
          mt_push (gp, x[i], 0);
          mt_seturadora (gp, i, x[i]);
        }
    }
  else if (sscanf (buf, MSG_RIVER, &i, &j, x, x + 1, x + 2) >= 5)
    {
      if (rvp)
        ch_river[rv_find_player (gp, rvp)] = 1;
      ch_river[i] = 1;
      ch_dora = 1;
      if (plog)
        fprintf (plog, "MSG_RIVER %d %d \n", i, j);
      if (!rv[i][j].out)
        {
          rv[i][j].attr = x[0];
          rv[i][j].in = x[1];
          rv[i][j].out = x[2];
          if (i != vself)
            rv_push (gp, i);
          mt_push (gp, rvp->out,
                   F_GET (rvp->attr) == F_MT
                   || F_GET (rvp->attr) == F_MT_TOP
                   || F_GET (rvp->attr) == F_RV_KAN);
          c = F_GET (rvp->attr);
          if (i != vself)
            {
              if (T_GET (rvp->attr) != T_HN_TSM)
                ret = ACT_RES;
            }
          else
            {
              hand_delete (gp, vself, rvp->out, 1);
              ch_hand[vself] = 1;
            }
          if (T_GET (rvp->attr) == T_RV_RCH)
            {
              hand[i].reach = 1;
              ch_point = 1;
              if (i == vself)
                ret = ACT_REACH;
            }
        }
      else
        {
          rv[i][j].attr = x[0];
          ret = ACT_FIX;
        }
    }

  else
    if (sscanf
        (buf, MSG_HANDC, &i, &c, x + 0, x + 1, x + 2, x + 3, x + 4, x + 5,
         x + 6, x + 7, x + 8, x + 9, x + 10, x + 11, x + 12, x + 13) >= 2)
    {
      ch_hand[i] = 1;
      for (j = 0; j < 14; j++)
        {
          hand[i].closed[j] = x[j];
          mt_push (gp, x[j], 0);
        }
      hand[i].closed_num = c;
    }
  else
    if (sscanf
        (buf, MSG_HANDI, &i, &c, x + 0, x + 1, x + 2, x + 3, x + 4, x + 5,
         x + 6, x + 7, x + 8, x + 9, x + 10, x + 11, x + 12, x + 13) >= 2)
    {
      for (j = 0; j < 14; j++)
        {
          init_hand[i].closed[j] = x[j];
        }
      init_hand[i].closed_num = c;
    }
  else
    if ((n =
         sscanf (buf, MSG_HANDO, &i, &j, &a, x, x + 1, x + 2, x + 3,
                 &b)) >= 7)
    {
      ch_hand[i] = 1;
      for (k = 0; k < 4; k++)
        {
          hand[i].opened[j][k] = x[k];
          mt_push (gp, x[k], 0);
        }
      hand[i].opened_kind[j] = a;
      if (n >= 8)
        hand[i].opened_from[j] = b;
      else
        hand[i].opened_from[j] = i;

      if (a && a != H_KAN_CLOSED)
        hand[i].naki = 1;
      if (hand[i].closed_num > 13 - 3 * (j + 1))
        hand[i].closed_num = 13 - 3 * (j + 1);
      if (protocol_version >= 2)
        {
          ret = ACT_MSG + i * 4 + j;
        }
    }
  else
    if (sscanf
        (buf, MSG_RIVERIN, &i, x + 0, x + 1, x + 2, x + 3, x + 4, x + 5,
         x + 6, x + 7, x + 8, x + 9, x + 10, x + 11, x + 12, x + 13, x + 14,
         x + 15, x + 16, x + 17, x + 18, x + 19, x + 20, x + 21, x + 22,
         x + 23, x + 24) >= 1)
    {
      ch_river[i] = 1;
      for (j = 0; j < RIVER_MAX; j++)
        {
          rv[i][j].in = x[j];
        }
    }
  else if (sscanf (buf, MSG_ASK, &a, &b) >= 2)
    {
      ask_attr = a;
      ask_pai = b;
      rv_push (gp, vself);
      if (plog)
        fprintf (plog, "MSG_ASK\n");
      ret = ACT_SEL;
    }
  else if (!strcmp (buf, MSG_KILL))
    {
      /* do nothing */
    }
  else if (!strcmp (buf, MSG_PLAYEND))
    {
      ch_play = 1;
      for (i = 0; i < 4; i++)
        {
          player[i].gpoint += player[i].ppoint;
        }
      ret = ACT_PEND;
    }
  else
    if (sscanf
        (buf, MSG_GAMEEND, &player[0].rpoint, &player[1].rpoint,
         &player[2].rpoint, &player[3].rpoint, &player[0].tpoint,
         &player[1].tpoint, &player[2].tpoint, &player[3].tpoint) == 8)
    {
      in_game = 2;
      ret = ACT_GEND;
    }
  else if (sscanf (buf, MSG_COMMENT2X, &comment_from) == 1)
    {
      char *p;
      comment_body[0] = 0;
      if (index (buf, '>'))
        strcpy (comment_body, index (buf, '>') + 1);
      if (p = index (comment_body, '\n'))
        *p = 0;
      ret = ACT_COMMENT;
    }
  else
    if (sscanf
        (buf, MSG_RESULT, &a, &y.flag, y.points, y.points + 1, y.points + 2,
         y.points + 3, &y.fu, &y.fan, &y.dora, &y.uradora, &y.akadora, x,
         x + 1, x + 2, x + 3) == 15)
    {

      result_cvt_from_int (&y, x);
      result_set_who (&y);
      result[a] = y;
      ret = ACT_RESULT;
    }
  else
    if ((c =
         sscanf (buf, MSG_POINT, x, x + 1, x + 2, x + 3, x + 4, x + 5, x + 6,
                 x + 7, x + 8)) >= 5)
    {
      player[0].ppoint = x[0];
      player[1].ppoint = x[1];
      player[2].ppoint = x[2];
      player[3].ppoint = x[3];
      if (c == 9)
        {
          player[0].tpoint = x[5];
          player[1].tpoint = x[6];
          player[2].tpoint = x[7];
          player[3].tpoint = x[8];
        }
      rest_point = x[4];
      ch_point = 1;
    }
  else
    {
      printf ("%s", buf);
    }
  return ret;
}

void
set_pplayer (global_t *gp)
{
  int i;
  for (i = 0; i < 4; i++)
    {
      pplayer[i] = (first_player + small_turn + i) % 4;
    }
}
