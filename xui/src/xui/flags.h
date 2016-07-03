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
 * flags.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:52:36 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#ifndef _FLAGS_H
#define _FLAGS_H

#include "gt/widget/widget.h"

#if XNET
#define EXVAL(x) =x
#define XEXTERN
#else
#define EXVAL(x)
#define XEXTERN extern
#endif

XEXTERN struct view view_struct;
XEXTERN struct view *view EXVAL (&view_struct);

XEXTERN integer_t flag_not_needredraw;
XEXTERN integer_t flag_not_needdraw_hand;
XEXTERN integer_t flag_soundp;
XEXTERN integer_t flag_pause EXVAL (1);
XEXTERN integer_t flag_advice EXVAL (1);
XEXTERN integer_t flag_advicealways;
XEXTERN integer_t flag_advicedigit EXVAL (1);
XEXTERN integer_t flag_advicepai EXVAL (1);
XEXTERN integer_t flag_adviceselect EXVAL (1);
XEXTERN char *flag_username;
XEXTERN integer_t flag_tonpu EXVAL (1);
XEXTERN integer_t flag_log_p EXVAL (1);
XEXTERN char *flag_logheader;
XEXTERN char *flag_host EXVAL ("localhost");
XEXTERN char *flag_port EXVAL ("4112");
XEXTERN char *flag_protocol EXVAL ("");
XEXTERN char *flag_viewer EXVAL ("notepad.exe");
XEXTERN char *flag_basiccolorname;
XEXTERN char *flag_users[3];
XEXTERN integer_t flag_executeid;
XEXTERN integer_t flag_invisible_riverpai;
XEXTERN char *flag_haifudir;
XEXTERN integer_t flag_original EXVAL (0);
XEXTERN integer_t flag_talk EXVAL (1);
XEXTERN integer_t flag_cancelbutton EXVAL (1);
XEXTERN integer_t flag_gamespeed EXVAL (2);
XEXTERN integer_t flag_talktick EXVAL (4);
XEXTERN integer_t flag_linepos EXVAL (0);
XEXTERN integer_t flag_posmark EXVAL (1);
XEXTERN integer_t flag_stringexec;
XEXTERN integer_t flag_analize;
XEXTERN integer_t flag_redrawsfx EXVAL (1);
XEXTERN integer_t flag_request EXVAL (1);
XEXTERN char *flag_rank_sort_func_str EXVAL ("money");

#ifndef NETMAJ_LIB
#define NETMAJ_LIB "/usr/lib/games/netmaj"
#endif

#define netmajlibdir NETMAJ_LIB

#define dirsep '/'

XEXTERN widget_t rankbutton;
XEXTERN widget_t testpaibutton;
XEXTERN widget_t totalbutton;
XEXTERN widget_t bdcolorbutton;
XEXTERN widget_t configbutton;
XEXTERN widget_t configsavebutton;
XEXTERN widget_t configbyebutton;

XEXTERN widget_t pointokbutton;
XEXTERN widget_t gameendokbutton;
XEXTERN widget_t rankokbutton;
XEXTERN widget_t playinfookbutton;
XEXTERN widget_t aboutokbutton;
XEXTERN widget_t quicklogbutton;

XEXTERN widget_t rankmoneybutton;
XEXTERN widget_t rankpointbutton;
XEXTERN widget_t rankfuributton;

#endif /* _FLAGS_H */
