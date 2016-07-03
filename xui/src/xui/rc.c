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
 * This file is part of xui
 * 
 * rc.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:37:59 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:41:01 1996
 * description:
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include "xnetmaj.h"
#include "xnet.h"
#include "flags.h"
#include "int/export.h"

void
save_rcfile (FILE *fp)
{
  if (flag_username)
    {
      fprintf (fp, "(define username \"%s\")\n", flag_username);
    }
  fprintf (fp, "(define tonpu %d)\n", flag_tonpu);
  fprintf (fp, "(define log_p %d)\n", flag_log_p);
  if (flag_logheader)
    {
      fprintf (fp, "(define logheader \"%s\")\n", flag_logheader);
    }
  if (flag_host)
    {
      fprintf (fp, "(define host \"%s\")\n", flag_host);
    }
  if (flag_basiccolorname)
    {
      fprintf (fp, "(define basiccolor \"%s\")\n", flag_basiccolorname);
    }
  if (flag_users[0])
    {
      fprintf (fp, "(define user0 \"%s\")\n", flag_users[0]);
    }
  if (flag_users[1])
    {
      fprintf (fp, "(define user1 \"%s\")\n", flag_users[1]);
    }
  if (flag_users[2])
    {
      fprintf (fp, "(define user2 \"%s\")\n", flag_users[2]);
    }
  fprintf (fp, "(define executeid %d)\n", flag_executeid);
  fprintf (fp, "(define speed %d)\n", flag_gamespeed);
  if (flag_haifudir)
    {
      fprintf (fp, "(define haifudir \"%s\")\n", flag_haifudir);
    }
  if (flag_viewer)
    {
      fprintf (fp, "(define viewer \"%s\")\n", flag_viewer);
    }
  fprintf (fp, "(define pause %d)\n", flag_pause);
  fprintf (fp, "(define sound %d)\n", flag_soundp);

  fprintf (fp, "(define advice %d)\n", flag_advice);
  fprintf (fp, "(define advicealways %d)\n", flag_advicealways);
  fprintf (fp, "(define advicedigit %d)\n", flag_advicedigit);
  fprintf (fp, "(define advicepai %d)\n", flag_advicepai);
  fprintf (fp, "(define adviceselect %d)\n", flag_adviceselect);
  fprintf (fp, "(define invisible %d)\n", flag_invisible_riverpai);
  fprintf (fp, "(define original %d)\n", flag_original);
  fprintf (fp, "(define talk %d)\n", flag_talk);
  fprintf (fp, "(define cancelbutton %d)\n", flag_cancelbutton);
  fprintf (fp, "(define talktick %d)\n", flag_talktick);
  fprintf (fp, "(define posline %d)\n", flag_linepos);
  fprintf (fp, "(define posmark %d)\n", flag_posmark);
  fprintf (fp, "(define stringexec %d)\n", flag_stringexec);
  fprintf (fp, "(define analize %d)\n", flag_analize);
  fprintf (fp, "(define redrawsfx %d)\n", flag_redrawsfx);
  fprintf (fp, "(define request %d)\n", flag_request);
  if (flag_rank_sort_func_str)
    {
      fprintf (fp, "(define rank_sort \"%s\")\n", flag_rank_sort_func_str);
    }
}


#define LOADVALUE(var, string) \
	if (name_exist(string)) { var = (integer_t)name_value(string); }

#define LOADPVALUE(var, string) \
	if (name_exist(string)) { var = (void *)name_value(string); }

void
load_rcfile (char *buf)
{
  if (!nu_source (buf))
      return;

  LOADPVALUE (flag_username, "username");
  LOADVALUE (flag_tonpu, "tonpu");
  LOADVALUE (flag_log_p, "log_p");
  LOADPVALUE (flag_logheader, "logheader");
  LOADPVALUE (flag_host, "host");
  LOADPVALUE (flag_basiccolorname, "basiccolor");
  LOADVALUE (flag_executeid, "executeid");
  LOADPVALUE (flag_users[0], "user0");
  LOADPVALUE (flag_users[1], "user1");
  LOADPVALUE (flag_users[2], "user2");
  LOADVALUE (flag_gamespeed, "speed");
  LOADPVALUE (flag_haifudir, "haifudir");
  LOADPVALUE (flag_viewer, "viewer");
  LOADVALUE (flag_pause, "pause");
  LOADVALUE (flag_soundp, "sound");
  LOADVALUE (flag_advice, "advice");
  LOADVALUE (flag_advicealways, "advicealways");
  LOADVALUE (flag_advicedigit, "advicedigit");
  LOADVALUE (flag_advicepai, "advicepai");
  LOADVALUE (flag_adviceselect, "adviceselect");
  LOADVALUE (flag_invisible_riverpai, "invisible");
  LOADVALUE (flag_original, "original");
  LOADVALUE (flag_talk, "talk");
  LOADVALUE (flag_cancelbutton, "cancelbutton");
  LOADVALUE (flag_talktick, "talktick");
  LOADVALUE (flag_linepos, "posline");
  LOADVALUE (flag_posmark, "posmark");
  LOADVALUE (flag_stringexec, "stringexec");
  LOADVALUE (flag_analize, "analize");
  LOADVALUE (flag_redrawsfx, "redrawsfx");
  LOADVALUE (flag_request, "request");
  LOADPVALUE (flag_rank_sort_func_str, "rank_sort");
}
