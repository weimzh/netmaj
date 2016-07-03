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
 * 5-7-2006 Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#ifndef _PORT_H_
#define _PORT_H_

#include <pthread.h>

typedef struct port
{
  pthread_mutex_t lock, rlock, wlock;
  int closed;
  struct
  {
    pthread_cond_t wait;
    int mode;
    int waiting;
    char *p;
    int sz;
  } buf[2];
} port_t;

int port_init (port_t *port, int reader_opt, int writer_opt);
int port_read (port_t *port, void *buffer, int size);
int port_write (port_t *port, void *buffer, int size);
void port_close (port_t *port);

/* port modes */
#define PORT_BREAK        1
#define PORT_NOBLOCK      2
#endif
