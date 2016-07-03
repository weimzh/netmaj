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
 * This file is part of xui
 * 
 * pixmapfr.h -- 
 * 
 * $Id: pixmapfr.h,v 1.2 1996/07/31 22:47:52 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:57:05 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:57:11 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#ifndef _pixmapframe_h
#define _pixmapframe_h

typedef struct widget_pixmapframe
  {
#define _pixmapframe_widget_body  _base_widget_body
    _pixmapframe_widget_body;
    void *pixmaps;
    integer_t pixmapstart;
    byte_t pixmappacked;
    integer_t n_pixmaps;
    integer_t max_pixmaps;
  }
 *pixmapframe_t;
extern struct item_type item_type_pixmapframe;

#define pixmapframe_pixmappacked(p) ((struct widget_pixmapframe *)p)->pixmappacked
#define pixmapframe_pixmapstart(p)  ((struct widget_pixmapframe *)p)->pixmapstart

#endif
