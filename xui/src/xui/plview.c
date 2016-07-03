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
 * Modified by Wei Mingzhi <whistler@openoffice.org> at 5/7/2006
 */

#define USAGE "\
usage:\n\
    plview [-o outfile] file \n"

#ifndef DEF_OPTARG
#define DEF_OPTARG
#endif

#define NO_PROTOTYPE

#include "global.h"
#include <signal.h>

extern char *pai_strings[];
river_t mrv[4][RIVER_MAX];

FILE *df, *plog;
FILE *logfile;
FILE *fp;
FILE *ofp;

int auto_after_reach = 0;
int auto_play = 0;
int auto_stop = 0;

void tr_play (global_t *gp);
void get_event (global_t *gp, int x, int y, int z);
void msg_clear ();
void msg_put (int x, int y, int z, char *buf);
void tmp_setppoint (global_t *gp, int i, int c);
void tmp_setdora (global_t *gp, int i, int c);
void tmp_adddora (global_t *gp);
void tmp_seturadora (global_t *gp, int i, int c);
void tmp_adduradora (global_t *gp);
void tmp_flush_dora (global_t *gp);
void tmp_flush_point (global_t *gp);

int getopt (int, char *const *, const char *);

void
analize ()
{
}

void
analize_res ()
{
}

char *
analize_message ()
{
  return "";
}

void
callback ()
{
}

int
callback_fd ()
{
  return -1;
}

int
callback_fd2 ()
{
  return -1;
}

void
do_disconnect ()
{
}

void
execute_auto ()
{
}

void
msgTalk ()
{
}

int
pf_param ()
{
}

int
pf_param_str ()
{
}

int
pf_param_strn ()
{
}

void
term ()
{
  if (ofp)
    fclose (ofp);
  ui_term ();
  exit (1);
}

int play_speed = 10;
int readable_key = 0;
int readable_socket = 0;
int rv_fixed = 0;
pai_t *cur_hand;
int view_river_all = 0;

hand_t last_hand[4];

int
main (int argc, char *argv[])
{
  global_t g;
  int i, c;
  char *board = "";

  while ((c = getopt (argc, argv, "o:T:")) != EOF)
    {
      switch (c)
        {
        case 'o':
          ofp = fopen (optarg, "w");
          break;
        case 'T':
          board = optarg;
          break;
        default:
          fprintf (stderr, USAGE);
          exit (1);
          break;
        }
    }
  if (argc > optind)
    fp = fopen (argv[optind], "r");
  if (!fp)
    exit (1);

  signal (SIGTERM, term);
  signal (SIGINT, term);

  bzero (&g, sizeof (g));
  for (i = 0; i < 4; i++)
    g._pplayer[i] = i;
  mt_next_new (&g);

  ui_init (&g, PROG_PLVIEW, board);
  get_event (&g, -1, -1, -1);
  while (rlog (&g, fp) == 0)
    {
      tr_play (&g);
      msg_clear ();
      if (ofp)
        log_play (&g, ofp);
      bzero (&g, sizeof (g));
      for (i = 0; i < 4; i++)
        g._pplayer[i] = i;
      mt_next_new (&g);
    }
  ui_term ();
}

int
str2pai (char *p)
{
  int i;
  for (i = 0; i < (K_MAX * 4); i++)
    {
      if (!(strncmp (pai_strings[i], p, 3)))
        return i;
    }
  return 0;
}

int
str2attr (char *p)
{
  int i, ret;

  ret = 0;
  for (i = 0; i < 8; i++)
    if (F_CHARS[i] == p[0])
      {
        F_SET (ret, (i << F_SHFT));
        break;
      }
  for (i = 0; i < 8; i++)
    if (T_CHARS[i] == p[1])
      {
        T_SET (ret, (i << T_SHFT));
        break;
      }
  for (i = 0; i < 8; i++)
    if (R_CHARS[i] == p[2])
      {
        R_SET (ret, (i << R_SHFT));
        break;
      }
  return ret;
}

void
tr_play (global_t *gp)
{
  river_t *nrvp;
  int i, j;
  int breakf = 0;
  int nagare = 1;

  rv_fixed = 1;
  rvp = &(rv[0][0]);
  rvp->in = mrv[0][0].in;
  cur_player = 0;
  hand_insert (gp, cur_player, rvp->in);
  cur_hand = &hand[cur_player].closed[hand[cur_player].closed_num - 1];
  mt_push (gp, rvp->in, 1);
  ui_draw (gp, 1);
  get_event (gp, 0, 0, 0);
  *rvp = mrv[0][0];
  while (rvp->in && !breakf)
    {
      hand_delete (gp, cur_player, rvp->out, 1);
      cur_hand = 0;
      ch_hand[cur_player] = 1;
      ch_river[cur_player] = 1;
      ch_dora = 1;
      rv_fixed = 0;
      ui_draw (gp, 0);
      get_event (gp, cur_player, rv_cur[cur_player], 1);
      ch_river[cur_player] = 1;
      rv_fixed = 1;
      ui_draw (gp, 0);

      if (T_GET (rvp->attr) == T_RV_RCH)
        {
          hand[cur_player].reach = 1;
          rest_point += 1000;
          ui_message_reach (cur_player);
        }
      switch (R_GET (rvp->attr))
        {
        case R_TIE:
          rv_cur[cur_player]++;
          cur_player = (cur_player + 1) % 4;
          ui_message_naki (cur_player, 0);
          nrvp = &(rv[cur_player][rv_cur[cur_player]]);
          *nrvp = mrv[cur_player][rv_cur[cur_player]];
          rvp->next = nrvp;
          nrvp->prev = rvp;
          for (i = 0; i < 4; i++)
            {
              if (!hand[cur_player].opened_kind[i])
                break;
            }
          hand_tie (gp, cur_player, rvp->out,
                    last_hand[cur_player].opened[i][1], rv_find_player (gp,
                                                                        rvp));
          ch_hand[cur_player] = 1;
          rvp = nrvp;
          break;
        case R_PON:
          rv_cur[cur_player]++;
          for (i = 0; i < 3; i++)
            {
              cur_player = (cur_player + 1) % 4;
              if (hand_can_pon (gp, cur_player, rvp->out))
                {
                  nrvp = &(rv[cur_player][rv_cur[cur_player]]);
                  *nrvp = mrv[cur_player][rv_cur[cur_player]];
                  break;
                }
            }
          ui_message_naki (cur_player, 1);
          hand_pon (gp, cur_player, rvp->out, rv_find_player (gp, rvp));
          ch_hand[cur_player] = 1;
          rvp->next = nrvp;
          nrvp->prev = rvp;
          rvp = nrvp;
          break;
        case R_KAN:
          tmp_adddora (gp);
          rv_cur[cur_player]++;
          for (i = 0; i < 3; i++)
            {
              cur_player = (cur_player + 1) % 4;
              if (hand_can_kan (gp, cur_player, rvp->out))
                {
                  nrvp = &(rv[cur_player][rv_cur[cur_player]]);
                  *nrvp = mrv[cur_player][rv_cur[cur_player]];
                  break;
                }
            }
          ui_message_naki (cur_player, 2);
          hand_kan (gp, cur_player, rvp->out, rv_find_player (gp, rvp));
          rvp->next = nrvp;
          nrvp->prev = rvp;
          rvp = nrvp;

          if (result_check_sukan (gp) || !nrvp->in)
            {
              breakf = 1;
              break;
            }
          mt_push (gp, rvp->in, 1);
          hand_insert (gp, cur_player, rvp->in);
          cur_hand =
            &hand[cur_player].closed[hand[cur_player].closed_num - 1];
          ch_hand[cur_player] = 1;
          ui_draw (gp, 0);
          get_event (gp, cur_player, rv_cur[cur_player], 0);
          break;
        case R_RON:
          {
            int ron_player[3];
            int result_player[4];
            int p;

            nagare = 0;
            j = 0;
            p = cur_player;
            for (i = 0; i < 4; i++)
              {
                result_player[i] = 0;
                if (p != cur_player)
                  {
                    hand_insert (gp, p, rvp->out);
                    ron_player[j] = p;
                    j++;
                  }
                p = (p + 1) % 4;
              }
            tmp_flush_dora (gp);
            ch_dora = 1;
            result_calc (gp, RESULT_AGARI_RON, ron_player, 3);
            for (i = 0; i < 3; i++)
              {
                if (result[i].flag == RESULT_AGARI_RON)
                  {
                    for (j = 0; j < 4; j++)
                      {
                        if (result[i].points[j] > 0)
                          {
                            result_player[j] = 1;
                            ui_message_ron (j);
                            if (i == 0)
                              {
                                result[i].points[j] += rest_point;
                                rest_point = 0;
                                break;
                              }
                          }
                      }
                  }
              }
            for (i = 0; i < 3; i++)
              {
                if (!result_player[ron_player[i]])
                  {
                    hand_delete (gp, ron_player[i], rvp->out, 0);
                  }
              }
            ui_draw (gp, 0);

            tmp_flush_point (gp);
            ui_result (gp);
            breakf = 1;
          }
          break;
        default:
          mt_push (gp, rvp->in, 1);
          switch (T_GET (rvp->attr))
            {
            case T_HN_TSM:
              nagare = 0;
              hand_insert (gp, cur_player, rvp->in);
              ui_message_tumo (cur_player);
              tmp_flush_dora (gp);
              ch_dora = 1;
              ui_draw (gp, 0);
              result_calc (gp, RESULT_AGARI_TUMO, &cur_player, 1);
              for (j = 0; j < 4; j++)
                {
                  if (result[0].points[j] > 0)
                    {
                      result[0].points[j] += rest_point;
                      rest_point = 0;
                    }
                }
              tmp_flush_point (gp);
              ui_result (gp);
              breakf = 1;
              break;
            case T_HN_KAN:
            case T_HN_KAN_C:
              ui_message_naki (cur_player, 2);
              tmp_adddora (gp);
              rv_cur[cur_player]++;
              nrvp = &(rv[cur_player][rv_cur[cur_player]]);
              *nrvp = mrv[cur_player][rv_cur[cur_player]];
              hand_insert (gp, cur_player, rvp->out);
              hand_kan (gp, cur_player, rvp->out, rv_find_player (gp, rvp));
              rvp->next = nrvp;
              nrvp->prev = rvp;
              rvp = nrvp;
              if (result_check_sukan (gp) || !nrvp->in)
                {
                  breakf = 1;
                  break;
                }
              hand_insert (gp, cur_player, rvp->in);
              cur_hand =
                &hand[cur_player].closed[hand[cur_player].closed_num - 1];
              ch_hand[cur_player] = 1;
              ui_draw (gp, 0);
              get_event (gp, cur_player, rv_cur[cur_player], 0);
              break;
#if 0
            case T_RV_RCH:
              hand[cur_player].reach = 1;
              rest_point += 1000;
              ui_message_reach (cur_player);
#endif
            default:
              rv_cur[cur_player]++;
              cur_player = (cur_player + 1) % 4;
              nrvp = &(rv[cur_player][rv_cur[cur_player]]);
              *nrvp = mrv[cur_player][rv_cur[cur_player]];
              rvp->next = nrvp;
              nrvp->prev = rvp;
              rvp = nrvp;
              if (!nrvp->in)
                {
                  breakf = 1;
                  break;
                }
              hand_insert (gp, cur_player, rvp->in);
              cur_hand =
                &hand[cur_player].closed[hand[cur_player].closed_num - 1];
              ch_hand[cur_player] = 1;
              ui_draw (gp, 0);
              get_event (gp, cur_player, rv_cur[cur_player], 0);
              break;
            }
        }
      if (result_check_sufurenta (gp))
        {
          break;
        }
    }
  if (nagare)
    {
      result_calc (gp, RESULT_RYUKYOKU, 0, 0);
      ch_hand[cur_player] = 1;
      ch_river[cur_player] = 1;
      ui_draw (gp, 0);
      ui_result (gp);
      ui_draw (gp, 0);
    }
  if (rvp->in == 0)
    {
      rvp = rvp->prev;
      rvp->next = 0;
      rv_cur[rv_find_player (gp, rvp)]--;
      tmp_flush_dora (gp);
      tmp_flush_point (gp);
    }
  ui_draw (gp, 1);
  get_event (gp, -1, -1, -1);
}

int
rlog (global_t *gp, FILE *fp)
{
  int c, i, j, n, x;
  int d1[1], d2[1], d3[1], d4[1], d5[1], d6[1], d7[1], d8[1], d9[1];
  char *p;
  char buf[80], s1[80], s2[80], s3[80], s4[80], s5[80];
  int rule;

  for (i = 0; i < 4; i++)
    pplayer[i] = i;
  while (1)
    {
      if (fgets (buf, sizeof (buf), fp) == 0)
        return (-1);
      if (sscanf (buf, "(%d,%d,%d) '", d1, d2, d3) == 3)
        {
          msg_put (*d1, *d2, *d3, buf + 10);
        }
      if (sscanf (buf, "rule: %d", &rule) == 1)
        {
          expand_rule (&rule);
        }
      else
        if (((c =
              sscanf (buf, "%s %d %s %d %s %s %d", s1, d1, s2, d2, s3, s4,
                      d3)) == 7) && !strcmp ("¶É", s2)
            && !strcmp ("¶¡Â÷", s4))
        {
          if (!strcmp ("Åì", s1))
            big_turn = 0;
          else if (!strcmp ("Æî", s1))
            big_turn = 1;
          else if (!strcmp ("À¾", s1))
            big_turn = 2;
          else if (!strcmp ("ËÌ", s1))
            big_turn = 3;
          small_turn = *d1 - 1;
          small_turn_cnt = *d2;
          rest_point = *d3;
          break;
        }
    }
  while (1)
    {
      if (fgets (buf, sizeof (buf), fp) == 0)
        return (-1);
      if (((c = sscanf (buf, "\t%s %s", s1, s2)) == 2) &&
          !strcmp ("¥É¥é", s1))
        {
          p = buf + 6;
          for (i = 0; p && (c = str2pai (p)) > 0; i++, p += 3)
            {
              tmp_setdora (gp, i, c);
            }
          tmp_adddora (gp);
          break;
        }
    }
  while (1)
    {
      if (fgets (buf, sizeof (buf), fp) == 0)
        return (-1);
      if (((c = sscanf (buf, "\t%s %s", s1, s2)) == 2) &&
          !strcmp ("¥¦¥é¥É¥é", s1))
        {
          p = buf + 10;
          for (i = 0; p && (c = str2pai (p)) > 0; i++, p += 3)
            {
              tmp_seturadora (gp, i, c);
            }
          break;
        }
    }

  for (i = 0; i < 4; i++)
    {
      while (1)
        {
          if (fgets (buf, sizeof (buf), fp) == 0)
            return (-1);
          if (((c = sscanf (buf, "%s %s %d(%d)", s1, s2, d1, d2) == 4) &&
               !strcmp ("²È", s1 + 2)))
            {
              strcpy (player[i].name, s2);
              player[i].gpoint = *d1 - *d2;
              player[i].ppoint = 0;
              tmp_setppoint (gp, i, *d2);
              break;
            }
        }
      if (fgets (s1, sizeof (buf), fp) == 0)
        return (-1);
      if (fgets (s2, sizeof (buf), fp) == 0)
        return (-1);
      if (fgets (s3, sizeof (buf), fp) == 0)
        return (-1);
      if (fgets (s4, sizeof (buf), fp) == 0)
        return (-1);
      if (fgets (s5, sizeof (buf), fp) == 0)
        return (-1);
      for (j = 0; j < 13; j++)
        {
          init_hand[i].closed[j] = c = str2pai (s1 + 1 + (j * 3));
          if (c == 0)
            break;
        }
      init_hand[i].closed_num = j;
      hand[i] = init_hand[i];
      for (j = 0; j < RIVER_MAX; j++)
        {
          mrv[i][j].out = c = str2pai (s3 + 1 + (j * 3));
          if (c == 0)
            break;
        }
      n = j;
      for (j = 0; j < n; j++)
        {
          mrv[i][j].in = c = str2pai (s2 + 1 + (j * 3));
          if (c == 3)
            mrv[i][j].in = mrv[i][j].out;
        }
      for (j = 0; j < n; j++)
        {
          mrv[i][j].attr = c = str2attr (s4 + 1 + (j * 3));
        }
      for (j = 0; j < 13; j++)
        {
          last_hand[i].closed[j] = c = str2pai (s5 + 1 + (j * 3));
          if (c == 0)
            break;
        }
      last_hand[i].closed_num = j;
      p = s5 + 1 + j * 3;
      x = 0;
      j = 0;
      while (*p)
        {
          if (c = str2pai (p))
            {
              while (c = str2pai (p))
                {
                  last_hand[i].opened[x][j] = c;
                  j++;
                  p += 3;
                }
              x++;
              j = 0;
            }
          else
            p++;
        }
    }
  return 0;
}

int msg_skip;
int msg_num;
char *msg[RIVER_MAX * 8];

void
get_event (global_t *gp, int x, int y, int z)
{
  int c;
  char buf[80];

  if (x >= 0)
    {
      if (msg_get (x, y, z, buf))
        {
          if (buf[0])
            comment (4, 1, buf);
        }
      c = ui_key ();
      if (c == 'q')
        term ();
      if (c == 'c')
        {
          comment (4, 0, buf);
          msg_put (x, y, z, buf);
        }
    }
  else
    {
      c = ui_key ();
      if (c == 'q')
        term ();
    }
}

void
msg_clear ()
{
  int i, x, y, z;
  for (i = 0; i < RIVER_MAX * 8; i++)
    {
      if (msg[i] && ofp)
        {
          x = i / (RIVER_MAX * 2);
          y = (i % (RIVER_MAX * 2)) / 2;
          z = i % 2;
          fprintf (ofp, "(%01d,%02d,%d) '%s\n", x, y, z, msg[i]);
        }
      if (msg[i])
        free (msg[i]);
      msg[i] = 0;
    }
  msg_num = 0;
}

void
msg_put (int x, int y, int z, char *buf)
{
  int i, len;

  msg_num++;
  i = x * (RIVER_MAX * 2) + y * 2 + z;
  msg[i] = malloc (strlen (buf) + 1);
  strcpy (msg[i], buf);
  len = strlen (msg[i]);
  if (len > 0 && msg[i][len - 1] == '\n')
    {
      msg[i][len - 1] = 0;
    }
}

int
msg_get (int x, int y, int z, char *buf)
{
  int i;
  i = x * (RIVER_MAX * 2) + y * 2 + z;
  if (!msg[i])
    {
      buf[0] = 0;
      return msg_skip;
    }
  if (!strcmp (msg[i], "skip"))
    {
      msg_skip = 1;
      buf[0] = 0;
      return msg_skip;
    }
  if (ofp)
    fprintf (ofp, "debug !%s!\n", msg[i]);
  msg_skip = 0;
  strcpy (buf, msg[i]);
  return 1;
}

int tmp_ppoint[4];
int tmp_dora_cnt;
int tmp_dora[5];
int tmp_uradora[5];

void
tmp_setppoint (global_t *gp, int i, int c)
{
  tmp_ppoint[i] = c;
}

void
tmp_setdora (global_t *gp, int i, int c)
{
  tmp_dora[i] = c;
}

void
tmp_adddora (global_t *gp)
{
  mt_setdora (gp, tmp_dora_cnt, tmp_dora[tmp_dora_cnt]);
  tmp_dora_cnt++;
  ch_dora = 1;
}

void
tmp_seturadora (global_t *gp, int i, int c)
{
  tmp_uradora[i] = c;
}

void
tmp_flush_dora (global_t *gp)
{
  int i;
  for (i = 0; i < 5; i++)
    {
      mt_seturadora (gp, i, tmp_uradora[i]);
      tmp_uradora[i] = 0;
      tmp_dora[i] = 0;
    }
  tmp_dora_cnt = 0;
  ch_dora = 1;
}

void
tmp_flush_point (global_t *gp)
{
  int i;
  for (i = 0; i < 4; i++)
    {
      player[i].ppoint = tmp_ppoint[i];
      player[i].gpoint += player[i].ppoint;
    }
  ch_point = 1;
}
