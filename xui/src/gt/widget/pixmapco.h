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
 * This file is part of my project
 * 
 * pixmapcont.h -- 
 * 
 * $Id: pixmapco.h,v 1.2 1996/07/31 22:47:49 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Mon Apr 17 12:28:32 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Sun Apr  7 01:48:08 1996
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#ifndef _pixmapcontainer_h
#define _pixmapcontainer_h

#include "gt/widget/frame.h"

typedef struct widget_pmcontainer
  {
    _frame_widget_body;
    void *content;
    integer_t margin;
    byte_t packdirect;
    integer_t n_contents;
    integer_t max_contents;
    integer_t pixmapstart;
  }
 *pmcontainer_t;
extern struct item_type item_type_pmcontainer;

#define pmcontainer_content(p)      ((struct widget_pmcontainer *)p)->content
#define pmcontainer_packdirect(p)    ((struct widget_pmcontainer *)p)->packdirect
#define pmcontainer_margin(p)        ((struct widget_pmcontainer *)p)->margin
#define pmcontainer_n_contents(p)    ((struct widget_pmcontainer *)p)->n_contents
#define pmcontainer_max_contents(p)  ((struct widget_pmcontainer *)p)->max_contents
#define pmcontainer_pixmapstart(w)   ((struct widget_pmcontainer *)p)->pixmapstart

#endif
