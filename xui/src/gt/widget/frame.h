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
 * frame.h -- 
 * 
 * $Id: frame.h,v 1.2 1996/07/31 22:47:35 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:01:18 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Wed Apr 10 23:56:55 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#ifndef _frame_h
#define _frame_h

typedef struct widget_frame
  {
#define _frame_widget_body  _base_widget_body; char align; integer_t start

    _frame_widget_body;
  }
 *frame_t;
extern struct item_type item_type_frame;

#define frame_packed(p) ((struct widget_frame *)p)->packed
#define frame_align(p)  ((struct widget_frame *)p)->align
#define frame_start(p)  ((struct widget_frame *)p)->start

#endif
