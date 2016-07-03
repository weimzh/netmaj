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
 *  Koji Suzuki   : suz@kt.rim.or.jp
 */

#ifndef DEF_OPTARG
#define DEF_OPTARG
#endif

#include <signal.h>
#include "global.h"

#define USAGE "\
usage:\n\
    xnetmaj1 [-l logfile] [-H|-Q]\n"

int protocol_version = PROTOCOL_VERSION;
int readable_key;
int readable_socket;
int rv_fixed = 0;
pai_t *cur_hand;
int view_river_all;

int auto_after_reach = 1;
int auto_play = 0;
int auto_stop = 1;
int play_speed = 1;
extern int tonpu;

FILE *logfile;
FILE *log_server;
FILE *plog = 0;

int getopt (int, char *const *, const char *);

callback ()
{
}

callback_fd ()
{
  return -2;
}

callback_fd2 ()
{
  return -2;
}

char *msgGets ();

void
term ()
{
  ui_term ();
  exit (0);
}

execute_auto ()
{
}

do_disconnect (gp)
     global_t *gp;
{
  term ();
}

talk (gp, from, msg)
     global_t *gp;
     char *msg;
{
}

runauto (buf)
     char *buf;
{
}

msgTalk (gp, buf)
     global_t *gp;
{
}

init (gp, board)
     global_t *gp;
     char *board;
{
  pf_param (gp, "play-speed", &play_speed);
  ui_init (gp, PROG_NETMAJ1, board);
  signal (SIGTERM, term);
  signal (SIGINT, term);
  signal (SIGCHLD, SIG_IGN);
  fclose (stderr);
  ui_draw (gp, 1);
}

user_prog (gp, buf)
     global_t *gp;
     char *buf;
{
  char msg[256];
  int st, c;
  int i, j, k;
  int res, how;

  st = recv_message (gp, buf);
  switch (st)
    {
    case ACT_RES:
      ui_draw (gp, 0);
      if (T_GET (rvp->attr) == T_RV_RCH)
        ui_message_reach (pplayer[cur_player]);
      if (auto_play)
        {
          res = analize_res (gp, &how);
          xsleep (play_speed);
        }
      else
        {
          res = ui_res (gp, &how);
        }
      sprintf (msg, MSG_REPL, res, how);
      msgWrites (gp, msg, 4);
      break;
    case ACT_SEL:
      if (auto_play == 0 || (auto_play == 2
                             && reach_can_kan (gp, vself, ask_pai)))
        {
          ui_choice (gp);
        }
      else
        {
          analize (gp, vself);
          xsleep (play_speed);
        }
      sprintf (msg, MSG_ASK, ask_attr, ask_pai);
      msgWrites (gp, msg, 4);
      break;
    case ACT_RESULT:
      for (i = 4; i >= 0; i--)
        if (result[i].flag)
          break;
      switch (result[i].flag)
        {
        case RESULT_AGARI_TUMO:
          ui_message_tumo (pplayer[result[i].who]);
          break;
        case RESULT_AGARI_RON:
          ui_message_ron (pplayer[result[i].who]);
          break;
        case RESULT_CYONBO:
        case RESULT_CYONBO_CONT:
          if (cur_player == result[i].who)
            {
              ui_message_tumo (pplayer[result[i].who]);
            }
          else
            {
              ui_message_ron (pplayer[result[i].who]);
            }
          break;
        }
      break;
    case ACT_PEND:
      if (auto_after_reach && auto_play >= 2)
        auto_play -= 2;

      pf_pend (gp);
      ui_result (gp);
      if (auto_play >= 2)
        xsleep (play_speed * 5);
      else
        ui_key ();
      break;
    case ACT_CONNECT:
      ui_message_player (gp);
      ui_draw (gp, 1);
      break;
    case ACT_GEND:
      pf_gend (gp);
      ui_draw (gp, 1);
      ui_message_gend (gp);
      break;
    case ACT_PLAY:
      ui_draw (gp, 1);
      ui_message_play (gp);
      break;
    case ACT_COMMENT:
      break;
    case ACT_FIX:
      rv_fixed = 1;
      ui_draw (gp, 0);
      rv_fixed = 0;
      break;
    case ACT_REACH:
      if (auto_after_reach)
        auto_play += 2;
      ui_draw (gp, 0);
      ui_message_reach (pplayer[vself]);
      break;
    default:
      if (ACT_MSG <= st && st < ACT_MSG + 16)
        {
          int kind;
          i = (st - ACT_MSG) / 4;
          j = (st - ACT_MSG) % 4;
          k = hand[i].opened_kind[j];
          if (k == H_TIE)
            kind = 0;
          if (k == H_PON)
            kind = 1;
          if (k == H_KAN_OPENED)
            kind = 2;
          if (k == H_KAN_CLOSED)
            kind = 2;
          ui_message_naki (pplayer[i], kind);
        }
      ui_draw (gp, 0);
      break;
    }
}

auto_prog (gp, buf)
     global_t *gp;
     char *buf;
{
  char msg[256];
  int st, c;
  int res, how;

  st = recv_message (gp, buf);
  switch (st)
    {
    case ACT_RES:
      res = analize_res (gp, &how);
      sprintf (msg, MSG_REPL, res, how);
      msgWrites (gp, msg, 4);
      break;
    case ACT_SEL:
      analize (gp, vself);
      sprintf (msg, MSG_ASK, ask_attr, ask_pai);
      msgWrites (gp, msg, 4);
      break;
    case ACT_PEND:
      pf_pend (gp);
      break;
    case ACT_GEND:
      pf_gend (gp);
      break;
    case ACT_COMMENT:
      break;
    }
}

global_t *globals[5];

FILE *df;

main (argc, argv)
     int argc;
     char *argv[];
{
  int i, c, j;
  int dmy;
  char *names[4];
  char *board = "";
  global_t *gp;

  while ((c = getopt (argc, argv, "d:l:HQT:")) != EOF)
    {
      switch (c)
        {
        case 'd':
          df = fopen (optarg, "w");
          break;
        case 'l':
          logfile = fopen (optarg, "w");
          break;
        case 'H':
          tonpu = 0;
          break;
        case 'Q':
          tonpu = 1;
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
  srandom (time (0));

  names[0] = getenv ("LOGNAME");
  for (i = 1; i < 4; i++)
    {
      names[i] = "auto";
    }

  for (i = 0; i < 5; i++)
    {
      globals[i] = global_alloc ();
    }
  for (i = 0; i < 4; i++)
    {
      gp = globals[i];
      gp->_self = i;
      for (j = 0; j < 5; j++)
        {
          strcpy (globals[j]->_player[i].name, names[i]);
          globals[j]->_player[i].flags = 0;
        }
      pf_init (gp, gp->_player[i].name, argv[0]);
      if (i == 0)
        pf_param_strn (gp, "name", player[i].name, 10);
      analize_init (gp);
    }
  pf_init (globals[4], "server", argv[0]);
  rule_init (globals[4]);
  for (i = 1; i < 5; i++)
    {
      strcpy (globals[i]->_player[0].name, globals[0]->_player[0].name);
    }

  init (globals[0], board);
  for (;;)
    {
      game (globals[4]);
    }
  term ();
}

char msgs[4][256];

int
msgWrites (gp, buf, p)
     global_t *gp;
     char *buf;
     int p;
{
  int i, q = 0;

  if (p == 4)
    {
      for (i = 0; i < 4; i++)
        if (gp == globals[i])
          q = i;
      strcpy (msgs[q], buf);
    }
  else if (p == 0)
    {
      user_prog (globals[p], buf);
    }
  else
    {
      auto_prog (globals[p], buf);
    }
}

msgSelect (gp, time_out)
     global_t *gp;
     int time_out;
{
  return 1;
}

msgIsRead (gp, p)
     global_t *gp;
{
  return (msgs[p][0] != 0);
}

char *
msgGets (gp, buf, len, p)
     global_t *gp;
     char *buf;
     int len;
     int p;
{
  if (msgs[p][0])
    {
      strcpy (buf, msgs[p]);
      msgs[p][0] = 0;
    }
  return buf;
}
