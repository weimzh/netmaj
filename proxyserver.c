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

#define USE_PIPE

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#ifndef SIGCHLD
#define SIGCHLD SIGCLD
#endif
#include <sys/time.h>
#include <sys/fcntl.h>

#ifndef USE_PIPE
#ifdef USE_TERMIOS              /* for POSIX */
#include <termios.h>
struct termios ta, tb;
#endif
#ifdef USE_TERMIO               /* for System V */
#include    <termio.h>
struct termio ta, tb;
#endif
#ifdef USE_SGTTY                /* for former BSD */
#define USE_OLD_TTY
#include    <sys/ioctl.h>
#undef USE_OLD_TTY
struct sgttyb ta, tb;
#endif
#endif

#include "global.h"

char buf[256];
int port = PROXY_PORT;

char *proxy_gets ();
int debug = 0;

char *cmd = "/bin/sh";

void proxy_term ();
void proxy_cleanup ();
int proxy_fd ();
void proxy_start ();

void
term ()
{
  proxy_term ();
  exit (1);
}

int
main (int argc, char *argv[])
{
  SOCKETIO *ssp, *csp;
  fd_set fds;
  int maxfd;
  int c;

  while ((c = getopt (argc, argv, "dc:")) != EOF)
    {
      switch (c)
        {
        case 'd':
          debug = 1;
          break;
        case 'c':
          cmd = optarg;
        case '?':
          printf ("usage: proxyserver [-d] [-c command]\n");
          exit (1);
        }
    }

  signal (SIGTERM, term);
  signal (SIGINT, term);
  signal (SIGCHLD, SIG_IGN);
  proxy_start ();

retry:
  proxy_cleanup ();

  printf ("Waiting connection to netmaj client\n");
  if (!(ssp = ServerSock (port)))
    {
      fprintf (stderr, "cannot open server socket:%d\n", port);
      exit (1);
    }
  csp = AcceptSock (ssp);
  SockClose (ssp);
  printf ("Connection established\n");

  for (;;)
    {
      FD_ZERO (&fds);
      FD_SET (proxy_fd (), &fds);
      FD_SET (csp->sd, &fds);
      maxfd = proxy_fd () > csp->sd ? proxy_fd () : csp->sd;

      if (debug)
        printf ("Wait start\n");
      select (maxfd + 1, &fds, 0, 0, 0);
      if (FD_ISSET (proxy_fd (), &fds))
        {
          if (debug)
            printf ("read from server\n");
          if (!proxy_gets (buf, 256))
            {
              term ();
            }
          if (SockWrites (buf, csp) != 0)
            {
              SockClose (csp);
              csp = 0;
              if (proxy_puts (MSG_KILL) != 0)
                {
                  term ();
                }
              goto retry;
            }
        }
      else if (FD_ISSET (csp->sd, &fds))
        {
          if (debug)
            printf ("read from client\n");
          if (!SockGets (buf, 256, csp))
            {
              SockClose (csp);
              csp = 0;
              if (proxy_puts (MSG_KILL) != 0)
                {
                  term ();
                }
              goto retry;
            }
          if (proxy_puts (buf) != 0)
            {
              term ();
            }
          if (!strcmp (MSG_KILL, buf))
            {
              if (SockWrites (MSG_KILL, csp) != 0)
                {               /* echo back */
                  SockClose (csp);
                  csp = 0;
                  goto retry;
                }
            }
        }
    }
}

static int child_id;
static int inp[2], outp[2];
FILE *fp_out;

void
proxy_term ()
{
  kill (child_id, SIGTERM);
#ifndef USE_PIPE
#ifdef USE_TERMIOS
  tcsetattr (0, TCSANOW, &ta);
#endif
#ifdef USE_TERMIO
  ioctl (0, TCSETA, &ta);
#endif
#ifdef USE_SGTTY
  ioctl (0, TIOCSETP, &ta);
#endif
#endif
}

int
proxy_fd ()
{
  return outp[0];
}

char *
proxy_gets (char *buf, int len)
{
  int ret;
  int l;

  ret = xread (outp[0], buf, 4);
  if (ret != 4)
    {
      printf ("protocol error 1\n");
      return (char *) 0;
    }
  buf[4] = 0;
  if (sscanf (buf, "%03d", &l) != 1 || l < 0 || l > 256)
    {
      printf ("protocol error 2\n");
      return (char *) 0;
    }
  ret = xread (outp[0], buf, l);

  if (ret != l)
    {
      printf ("protocol error 3\n");
      return (char *) 0;
    }
  buf[l] = 0;
  if (debug)
    printf ("< %s", buf);
  return buf;
}

int
proxy_puts (char *buf)
{
  int ret;
  if (debug)
    printf ("> %s", buf);
  fprintf (fp_out, "%03d ", strlen (buf));
  ret = fputs (buf, fp_out);
  fflush (fp_out);
  return ret;
}

void
proxy_cleanup ()
{
  fd_set fds;
  struct timeval tv;

  for (;;)
    {
      tv.tv_sec = 0;
      tv.tv_usec = 0;
      FD_ZERO (&fds);
      FD_SET (proxy_fd (), &fds);
      select (proxy_fd () + 1, &fds, 0, 0, &tv);
      if (!FD_ISSET (proxy_fd (), &fds))
        break;
      if (!proxy_gets (buf, 256))
        {
          term ();
        }
      if (debug)
        printf ("sute %s\n", buf);
    }
}

void
proxy_start ()
{
  char buf[256];
  fd_set fds;
  int maxfd;
  int ret, st;

#ifdef USE_PIPE
  pipe (inp);
  pipe (outp);

  if ((child_id = fork ()) == 0)
    {
      close (0);
      close (1);
      /*close(2); */
      dup2 (inp[0], 0);
      dup2 (outp[1], 1);
      /*dup2(outp[1],2); */
      printf ("connect to proxyclient by telnet,kermit...etc\n");
      fflush (stdout);
      system (cmd);
      printf ("command exit without connection\n");
      fflush (stdout);
      exit (0);
    }
  if (child_id < 0)
    {
      perror ("fork");
      exit (1);
    }
#else
  char ttyname[20];
  int fd;
#ifdef TIOCGWINSZ
  struct winsize win;
#endif

#ifdef USE_TERMIOS
  tcgetattr (0, &ta);
#endif
#ifdef USE_TERMIO
  ioctl (0, TCGETA, &ta);
#endif
#ifdef USE_SGTTY
  ioctl (0, TIOCGETP, &ta);
#endif
  in_fd = out_fd = ptyopen (ttyname);
  if (in_fd < 0)
    {
      perror ("pty");
      exit (1);
    }
  if (debug)
    fprintf (stderr, "new tty is %s\n", ttyname);

#ifdef TIOCGWINSZ
  ioctl (0, TIOCGWINSZ, &win);
#endif

  if ((child_id = fork ()) == 0)
    {
      close (in_fd);
      close (out_fd);
      close (0);
      close (1);
      close (2);
      fd = open (ttyname, O_RDWR, 0);
      dup2 (fd, 0);
      dup2 (fd, 1);
      dup2 (fd, 2);
#ifdef TIOCGWINSZ
      ioctl (fd, TIOCSWINSZ, &win);
#endif
      printf ("connect to proxyclient by telnet,kermit...etc\n");
      fflush (stdout);
      system (cmd);
      printf ("command exit without connection\n");
      fflush (stdout);
      exit (0);
    }
  if (child_id < 0)
    {
      perror ("fork");
      exit (1);
    }

#ifdef USE_TERMIOS
  tcgetattr (0, &tb);
  tb.c_iflag |= ICRNL;
  tb.c_oflag |= OPOST;
  tb.c_lflag &= ~(ECHO | ICANON);
  tb.c_cc[VMIN] = 1;
  tb.c_cc[VTIME] = 0;
  tcsetattr (0, TCSANOW, &tb);
#endif
#ifdef USE_TERMIO
  ioctl (0, TCGETA, &tb);
  tb.c_iflag |= ICRNL;
  tb.c_oflag |= OPOST;
  tb.c_lflag &= ~(ECHO | ICANON);
  tb.c_cc[VMIN] = 1;
  tb.c_cc[VTIME] = 0;
  ioctl (0, TCSETA, &tb);
#endif
#ifdef USE_SGTTY
  ioctl (0, TIOCGETP, &tb);
  tb.sg_flags |= CBREAK | CRMOD;
  tb.sg_flags &= ~(ECHO);
  ioctl (0, TIOCSETP, &tb);
#endif

#endif /* USE_PIPE */

  fp_out = fdopen (inp[1], "w");
  for (;;)
    {
      maxfd = outp[0];
      FD_ZERO (&fds);
      FD_SET (0, &fds);
      FD_SET (outp[0], &fds);
      st = select (maxfd + 1, &fds, (fd_set *) 0, (fd_set *) 0, 0);

      if (st > 0 && FD_ISSET (outp[0], &fds))
        {
          ret = read (outp[0], buf, 256);
          if (ret > 0)
            {
              buf[ret] = 0;
              if (!strcmp (buf, "proxyclient READY\n"))
                {
                  return;
                }
              write (1, buf, ret);
            }
          else
            {
              perror ("");
              kill (child_id, SIGTERM);
              exit (1);
            }
        }
      if (st > 0 && FD_ISSET (0, &fds))
        {
          ret = read (0, buf, 256);
          if (ret > 0)
            {
              write (inp[1], buf, ret);
            }
          else
            {
              perror ("");
              kill (child_id, SIGTERM);
              exit (1);
            }
        }
    }
}

int
xread (int fd, char *buf, int len)
{
  int ret, r;

  ret = 0;
retry:
  r = read (fd, buf, len);

  if (r < 0)
    {
#ifdef EINTR
      if (errno == EINTR)
        goto retry;
#endif
      return r;
    }
  ret += r;
  if (r == 0 || r >= len)
    return ret;

  len -= r;
  buf += r;
  goto retry;
}

int
ptyopen (char *ttyname)
{
  char ptyname[20];
  int i, fd, c1, c2;

  for (i = 0; i < 64; i++)
    {
      c1 = 'p' + (i / 16);
      c2 = (i % 16) >= 10 ? ('a' + (i % 16) - 10) : ('0' + (i % 16));
      sprintf (ptyname, "/dev/pty%c%c", c1, c2);

      fd = open (ptyname, O_RDWR, 0);
      if (fd >= 0)
        {
          sprintf (ttyname, "/dev/tty%c%c", c1, c2);
          return fd;
        }
    }
}
