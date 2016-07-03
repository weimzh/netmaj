/* 
 * Copyright (c) 1996 Koji Suzuki All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ss.h --- small simple socket library for netmaj
 * This is RE-WROTE version of 
 *     "Simple Socket Library by Mat Watson and Hubert Bartels"
 */

#ifndef _SSS_H_
#define _SSS_H_

#include <stdio.h>
#include <signal.h>
#ifndef NO_STDLIB
#include <stdlib.h>
#endif
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>

#ifdef OLD_SELECT
typedef int fd_set;
#define FD_SET(n, p)    (*(p) |= (1 << (n)))
#define FD_CLR(n, p)    (*(p) &= ~(1 << (n)))
#define FD_ISSET(n, p)  (*(p) & (1 << (n)))
#define FD_ZERO(p)      (*(p) = 0)
#define FD_COPY(f, t)   (*(t) = *(f))
#endif
#define SSS_BUFSIZE 1024

#ifndef FD_COPY
#ifdef USE_BCOPY
#define FD_COPY(f, t)   bcopy(f, t, sizeof(*(f)))
#else
#define FD_COPY(f, t)   memcpy(t, f, sizeof(*(f)))
#endif
#endif

typedef struct SOCKETIO
{
  int sd;
  char rbuf[SSS_BUFSIZE];
  int rlen;
  int rptr;
} SOCKETIO;

SOCKETIO *AcceptSock (SOCKETIO *ssp);
SOCKETIO *ConnectSock (char *hostname, int port_number);
SOCKETIO *TimedConnectSock (char *hostname, int port_number, double timeout);
SOCKETIO *ServerSock (int port_number);
SOCKETIO *SockFdopen (int fd);
void SockClose (SOCKETIO *sp);
char *SockGets (char *s, int n, SOCKETIO *sp);
int SockIsRead (SOCKETIO *sp);
int SockSelect (double timeout, char *flag);
int SockWrites (char *s, SOCKETIO *sp);
void SockSetFd (int fd);
void SockClrFd (int fd);

#endif
