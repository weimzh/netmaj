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
 * geom.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:14:22 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#define STATE_H   PAI_H
#define STATE_W   PAI_W

#define name_width 98

struct board_param {
	int xclient;
	int bitmapwidth;
	int bitmapwidth2;
	int bitmapheight;
	int xwidth;
	int xheight;
        int actbutton_w;
        int button_h;
        int button_w;
        int topmargin;
        int bottommargin;
	int communication;
};

extern int board_type;
extern struct board_param board_param;

#define BUILD4	  (board_type & 4)
#define PAICARD   ((board_type & 3) == 0)
#define PAICARD3D ((board_type & 3) == 1)

#define PAICARD_V    0
#define PAICARD3D_V  1
#define BUILD4_V     4

#define ATTR_H       2
#define ATTR_W       PAI_W
#define TITLE_H      MBUTTON_H
#define BITMAPWIDTH  board_param.bitmapwidth
#define BITMAPWIDTH2 board_param.bitmapwidth2
#define BITMAPHEIGHT board_param.bitmapheight
#define XWIDTH       board_param.xwidth
#define XHEIGHT      board_param.xheight

#define ACTBUTTON_W  board_param.actbutton_w
#define BUTTON_H     board_param.button_h
#define BUTTON_W     board_param.button_w
#define TOPMARGIN    board_param.topmargin
#define BOTTOMMARGIN board_param.bottommargin

#define XCLIENT		 board_param.xclient

#define COMMUNICATION board_param.communication

#define JPFONTHEIGHT  16

#define PAI_W        BITMAPWIDTH
#define PAI_W2       BITMAPWIDTH2
#define PAI_H        BITMAPHEIGHT

#define MBUTTON_H  22
#define MBUTTON_W  62
#if 0
#define MBUTTON_H  18
#define MBUTTON_W  54
#endif

#define MARK_H    12
#define DIGIT_H   12
#define NAME_H    12

#define YAKU_H    14
#define GEND_H    24
#define BDIGIT_H  24

#define CELL_H  18

#define CELLHOME_H      20
#define RESULTHOME_H    32
#define YAKUHEAD_H      22

#define LEFTMARGIN    ((XWIDTH-400)/2)

#define MSG_H 24

#define HEAD_H 24

#define USERSTATE_H 16
#define HANDPAISTART 5

#if ENGLISH
#undef SMALLJP
#undef MEDIUMJP
#define SMALLJP   TIMESIB
#define MEDIUMJP  LUCIDAB
#endif

#define HANDPAIMARGIN    (PAI_W/4)
#define OPENPAIMARGIN    5

