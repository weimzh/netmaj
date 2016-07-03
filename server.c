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
 * 5-7-2006 Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include <signal.h>
#include <sys/wait.h>
#ifndef SIGCHLD
#define SIGCHLD SIGCLD
#endif
#include "pai.h"
#include "ahand.h"
#include "global.h"

#define USAGE "\
usage:\n\
    server [-p port-number] [-l logfile] [-s seed] [-H|-Q]\n"

int server_id;
int port = PORT_NUMBER;
int protocol_version = PROTOCOL_VERSION;
char *plogfile;
char *logfile;
int limit = -1;
void term ();

FILE *df;
FILE *log_server;

int debug = 0;

int rule;
int server_max = 5;
int server_num = 0;

global_t *gp_server;
char *msgGets ();

extern int tonpu;

void
child_exit ()
{
  int status;
  if (plog)
    fprintf (plog, "child_exit\n");
#ifdef USE_WAIT3
  while (wait3 (&status, WNOHANG, 0) > 0)
    {
#else
  while (waitpid (-1, &status, WNOHANG) > 0)
    {
#endif
      if (plog)
        fprintf (plog, "child_exit status %08x\n", status);
      if (WIFEXITED (status))
        server_num--;
    }
}

main (argc, argv)
     int argc;
     char *argv[];
{
  SOCKETIO *ssp, *csp;
  char buf[MSG_MAX];
  char name[MSG_MAX];
  int flags;
  int i, j, k, c, new, cid;
  int seed = 0, s;
  int non_player;
  global_t *gp;

  gp = gp_server = global_alloc ();
  signal (SIGINT, term);
  signal (SIGTERM, term);
  signal (SIGCHLD, child_exit);

  while ((c = getopt (argc, argv, "p:l:L:s:n:N:HQD")) != EOF)
    {
      switch (c)
        {
        case 'p':
          port = atoi (optarg);
          break;
        case 'l':
          logfile = optarg;
          break;
        case 'L':
          plogfile = optarg;
          break;
        case 's':
          seed = atoi (optarg);
          break;
        case 'n':
          limit = atoi (optarg);
          break;
        case 'H':
          tonpu = 0;
          break;
        case 'Q':
          tonpu = 1;
          break;
        case 'D':
          debug = 1;
          break;
        case 'N':
          server_max = atoi (optarg);
          break;
        default:
          fprintf (stderr, USAGE);
          exit (1);
        }
    }

  if (!(ssp = ServerSock (port)))
    {
      fprintf (stderr, "cannot open server socket:%d\n", port);
      exit (1);
    }
  pf_init (gp, "server", argv[0]);
  rule_init (gp);
  pack_rule (&rule);

  if (debug == 0)
    {
      if (!fork ())
        {
          fclose (stdin);
          fclose (stdout);
          fclose (stderr);
          if (plogfile)
            plog = fopen (plogfile, "a");
        }
      else
        {
          exit (0);
        }
    }

  s = seed;
  if (!s)
    s = time (0);
  srandom (s);
  while (1)
    {
      for (i = 0; i < 4; i++)
        strcpy (player[i].name, "---------");
      i = 0;
      while (i < 4)
        {
          SockSelect ((double) (-1.0), "r");
          if (SockIsRead (ssp) > 0)
            {
              csp = AcceptSock (ssp);
              for (;;)
                {
                  new = random () % 4;
                  if (!player[new].sp)
                    break;
                }
              player[new].sp = csp;
              if (!msgGets (gp, buf, sizeof (buf) - 1, new))
                {
                  player[new].sp = 0;
                  SockClose (csp);
                  continue;
                }

              if (sscanf (buf, MSG_CONNECT, name, &flags) >= 2)
                {
                  if (server_num >= server_max)
                    {
                      SockClose (csp);
                      player[new].sp = 0;
                      continue;
                    }
                  player[new].flags = flags;
                  strncpy (player[new].name, name, PNAME_MAX);
                  player[new].name[PNAME_MAX - 1] = 0;
                  sprintf (buf, MSG_CONNECT_DONE, new, rule,
                           PROTOCOL_VERSION);
                  msgWrites (gp, buf, new);
                  i++;
                  sprintf (buf, MSG_CONNECT_PLAYER, player[0].name,
                           player[1].name, player[2].name, player[3].name);
                  for (j = 0; j < 4; j++)
                    {
                      if (player[j].sp)
                        msgWrites (gp, buf, j);
                    }
                }
              else if (!strcmp (buf, MSG_KILL))
                {
                  term ();
                }
              else if (sscanf (buf, MSG_OPT, name, name + 1) > 0)
                {
                  optprot (gp, new, name[0], name + 1);
                  player[new].sp = 0;
                  SockClose (csp);
                  continue;
                }
            }
          for (j = 0; j < 4; j++)
            {
              if (player[j].sp && (SockIsRead (player[j].sp) > 0))
                {
                  if (talk_pg (gp, j))
                    {
                      SockClose (player[j].sp);
                      player[j].sp = NULL;
                      strcpy (player[j].name, "---------");
                      i--;
                      sprintf (buf, MSG_CONNECT_PLAYER, player[0].name,
                               player[1].name, player[2].name,
                               player[3].name);
                      for (k = 0; k < 4; k++)
                        {
                          if (player[k].sp)
                            msgWrites (gp, buf, k);
                        }
                    }
                }
            }
        }

      if (logfile)
        {
          log_server = fopen (logfile, "a");
          if (log_server)
            {
              fprintf (log_server, "start at %d with %s %s %s %s\n", time (0),
                       player[0].name, player[1].name, player[2].name,
                       player[3].name);
              fclose (log_server);
              log_server = 0;
            }
        }
      if (plog)
        {
          fclose (plog);
          plog = 0;
        }
      server_id++;
      if ((cid = fork ()) == 0)
        {
          if (logfile)
            {
              sprintf (buf, "%s.%03d", logfile, server_id);
              log_server = fopen (buf, "a");
            }
          if (plogfile)
            {
              sprintf (buf, "%s", plogfile);
              plog = fopen (buf, "a");
            }
          SockClose (ssp);
          s = seed;
          if (!s)
            s = time (0);
          srandom (s);
          if (log_server)
            fprintf (log_server, "seed: %d\n", s);
          game (gp);
          non_player = 1;
          for (i = 0; i < 4; i++)
            {
              if (player[i].flags & M_TALK)
                non_player = 0;
            }
          if (!non_player)
            after_game (gp);
          term ();
        }
      else if (cid > 0)
        {
          server_num++;
        }
      if (plogfile)
        plog = fopen (plogfile, "a");
      for (i = 0; i < 4; i++)
        {
          SockClose (player[i].sp);
          player[i].sp = 0;
        }
      if (server_id == limit)
        term ();
    }
}

talk_pg (gp, from)
     global_t *gp;
{
  char buf[MSG_MAX];
  char msg[MSG_MAX];

  if (!msgGets (gp, buf, sizeof (buf) - 1, from))
    return -1;
  if (strncmp (MSG_COMMENTX, buf, strlen (MSG_COMMENTX)) == 0)
    {
      talk (gp, from, buf + strlen (MSG_COMMENTX));
    }
  else if (strncmp (MSG_AUTO0, buf, 4) == 0)
    {
      runauto (buf + 4);
    }
  else if (sscanf (buf, MSG_OPT, msg, msg + 1) > 0)
    {
      optprot (gp, from, msg[0], msg + 1);
    }
  return 0;
}

runauto (buf)
     char *buf;
{
  char *p;
  char cmd[256];

  if (p = rindex (buf, '\n'))
    *p = 0;
  if (buf[0])
    {
      sprintf (cmd, "%s/auto -p %d -n %s &\n", NETMAJ_LIB, port, buf);
    }
  else
    {
      sprintf (cmd, "%s/auto -p %d&\n", NETMAJ_LIB, port);
    }
  system (cmd);
}

talk (gp, from, msg)
     global_t *gp;
     char *msg;
{
  int i;
  char buf[MSG_MAX];
  char *p;

  if (p = index (msg, '\n'))
    *p = 0;
  sprintf (buf, MSG_COMMENT2, from, msg);
  for (i = 0; i < 4; i++)
    if (i != from && player[i].sp && (player[i].flags & M_TALK))
      msgWrites (gp, buf, i);
}

void
term ()
{
  int i;
  global_t *gp;

  gp = gp_server;

  for (i = 0; i < 4; i++)
    if (player[i].sp)
      {
        msgWrites (gp, MSG_KILL, i);
        SockClose (player[i].sp);
      }
  if (plog)
    fclose (plog);
  fclose (stdout);
  exit (1);
}

/* comm wrapper */

int
msgWrites (gp, buf, p)
     global_t *gp;
     char *buf;
     int p;
{
  if (plog)
    {
      fprintf (plog, "%d>%s", p, buf);
      fflush (plog);
    }

  return SockWrites (buf, player[p].sp);
}

char *
msgGets (gp, buf, len, p)
     global_t *gp;
     char *buf;
     int len;
     int p;
{
  char *ret;
  ret = SockGets (buf, len, player[p].sp);
  if (plog)
    {
      fprintf (plog, "%d<%s", p, buf);
      fflush (plog);
    }
  return ret;
}

msgSelect (gp, time_out)
     global_t *gp;
     int time_out;
{
  return SockSelect ((10.0) * time_out, "r");
}

msgIsRead (gp, p)
     global_t *gp;
{
  return SockIsRead (player[p].sp);
}

optprot (gp, from, opt, msg)
     global_t *gp;
     char *msg;
{
  char buf[128];
  char buf2[128];
  int i, j, n;

  switch (opt)
    {
    case 's':
      n = 0;
      for (i = 0; i < 4; i++)
        {
          if (player[i].sp)
            n++;
        }
      sprintf (buf, "member:%d tonpu:%d rule:%08x", n - 1, tonpu, rule);
      sprintf (buf2, MSG_OPT, opt, buf);
      msgWrites (gp, buf2, from);
      break;
    case 't':
      n = 0;
      for (i = 0; i < 4; i++)
        {
          if (player[i].sp)
            n++;
        }
      sprintf (buf, "member:%d tonpu:%d rule:%08x server:%d",
               (server_num >= server_max) ? -1 : n - 1, tonpu, rule,
               server_num);
      sprintf (buf2, MSG_OPT, opt, buf);
      msgWrites (gp, buf2, from);
      break;
    }
}

after_game (gp)
     global_t *gp;
{
  int i, j, a, b, pn;
  int break_flag = 0;
  char buf[MSG_MAX];

  for (;;)
    {
      msgSelect (gp, -1);
      for (i = 0; i < 4; i++)
        {
          if (!player[i].sp || !msgIsRead (gp, i))
            continue;
          if (!msgGets (gp, buf, sizeof (buf) - 1, i) ||
              !strcmp (buf, MSG_KILL))
            {
              SockClose (player[i].sp);
              strcpy (player[i].name, "---------");
              player[i].sp = 0;
              pn = 0;
              for (j = 0; j < 4; j++)
                {
                  if (player[j].sp && (player[j].flags & M_TALK))
                    pn++;
                }
              if (pn == 0)
                term ();
              sprintf (buf, MSG_CONNECT_PLAYER, player[0].name,
                       player[1].name, player[2].name, player[3].name);
              for (j = 0; j < 4; j++)
                {
                  if (player[j].sp)
                    msgWrites (gp, buf, j);
                }
            }
          else if (strncmp (MSG_COMMENTX, buf, strlen (MSG_COMMENTX)) == 0)
            {
              talk (gp, i, buf + strlen (MSG_COMMENTX));
            }
        }
    }
}
