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
 *  See the gpl.text for more details.
 *
 *   If you would like to do something with NETMAJ that this copyright
 *  prohibits (such as distributing it with a commercial product,
 *  using portions of the source in some other program, etc.), please
 *  contact the author (preferably via email).  Arrangements can
 *  probably be worked out.
 *
 *   Todo Software : NBC02365@niftyserve.or.jp
 */
#define resource_left 1
#define resource_top  2
#define resource_height 3
#define resource_width 4
#define resource_text 5
#define resource_pixmap 6
#define resource_packed 8
#define resource_pixmap_mask 9
#define resource_border_width       10
#define resource_relief       11
#define resource_fix       12
#define resource_wrap      13

#define resource_active_relief 50
#define resource_inactive_relief 51

#define resource_buttonpress_callback   100
#define resource_motion_callback        101
#define resource_buttonrelease_callback 102
#define resource_keypress_callback      103
#define resource_button3press_callback  104

#define resource_virtical    110
#define resource_horizontal  111
#define resource_contentpacked      113
#define resource_pixmapstart        114
#define resource_max_contents       115
#define resource_transparent        116
#define resource_textcolor  200
#define resource_textfont   201
#define resource_textshadow 202
#define resource_name 203
#define resource_frame_start 204 
#define resource_radiobutton_value 205
#define resource_radiobutton_width 206
#define resource_radiobutton_height 207
#define resource_radiobutton_border_width 208

#define resource_align 300
#define resource_align_center 301
#define resource_align_begin   302
#define resource_align_end  303
#define resource_cannot_change_model 304

#define resource_packdirect       resource_contentpacked
#define resource_margin           resource_pixmapstart

#define resource_row 1005
#define resource_col 1004
#define resource_unit_w 1003
#define resource_unit_h 1002
#define resource_direction 1000
#define resource_drawdirection 1001
#define resource_invisible 1006




enum
  {
    datatype_none, datatype_int, datatype_char,
    datatype_string, datatype_pointer,
    datatype_pixmap, datatype_proc,
    datatype_string2int
  };
