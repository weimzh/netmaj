/*
 * Copyright (C) 1994, 1995 Koji Suzuki (suz@d2.bs1.fc.nec.co.jp)
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * 5/7/2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */
/*
 * port.c   ---- port manager 
 *
 *    writer    ------> [ PORT ] ------->   reader
 *
 *    methods:
 *		port_init   : initialize port structure
 *		port_close  : close port and wakeup waiting thread
 *		port_write  : write to port but closed then return -1
 *		port_read   : read from port but closed then return -1
 *
 *   modes:
 *		PORT_BREAK   : if read/write any bytes return
 *		PORT_NOBLOCK : no bloking
 *
 *    o burtterfly argorithm
 *
 *    		  writer:port_write  -> blocked
 *  	 	  reader:port_read   -> unblock writer
 *
 *    		  reader:port_read   -> blocked
 *    		  writer:port_write  -> unblock reader
 *
 *    o multi writer/reader surport 
 *
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "port.h"

#define min(a,b)	( (a) > (b) ? (b) : (a) )
#define READ	0
#define WRITE	1

static int
port_io (port_t *port, char *buf, int bufsz, int dir)
{
  int l = 0;
  int rid = !dir;

  pthread_mutex_lock (&port->lock);

  if (port->closed)
    {
      l = -1;
      goto abort;
    }

  if (port->buf[rid].waiting)
    {
      /* 相手が既にブロックされている */
      l = min (bufsz, port->buf[rid].sz);
      if (dir == READ)
        memcpy (buf, port->buf[rid].p, l);
      else
        memcpy (port->buf[rid].p, buf, l);
      port->buf[rid].sz -= l;
      port->buf[rid].p += l;
      if (port->buf[rid].sz == 0 || port->buf[rid].mode & PORT_BREAK)
        {
          /* 相手が読み(書き)きった */
          pthread_cond_signal (&port->buf[rid].wait);
        }
      if ((bufsz > l) && (!(port->buf[dir].mode & PORT_BREAK)))
        {
          /* 自分がまだ読み(書き)きれていない */
          port->buf[dir].p = buf + l;
          port->buf[dir].sz = bufsz - l;
          port->buf[dir].waiting = 1;
          pthread_cond_wait (&port->buf[dir].wait, &port->lock);
          port->buf[dir].waiting = 0;
          l = bufsz - port->buf[dir].sz;
        }
    }
  else if (!(port->buf[dir].mode & PORT_NOBLOCK))
    {
      /* 自分が最初にブロックされるケース */
      port->buf[dir].p = buf;
      port->buf[dir].sz = bufsz;
      port->buf[dir].waiting = 1;
      pthread_cond_wait (&port->buf[dir].wait, &port->lock);
      port->buf[dir].waiting = 0;
      l = bufsz - port->buf[dir].sz;
    }
abort:
  pthread_mutex_unlock (&port->lock);
  return l;
}

int
port_read (port_t *port, void *buf, int size)
{
  int ret;

  pthread_mutex_lock (&port->rlock);    /* 同時に読める人は、ひとりのみ */
  ret = port_io (port, buf, size, READ);
  pthread_mutex_unlock (&port->rlock);
  return ret;
}

int
port_write (port_t *port, void *buf, int size)
{
  int ret;

  pthread_mutex_lock (&port->wlock);    /* 同時に書ける人は、ひとりのみ */
  ret = port_io (port, buf, size, WRITE);
  pthread_mutex_unlock (&port->wlock);
  return ret;
}

void
port_close (port_t *port)
{
  pthread_mutex_lock (&port->lock);
  port->closed = 1;
  if (port->buf[READ].waiting)
    {
      pthread_cond_signal (&port->buf[READ].wait);
    }
  if (port->buf[WRITE].waiting)
    {
      pthread_cond_signal (&port->buf[WRITE].wait);
    }
  pthread_mutex_unlock (&port->lock);
}

int
port_init (port_t *port, int ropt, int wopt)
{
  port->closed = 0;
  port->buf[READ].mode = ropt;
  port->buf[WRITE].mode = wopt;
  port->buf[READ].waiting = 0;
  port->buf[WRITE].waiting = 0;

#if 0
  pthread_mutex_init (&port->lock, pthread_mutexattr_default);
  pthread_mutex_init (&port->rlock, pthread_mutexattr_default);
  pthread_mutex_init (&port->wlock, pthread_mutexattr_default);
  pthread_cond_init (&port->buf[READ].wait, pthread_condattr_default);
  pthread_cond_init (&port->buf[WRITE].wait, pthread_condattr_default);
#else
  pthread_mutex_init (&port->lock, NULL);
  pthread_mutex_init (&port->rlock, NULL);
  pthread_mutex_init (&port->wlock, NULL);
  pthread_cond_init (&port->buf[READ].wait, NULL);
  pthread_cond_init (&port->buf[WRITE].wait, NULL);
#endif

  return 0;
}
