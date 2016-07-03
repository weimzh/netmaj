/* 
 * Copyright (c) 1996 Koji Suzuki All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY KOJI SUZUKI AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL KOJI SUZUKI OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ss.c --- small simple socket library for netmaj
 * This is RE-WROTE version of 
 *     "Simple Socket Library by Mat Watson and Hubert Bartels"
 */

#include "ss.h"

static int max_sd;
static fd_set fds, readable, zero;

static SOCKETIO *
_sock_alloc (int sd)
{
  SOCKETIO *sp;
  sp = (SOCKETIO *) calloc (sizeof (SOCKETIO), 1);
  if (sp)
    {
      sp->sd = sd;
      sp->rlen = 0;
      sp->rptr = 0;
      if (sd > max_sd)
        max_sd = sd;
    }
  FD_SET (sd, &fds);
  return sp;
}

static void
alrment ()
{
  return;
}

static void
_sock_free (SOCKETIO *sp)
{
  if (sp)
    {
      FD_CLR (sp->sd, &fds);
      FD_CLR (sp->sd, &readable);
      free (sp);
    }
  close (sp->sd);
}

SOCKETIO *
ServerSock (int port)
{
  int sd, r, flag;
  struct sockaddr_in addr;
  SOCKETIO *ssp;

  sd = socket (AF_INET, SOCK_STREAM, 0);
  if (sd < 0)
    {
      return (SOCKETIO *) 0;
    }

  flag = 1;
  r = setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, (char *) &flag, sizeof (flag));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons (port);

  r = bind (sd, (struct sockaddr *) &addr, sizeof (struct sockaddr));

  if (r != 0)
    {
      close (sd);
      return (SOCKETIO *) 0;
    }
  r = listen (sd, 5);
  if (r != 0)
    {
      close (sd);
      return (SOCKETIO *) 0;
    }
  return _sock_alloc (sd);
}

SOCKETIO *
AcceptSock (SOCKETIO *ssp)
{
  int sd;

  sd = accept (ssp->sd, (struct sockaddr *) 0, (unsigned int *) 0);
  if (sd < 0)
    {
      return (SOCKETIO *) 0;
    }
  return _sock_alloc (sd);
}

SOCKETIO *
ConnectSock (char *host, int port)
{
  return TimedConnectSock (host, port, -1.0);
}

SOCKETIO *
TimedConnectSock (char *host, int port, double timeout)
{
  int sd, r;
  struct hostent *hep;
  struct sockaddr_in addr;
  int to = (int) timeout;
  void (*func) ();

  hep = gethostbyname (host);
  if (!hep)
    {
      return (SOCKETIO *) 0;
    }

#ifdef USE_BCOPY
  bcopy ((char *) (hep->h_addr), (char *) &(addr.sin_addr), hep->h_length);
#else
  memcpy ((char *) &(addr.sin_addr), (char *) (hep->h_addr), hep->h_length);
#endif

  addr.sin_family = AF_INET;
  addr.sin_port = htons (port);

  sd = socket (AF_INET, SOCK_STREAM, 0);
  if (sd < 0)
    {
      return (SOCKETIO *) 0;
    }
  if (to > 0)
    {
      func = signal (SIGALRM, alrment);
      alarm (to);
    }
  r = connect (sd, (struct sockaddr *) &addr, sizeof (addr));
  if (to > 0)
    {
      alarm (0);
      signal (SIGALRM, func);
    }

  if (r)
    {
      close (sd);
      return (SOCKETIO *) 0;
    }

  return _sock_alloc (sd);
}

SOCKETIO *
SockFdopen (int fd)
{
  return _sock_alloc (fd);
}

void
SockClose (SOCKETIO *sp)
{
  _sock_free (sp);
}

char *
SockGets (char *buf, int len, SOCKETIO *sp)
{
  int c, r;
  char *top = buf;

  while (len > 0)
    {
      if (sp->rptr < sp->rlen)
        {
          *buf++ = c = sp->rbuf[sp->rptr++];
          len--;
          if (c == '\n')
            {
              *buf = 0;
              if (sp->rptr < sp->rlen)
                FD_SET (sp->sd, &readable);
              else
                FD_CLR (sp->sd, &readable);
              return top;
            }
        }
      else
        {
          sp->rlen = sp->rptr = 0;
          r = read (sp->sd, sp->rbuf, SSS_BUFSIZE);
          if (r <= 0)
            {
              return (char *) 0;
            }
          sp->rlen = r;
        }
    }
  *buf = 0;
  if (sp->rptr < sp->rlen)
    FD_SET (sp->sd, &readable);
  else
    FD_CLR (sp->sd, &readable);
  return top;
}

int
SockWrites (char *buf, SOCKETIO *sp)
{
  int len, r;
  len = strlen (buf);
  r = write (sp->sd, buf, len);
  return (r >= 0) ? 0 : -1;
}

int
SockIsRead (SOCKETIO *sp)
{
  int r;
  struct timeval tv;
  fd_set readfds;

  if (sp->rlen != sp->rptr)
    return 1;

  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO (&readfds);
  FD_SET (sp->sd, &readfds);
  r = select (sp->sd + 1, &readfds, (fd_set *) 0, (fd_set *) 0, &tv);
  if (r < 0)
    {
      perror ("SockIsRead");
      return 0;
    }
  return r;
}

int
SockSelect (double timeout, char *flags)
{
  int r;
  fd_set readfds;
  struct timeval tv, *tvp;

#ifdef USE_BCOPY
  if (bcmp (&readable, &zero, sizeof (zero)))
    return 1;
#else
  if (memcmp (&readable, &zero, sizeof (zero)))
    return 1;
#endif
  if (timeout < 0.0)
    tvp = 0;
  else
    {
      tv.tv_sec = timeout;
      tv.tv_usec = (timeout - tv.tv_sec) * 1e+6;
      tvp = &tv;
    }
  FD_COPY (&fds, &readfds);
  r = select (max_sd + 1, &readfds, (fd_set *) 0, (fd_set *) 0, tvp);
  return r;
}

void
SockSetFd (int fd)
{
  if (fd > max_sd)
    max_sd = fd;
  FD_SET (fd, &fds);
}

void
SockClrFd (int fd)
{
  FD_CLR (fd, &fds);
}
