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
/*  -*- Mode: C -*- 
 * This file is part of xnetmaj
 * 
 * itemtype.h -- 
 * 
 * $Id: itemtype.h,v 1.2 1996/07/31 22:47:38 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:01:25 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Sun Apr  7 02:19:26 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

/* 
   item type 
 */
typedef struct item_type
  {
    char *name;
    integer_t type;
    integer_t size;
    struct
      {
	widget_t (*create) ();
	void (*destroy) ();
	void (*configure) ();
	void (*display) ();
	void (*action) ();
	void (*invoke) ();
	void (*clear) ();
      }
    proc;
    struct item_type *next;
  }
 *item_type_t;

/* 
   model 
 */
enum relief_type
  {
    relief_none, relief_sunken, relief_raised, relief_flat, relief_frame
  };


#define _offsetof(s, t)   (unsigned int)(&(((s *)0)->t))


typedef struct configure
  {
    integer_t resourceid;
    integer_t datatype;
    integer_t offset;
    void *value;
  }
 *configure_t;

#define widget_type_menu        1
#define widget_type_frame       2
#define widget_type_menuitem    3
#define widget_type_button      4
#define widget_type_toplevel    5
#define widget_type_pixmapframe 6
#define widget_type_text        7
#define widget_type_pmcontainer 8
#define widget_type_canvas      9
#define widget_type_radio       10
#define widget_type_core        100

#define widget_type_pixbox        200
#define widget_type_pmcontainer4  201

#define widget_configure(a, b, c)  _widget_configure(a, b, (void *)(c))
