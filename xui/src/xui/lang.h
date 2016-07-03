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
 * lang.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:52:28 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:52:33 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

extern char *conf_message(int);

#define STRING_SOUND        0
#define STRING_PAUSE        1
#define STRING_ADVICE       2
#define STRING_ADVICEALWAYS 3
#define STRING_ADVICEDIGIT  4
#define STRING_ADVICESELECT 5
#define STRING_ADVICEPAI    6
#define STRING_BOARDCOLOR   7
#define STRING_ORIGINAL     8
#define STRING_TALK         9
#define STRING_CANCELBUTTON 10
#define STRING_TALKTICK     11
#define STRING_LINEPOS      12
#define STRING_POSMARK      13
#define STRING_STRINGEXEC   14
#define STRING_ANALIZE      15
#define STRING_REDRAWSFX    16
#define STRING_REQUEST      17

#if ENGLISH
#define t1    "param: talk_1 = \n"
#define t2    "param: talk_2 = \n"
#define t3    "param: talk_3 = \n"
#define t4    "param: talk_4 = \n"
#define t5    "param: talk_5 = \n"
#define t6    "param: talk_6 = \n"
#define t7    "param: talk_7 = \n"
#define t8    "param: talk_8 = \n"
#define t9    "param: talk_9 = \n"
#else
#define t1    "param: talk_1 = よろしくおねがいします\n"
#define t2    "param: talk_2 = はやくしてね\n"
#define t3    "param: talk_3 = ちょっとまってね\n"
#define t4    "param: talk_4 = やった！！\n"
#define t5    "param: talk_5 = しまった\n"
#define t6    "param: talk_6 = 困った\n"
#define t7    "param: talk_7 = まいった\n"
#define t8    "param: talk_8 = 地獄に落ちろ!\n"
#define t9    "param: talk_9 = ギャー\n"
#endif
