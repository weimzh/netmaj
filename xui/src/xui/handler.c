/*  NETMAJ-XUI -- X User Interface --  Copyright Notice
 *
 *      Copyright (c) 1995,1996 Todo Software All rights reserved.
 *
 *   You can redistribute it and/or modify this software under the terms
 *  of the GNU General Public License version 2 as published by
 *  the Free Software Foundation.
 *
 *   This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the author be held liable for any damages
 *  arising from the use of this software.
 *
 *   Todo Software : NBC02365@niftyserve.or.jp
 */
/*  -*- Mode: C -*- 
 * This file is part of xnetmaj
 * 
 * handler.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:14:33 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:53:03 1996
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include <stdlib.h>

#include "systemio.h"

#define Fd_set fd_set

struct handler
{
  void *object;
  int (*handler) ();
  struct handler *next;
  int time;
  int done;
};
static struct handler *handlers, *freehandlers;

void
save_handler (void *object, int (*saveaction) ())
{
  struct handler *h;
  if (!freehandlers)
    {
      h = (void *) xcalloc (sizeof (struct handler));
      h->next = 0;
    }
  else
    {
      h = freehandlers;
      freehandlers = freehandlers->next;
      h->next = 0;
    }
  h->object = object;
  h->handler = saveaction;
  h->done = false;

  if (handlers)
    {
      if (h == handlers)
        {
          abort ();
        }
      h->next = handlers;
      handlers = h;
    }
  else
    {
      handlers = h;
      h->next = 0;
    }
}

static void
free_handler (struct handler *handler)
{
  if (!handler->done)
    {
      fprintf (stderr, "not executed handler\n");
    }
  if (freehandlers)
    {
      if (freehandlers == handler)
        {
          abort ();
        }
      handler->next = freehandlers->next;
      freehandlers = handler;
    }
  else
    {
      freehandlers = handler;
      handler->next = 0;
    }
}

boolean
invoke_handlers ()
{
  struct handler *handler, *newhandler;
  int flag = 0;

  for (handler = handlers; handler;)
    {
      (*(handler->handler)) (handler->object);
      handler->done = true;
      newhandler = handler->next;
      handlers = newhandler;
      free_handler (handler);
      if (newhandler == handler)
        {
          abort ();
        }
      handler = newhandler;
      flag = 1;
    }
  return flag;
}


/*
   iohandler 
   sync io implementation make use of select system call
 */
struct iohandler
{
  int fd;
  int mode;
  int (*handler) ();
};

#define MAXFD  20
static struct iohandler iohandler[MAXFD];
int io_fdwidth;

static struct fdset
{
  Fd_set fdset;
  int width;
}
rfdset, wfdset, exceptfdset;

void
save_iohandler (int fd, char *mode, int (*handler) ())
{
  struct fdset *p;
  if (*mode == 'r')
    {
      p = &rfdset;
    }
  else if (*mode == 'w')
    {
      p = &wfdset;
    }
  else if (*mode == 'e')
    {
      p = &exceptfdset;
    }
  else
    {
      return;
    }
  FD_SET (fd, &p->fdset);
  io_fdwidth = p->width = (fd > p->width ? fd : p->width);
  iohandler[fd].fd = fd;
  iohandler[fd].mode = *mode;
  iohandler[fd].handler = handler;
}

void
save_X_iohandler (int fd)
{
  struct fdset *p;
  p = &rfdset;
  p->width = (fd > p->width ? fd : p->width);
  io_fdwidth = p->width;
}

void
free_iohandler (int fd)
{
  struct fdset *p;

  iohandler[fd].fd = 0;
  iohandler[fd].handler = 0;
  if (iohandler[fd].mode == 'r')
    {
      p = &rfdset;
      rfdset.width--;
    }
  else if (iohandler[fd].mode == 'w')
    {
      p = &wfdset;
    }
  else if (iohandler[fd].mode == 'e')
    {
      p = &exceptfdset;
    }
  else
    {
      return;
    }
  FD_CLR (fd, &p->fdset);
  iohandler[fd].mode = 0;
}

static void
invoke_iohandler (int fd)
{
  if (!iohandler[fd].handler)
    return;
  (*(iohandler[fd].handler)) (fd);
}

void
invoke_iohandlers (Fd_set *rfd, int count)
{
  int fd = 0;
  int i = 0;
  for (fd = 0; fd < io_fdwidth + 1; fd++)
    {
      if (FD_ISSET (fd, rfd))
        {
          if (!iohandler[fd].handler)
            break;
          (*(iohandler[fd].handler)) (fd);
          i++;
        }
      if (i >= count)
        {
          break;
        }
    }
}

int
get_iohandler_fd_set_width (char *mode)
{
  return rfdset.width;
}

/* return 'fd_set copy'  */
Fd_set *
get_iohandler_fd_set (char *mode)
{
  if (*mode == 'r')
    return &rfdset.fdset;
  if (*mode == 'w')
    return &wfdset.fdset;
  if (*mode == 'e')
    return &exceptfdset.fdset;
  abort ();
}
