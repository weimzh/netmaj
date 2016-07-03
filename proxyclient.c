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

#include "global.h"

char host[256] = "localhost";
char name[12] = "auto";

int port = PORT_NUMBER;
int protocol_version = 0;
int debug = 0;

void
term ()
{
  exit (0);
}

int
main (int argc, char *argv[])
{
  fd_set fds;
  char buf[MSG_MAX];
  char nbuf[MSG_MAX];
  SOCKETIO *ssp = 0;
  int st, ret, maxfd;
  char h[MSG_MAX];
  char cmd[256];
  int p, c;
  int i;

  while ((c = getopt (argc, argv, "d")) != EOF)
    {
      switch (c)
        {
        case 'd':
          debug = 1;
          break;
        case '?':
          printf ("usage: proxyclient [-d]\n");
          exit (1);
        }
    }

  printf ("proxyclient READY\n");
  fflush (stdout);

retry:
  for (;;)
    {
      maxfd = 0;
      FD_ZERO (&fds);

      if (debug)
        fprintf (stderr, "# Wait start\n");
      if (!ssp || !SockIsRead (ssp))
        {
          FD_SET (0, &fds);
          if (ssp)
            {
              maxfd = ssp->sd;
              FD_SET (ssp->sd, &fds);
            }
          if (debug)
            fprintf (stderr, "# Wait by socket\n");
          st = select (maxfd + 1, &fds, (fd_set *) 0, (fd_set *) 0, 0);
          if (debug)
            fprintf (stderr, "# Wakeuped\n");
          if (st < 0)
            {
              perror ("select");
            }
        }

      if (FD_ISSET (0, &fds))
        {
          int len;
          if (debug)
            fprintf (stderr, "# reading from client\n");
          ret = (int) xread (0, nbuf, 4);
          if (ret != 4)
            term ();
          nbuf[4] = 0;
          if (sscanf (nbuf, "%03d", &len) != 1 || len < 0 || len > 256)
            term ();
          ret = (int) xread (0, buf, len);
          if (ret != len)
            term ();
          buf[len] = 0;
          if (debug)
            fprintf (stderr, "#> %03d %s", len, buf);
          fflush (stderr);
          if (sscanf (buf, MSG_CONNECT, h, &p) == 2)
            {
              if (ssp)
                {
                  if (debug)
                    fprintf (stderr, "# connection with Server is closed\n");
                  SockClose (ssp);
                  ssp = 0;
                }
              if (debug)
                fprintf (stderr, "# connecting with Server\n");
              for (i = 0; i < 3; i++)
                {
                  ssp = ConnectSock (host, port);
                  if (ssp)
                    break;
                  sleep (2);
                }
              if (!ssp)
                {
                  fprintf (stdout, "%03d %s", strlen (MSG_KILL), MSG_KILL);
                  fflush (stdout);
                  goto retry;
                }
              ret = SockWrites (buf, ssp);
              if (ret != 0)
                {
                  SockClose (ssp);
                  ssp = 0;
                  fprintf (stdout, "%03d %s", strlen (MSG_KILL), MSG_KILL);
                  fflush (stdout);
                  goto retry;
                }
              if (debug)
                fprintf (stderr,
                         "# connection with Server is Established \n");
            }
          else if (sscanf (buf, MSG_HOST, h, &p) == 2)
            {
              if (ssp)
                {
                  if (debug)
                    fprintf (stderr, "# connection with Server is closed\n");
                  SockClose (ssp);
                  ssp = 0;
                }
              strcpy (host, h);
              port = p;
            }
          else if (!strcmp (MSG_KILL, buf))
            {
              if (ssp)
                {
                  if (debug)
                    fprintf (stderr, "# connection with Server is closed\n");
                  SockWrites (MSG_KILL, ssp);
                  SockClose (ssp);
                  ssp = 0;
                }
              goto retry;
            }
          else if (ssp)
            {
              ret = SockWrites (buf, ssp);
              if (ret != 0)
                {
                  SockClose (ssp);
                  ssp = 0;
                  goto retry;
                }
            }
        }
      if (ssp && (FD_ISSET (ssp->sd, &fds)))
        {
          if (debug)
            fprintf (stderr, "# reading from server\n");
          ret = (int) SockGets (buf, MSG_MAX, ssp);
          if (ret == 0)
            {
              SockClose (ssp);
              ssp = 0;
              fprintf (stdout, "%03d %s", strlen (MSG_KILL), MSG_KILL);
              fflush (stdout);
              goto retry;
            }
          if (debug)
            fprintf (stderr, "#< %s", buf);
          fflush (stderr);
          fprintf (stdout, "%3d ", strlen (buf));
          ret = (int) fputs (buf, stdout);
          fflush (stdout);
          if (ret != 0)
            term ();
        }
    }
}

int
xread (int fd, char *buf, int len)
{
  int ret = 0, r;

retry:
  r = read (fd, buf, len);

  if (r < 0)
    return r;
  ret += r;
  if (r == 0 || r >= len)
    return ret;

  len -= r;
  buf += r;
  goto retry;
}
