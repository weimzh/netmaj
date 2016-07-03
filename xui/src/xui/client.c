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

#define USAGE "\
usage:\n\
    client [-h host-name] [-p port-number] [-n player-name] [-l logfile] \n"

#include <setjmp.h>
#include <signal.h>
#ifndef SIGCHLD
#define SIGCHLD SIGCLD
#endif

#ifndef DEF_OPTARG
#define DEF_OPTARG
#endif

#include "pai.h"
#include "global.h"
#include "result.h"

global_t *gp_client;
FILE *df;
FILE *logfile;
char *host;
int port = PORT_NUMBER;
int protocol_version = 0;
int play_speed = 10;
int auto_stop = 1;
int auto_play = 0;
int auto_after_reach = 1;
void term ();
char *msgGets (global_t *gp, char *buf, int len, int p);
void msgConvertOut (char *buf);

int readable_socket;
int readable_key;
int rv_fixed = 0;
pai_t *cur_hand;
int view_river_all;
int rule = 0;

jmp_buf reconnect;

char *program;
int proxy_mode = 0;
int input_mode = 0;
SOCKETIO *proxy_sp = 0;
SOCKETIO *input_sp = 0;
int proxy_port = PROXY_PORT;
int input_port = INPUT_PORT;
int read_from_input = 0;
int proxy_error = 0;

int getopt (int, char *const *, const char *);

void
init (global_t *gp, char *board)
{
  pf_param (gp, "play-speed", &play_speed);
  pf_param (gp, "auto-stop", &auto_stop);
  global_init (gp);
  ui_init (gp, PROG_NETMAJ, board);
  signal (SIGTERM, term);
  signal (SIGINT, term);
  signal (SIGCHLD, SIG_IGN);
  fclose (stderr);
  ui_draw (gp, 1);
}

void
term ()
{
  ui_term ();
  if (plog)
    fclose (plog);
  if (df)
    fclose (df);
  pf_term (gp_client);
  if (input_sp)
    SockWrites (MSG_KILL, input_sp);
  exit (0);
}

char name[12];

int
main (int argc, char *argv[])
{
  char buf[MSG_MAX];
  SOCKETIO *ssp;
  int c, st, which, i, j, k;
  char *n, *p;
  int ver;
  int flag;
  int res, how;
  global_t *gp;
  char *board = "";

  gp = gp_client = global_alloc ();

  host = "localhost";
  n = getenv ("LOGNAME");
  if (!n)
    {
      n = "you";
    }
  flag = 0;
  program = argv[0];

  while ((c = getopt (argc, argv, "p:h:sd:L:n:l:T:ti")) != EOF)
    {
      switch (c)
        {
        case 'p':
          port = atoi (optarg);
          break;
        case 'n':
          n = optarg;
          break;
        case 'h':
          host = optarg;
          break;
        case 's':
          flag = 1;
          break;
        case 'd':
          df = fopen (optarg, "a");
          break;
        case 'L':
          plog = fopen (optarg, "a");
          break;
        case 'l':
          logfile = fopen (optarg, "a");
          break;
        case 'T':
          board = optarg;
          break;
        case 't':
          proxy_mode = !proxy_mode;
          break;
        case 'i':
          input_mode = !input_mode;
          break;
        default:
          fprintf (stderr, USAGE);
          exit (1);
          break;
        }
    }

  strncpy (name, n, 10);
  pf_init (gp, name, argv[0]);
  pf_param_strn (gp, "name", name, 10);

  analize_init (gp);

  srandom (time (0));

  init (gp, board);
  setjmp (reconnect);

retry:
  readable_socket = 0;
  readable_key = 0;
  read_from_input = 0;
  ssp = 0;
  in_game = 0;

  for (;;)
    {
      if (proxy_mode)
        {
          ssp = proxy_sp = ConnectSock ("localhost", proxy_port);
        }
      else
        {
          ssp = ConnectSock (host, port);
        }
      if (ssp)
        break;
      ui_message_connecting ();
    }
  player[0].sp = ssp;
  ui_message_connect ();
  if (input_mode && !input_sp)
    {
      input_sp = ConnectSock ("localhost", input_port);
    }

  if (proxy_sp)
    {
      sprintf (buf, MSG_HOST, host, port);
      msgWrites (gp, buf, 4);
    }

  sprintf (buf, MSG_CONNECT, name, flag | M_TALK);
  msgWrites (gp, buf, 4);

  if (!msgGets (gp, buf, sizeof (buf) - 1, 4)
      || (i = sscanf (buf, MSG_CONNECT_DONE, &self, &rule, &ver)) < 1)
    {
      SockClose (ssp);
      ui_message_disconnect ();
      if (plog)
        fprintf (plog, "cant read (1)\n");
      if (!proxy_error)
        goto retry;
      term ();
    }
  if (i >= 3)
    {
      protocol_version = ver & 0xff;
    }
  expand_rule (&rule);

  for (;;)
    {
      ui_event_wait (gp, -1);
      if (readable_socket)
        {
          readable_socket = 0;
          if (!msgGets (gp, buf, sizeof (buf) - 1, 4))
            {
              ui_message_disconnect ();
              if (plog)
                fprintf (plog, "cant read (2)\n");
              SockClose (player[0].sp);
              if (!proxy_error)
                goto retry;
              term ();
            }
          if (read_from_input)
            {
              msgWrites (gp, buf, 4);
              read_from_input = readable_socket = 0;
              continue;
            }
          st = recv_message (gp, buf);
          switch (st)
            {
            case ACT_RES:
              ui_draw (gp, 0);
              if (T_GET (rvp->attr) == T_RV_RCH)
                ui_message_reach (pplayer[cur_player]);

              res = analize_res (gp, &how);
              if (auto_play == 0 || (auto_play == 2 && (res == R_RON)))
                {
                  res = ui_res (gp, &how);
                }
              else
                {
                  xsleep (play_speed);
                }
              sprintf (buf, MSG_REPL, res, how);
              msgWrites (gp, buf, 4);
              break;
            case ACT_SEL:
              analize (gp, vself);
              if (auto_play == 0 ||
                  (auto_play == 2 &&
                   (reach_can_kan (gp, vself, ask_pai)
                    || ask_attr == T_HN_TSM)))
                {
                  ask_attr = T_RV;
                  ui_choice (gp);
                }
              else
                {
                  xsleep (play_speed);
                }
              sprintf (buf, MSG_ASK, ask_attr, ask_pai);
              msgWrites (gp, buf, 4);
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
              if (auto_play)
                xsleep (play_speed * 5);
              else
                ui_key ();
              break;
            case ACT_CONNECT:
              ch_point = 1;
              ui_message_player (gp);
              ui_draw (gp, 0);
              break;
            case ACT_GEND:
              pf_gend (gp);
              ui_message_gend (gp);
              break;
            case ACT_PLAY:
              ui_draw (gp, 1);
              ui_message_play (gp);
              break;
            case ACT_COMMENT:
              if ((0 <= comment_from) && (comment_from < 4))
                {
                  /* from player */
                  msgConvertOut (comment_body);
                  comment (comment_from, 1, comment_body, 20);
                }
              else
                {
                  /* from server */
                  popup (comment_body, -1);
                }
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
      if (readable_key)
        {
          readable_key = 0;
          ui_menu (gp, ui_key ());
        }
    }
}

int
msgWrites (global_t *gp, char *buf, int p)
{
  int ret;
  if (p == 4)
    p = 0;

  if (plog)
    fprintf (plog, "%d>%s", p, buf);
  ret = SockWrites (buf, player[p].sp);
  return ret;
}

char *
msgGets (global_t *gp, char *buf, int len, int p)
{
  char *ret;

  if (input_sp && SockIsRead (input_sp))
    {
      ret = SockGets (buf, len, input_sp);
      if (ret)
        {
          read_from_input = 1;
          return ret;
        }
      else
        {
          SockClose (input_sp);
          input_sp = 0;
        }
    }
  if (p == 4)
    p = 0;
  ret = SockGets (buf, len, player[p].sp);

  if (!ret)
    {
      if (proxy_sp)
        proxy_error = 1;
      return ret;
    }
  if (plog)
    fprintf (plog, "%d<%s", p, buf);
  if (!strcmp (MSG_KILL, buf))
    {
      return 0;
    }
  return ret;
}

void
msgConvertIn (char *buf)
{
  while (*buf)
    {
      if (*buf == ' ')
        *buf = '@';
      buf++;
    }
}

void
msgConvertOut (char *buf)
{
  while (*buf)
    {
      if (*buf == '@')
        *buf = '_';
      buf++;
    }
}

void
msgTalk (global_t *gp, char *buf)
{
  char msg[256];
  msgConvertIn (buf);
  sprintf (msg, MSG_COMMENT, buf);
  msgWrites (gp, msg, 4);
}

int
callback_fd (global_t *gp)
{
  int fd = 0;

  if (player[0].sp)
    {
      if (SockIsRead (player[0].sp))
        {
          readable_socket = 1;
          return -1;
        }
      fd = player[0].sp->sd;
    }
  return fd;
}

int
callback_fd2 (global_t *gp)
{
  int fd = 0;

  if (input_sp)
    {
      if (SockIsRead (input_sp))
        {
          readable_socket = 1;
          return -1;
        }
      fd = input_sp->sd;
    }
  return fd;
}

void
callback (global_t *gp)
{
  char buf[256];
  char buf2[256];

  readable_socket = 0;

  if (!msgGets (gp, buf, sizeof (buf) - 1, 4))
    {
      ui_message_disconnect ();
      if (plog)
        fprintf (plog, "cant read (3)\n");
      SockClose (player[0].sp);
      if (!proxy_error)
        longjmp (reconnect, 1);
      term ();
    }
  if (read_from_input)
    {
      msgWrites (gp, buf, 4);
      read_from_input = readable_socket = 0;
    }
  else if (recv_message (gp, buf) == ACT_COMMENT
           && (0 <= comment_from) && (comment_from < 4))
    {
      /* from player */
      msgConvertOut (comment_body);
      comment (comment_from, 1, comment_body, 20);
    }
  readable_socket = 0;
}

void
execute_auto (char *com)
{
  char buf[256];
  char *p;

  if (proxy_sp)
    {
      if (com[0] == '-' && com[1] == 'n')
        {
          sprintf (buf, MSG_AUTO, com + 2);
        }
      else
        {
          if (p = index (com, ' '))
            {
              p++;
              if (p[0] == '-' && p[1] == 'n')
                {
                  sprintf (buf, MSG_AUTO, p + 2);
                }
              else
                {
                  sprintf (buf, MSG_AUTO, "");
                }
            }
          else
            {
              sprintf (buf, MSG_AUTO, "");
            }
        }
      msgWrites (gp_client, buf, 4);
    }
  else
    {
      if (com[0] == 0)
        sprintf (buf, "%s -h %s -p %d &", "auto", host, port);
      else if (com[0] == '-')
        sprintf (buf, "%s %s -h %s -p %d &", "auto", com, host, port);
      else
        sprintf (buf, "%s -h %s -p %d &", com, host, port);
      system (buf);
    }
}

void
do_disconnect (global_t *gp)
{
  msgWrites (gp, MSG_KILL, 4);
}
