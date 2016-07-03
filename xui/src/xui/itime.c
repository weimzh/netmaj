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
 * This file is part of my project
 * 
 * itime.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Wed Jun  7 04:51:46 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include <signal.h>

static int (*handler) ();
static int timer_interval;

static int
timer_handler_invoke ()
{
  if (handler)
    (*handler) ();

#if SYSV || defined(linux)
  signal (SIGALRM, (void *) timer_handler_invoke);
#endif
  alarm (timer_interval);
}

#include <sys/time.h>

#define BACKGROUND 1

#if BACKGROUND
void
set_timer_handler (int (*func) (), int interval)
{
  handler = func;
  signal (SIGALRM, (void *) timer_handler_invoke);
  alarm (timer_interval = interval);
}

void
clear_timer_handler ()
{
  alarm (0);
  signal (SIGALRM, SIG_IGN);
}

void
stop_timer ()
{
  alarm (0);
}

void
resume_timer ()
{
  alarm (timer_interval);
}

#else

void
set_timer_handler (int (*func) (), interval)
{
}

void
clear_timer_handler ()
{
}

void
stop_timer ()
{
}

void
resume_timer ()
{
}

#endif
