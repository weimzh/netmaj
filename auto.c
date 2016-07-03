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
 * 5/7/2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#define USAGE "\
usage:\n\
    auto [-h host-name] [-p port-number] [-n player-name]\n"

#include "global.h"

char *host = "localhost";
char name[12] = "auto";
FILE *df;

int port = PORT_NUMBER;
int protocol_version = 0;

global_t *gp_auto;

int rule = 0;

char *msgGets (global_t *gp, char *buf, int len, int p);
int msgWrites (global_t *gp, char *buf, int p);
void noisy_talk (global_t *gp);

int noisy = 0;

void
term ()
{
  if (df)
    fprintf (df, "connection closed\n");
  fclose (stdin);
  if (plog)
    fclose (plog);
  if (df)
    fclose (df);
  pf_term (gp_auto);
  exit (0);
}

int
main (int argc, char *argv[])
{
  result_t y;
  char buf[MSG_MAX];
  SOCKETIO *ssp;
  int st, c;
  char *p;
  int res, how;
  global_t *gp;
  int ver;

  gp = gp_auto = global_alloc ();
  signal (SIGTERM, term);
  signal (SIGINT, term);

  srandom (time (0));
  while ((c = getopt (argc, argv, "p:h:l:L:n:")) != EOF)
    {
      switch (c)
        {
        case 'n':
          strncpy (name, optarg, 10);
          break;
        case 'p':
          port = atoi (optarg);
          break;
        case 'h':
          host = optarg;
          break;
        case 'l':
          df = fopen (optarg, "a");
          break;
        case 'L':
          plog = fopen (optarg, "a");
          break;
        default:
          fprintf (stderr, USAGE);
          exit (1);
        }
    }

  pf_init (gp, name, argv[0]);
  pf_param_strn (gp, "name", name, 10);
  pf_param (gp, "noisy", &noisy);

  analize_init (gp);

  ssp = ConnectSock (host, port);
  if (!ssp)
    {
      sleep (5);
      ssp = ConnectSock (host, port);
    }
  if (!ssp)
    {
      exit (0);
    }
  player[0].sp = ssp;
  sprintf (buf, MSG_CONNECT, name, 0);
  msgWrites (gp, buf, 4);
  if (!msgGets (gp, buf, sizeof (buf) - 1, 4)
      || sscanf (buf, MSG_CONNECT_DONE, &self, &rule, &ver) < 1)
    {
      SockClose (ssp);
      exit (0);
    }
  expand_rule (&rule);
  if (df)
    fprintf (df, "connectied %d\n", self);

  while (msgGets (gp, buf, sizeof (buf) - 1, 4))
    {
      st = recv_message (gp, buf);
      switch (st)
        {
        case ACT_RES:
          res = analize_res (gp, &how);
          sprintf (buf, MSG_REPL, res, how);
          msgWrites (gp, buf, 4);
          break;
        case ACT_SEL:
          analize (gp, vself);
          if (noisy)
            noisy_talk (gp);
          sprintf (buf, MSG_ASK, ask_attr, ask_pai);
          msgWrites (gp, buf, 4);
          break;
        case ACT_PEND:
          pf_pend (gp);
          if (df)
            log_play (gp, df);
          break;
        case ACT_GEND:
          pf_gend (gp);
          break;
        case ACT_COMMENT:
          break;
        }
    }
  term ();
}

int
msgWrites (global_t *gp, char *buf, int p)
{
  if (p == 4)
    p = 0;
  if (plog)
    fprintf (plog, "%d>%s", p, buf);
  return SockWrites (buf, player[p].sp);
}

char *
msgGets (global_t *gp, char *buf, int len, int p)
{
  char *ret;

  if (p == 4)
    p = 0;
  ret = SockGets (buf, len, player[p].sp);
  if (plog)
    fprintf (plog, "%d<%s", p, buf);
  return ret;
}

int
msgSelect (global_t *gp, int time_out)
{
  return SockSelect ((10.0) * time_out, "r");
}

int
msgIsRead (global_t *gp, int p)
{
  return SockIsRead (player[p].sp);
}

void
noisy_talk (global_t *gp)
{
  int i, n;
  int kinds[K_MAX];
  char buf[256];

  n = rv_cur[vself];
  if (n > 1 && P_KIND (rv[vself][n - 1].out) == P_KIND (ask_pai)
      && rv[vself][n].in == ask_pai)
    {
      sprintf (buf, MSG_COMMENT, "かぶった！");
      msgWrites (gp, buf, 4);
    }
  else if (n > 4 && P_KIND (rv[vself][n - 4].out) == P_KIND (ask_pai)
           && P_KIND (rv[vself][n - 3].out) == P_KIND (rv[vself][n - 1].out))
    {
      sprintf (buf, MSG_COMMENT, "しんぶんし");
      msgWrites (gp, buf, 4);
    }
  else if (n > 2 && P_KIND (rv[vself][n - 2].out) == P_KIND (ask_pai))
    {
      sprintf (buf, MSG_COMMENT, "トマト");
      msgWrites (gp, buf, 4);
    }
  else
    {
      for (i = 0; i < K_MAX; i++)
        kinds[i] = 0;
      for (i = 0; i < n; i++)
        {
          kinds[P_KIND (rv[vself][i].out)]++;
        }
      if (kinds[P_KIND (ask_pai)] == 2)
        {
          sprintf (buf, MSG_COMMENT, "アンコ！");
          msgWrites (gp, buf, 4);
        }
      else if (kinds[P_KIND (ask_pai)] == 3)
        {
          sprintf (buf, MSG_COMMENT, "カンツ！");
          msgWrites (gp, buf, 4);
        }
    }
}
