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

#include <signal.h>
#include "global.h"

void
xsleep (int time_out)
{
  ysleep (time_out * 10);
}

#ifndef HAVE_GETTIMEOFDAY
void
ysleep (int time_out)
{
  fd_set fds;
  struct timeval to, *top;
  int ret = 0;

  top = 0;
  FD_ZERO (&fds);
  if (time_out >= 0)
    {
      to.tv_sec = time_out / 100;
      to.tv_usec = (time_out % 100) * 100000;
      top = &to;
    }
  select (1, &fds, (fd_set *) 0, (fd_set *) 0, top);
}
#else
static double
dtime ()
{
  struct timeval now;
  gettimeofday (&now, 0);
  return (now.tv_sec + now.tv_usec * 0.0000001);
}

void
ysleep (int time_out)
{
  fd_set fds;
  struct timeval to, *top;
  double t, now;
  int ret = 0;

  top = 0;
  FD_ZERO (&fds);
  if (time_out < 0)
    return;

  t = dtime () + time_out * 0.01;

  while ((now = dtime ()) < t)
    {
      to.tv_sec = t - now;
      to.tv_usec = (t - now - (int) (t - now)) * 1000000;
      select (1, &fds, (fd_set *) 0, (fd_set *) 0, &to);
    }
}
#endif
