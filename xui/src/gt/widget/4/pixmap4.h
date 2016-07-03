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
 * pixmapcont.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Mon Apr 17 12:28:32 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#ifndef _pixmapcontainer4_h
#define _pixmapcontainer4_h

#include "gt/widget/frame.h"
#include "gt/widget/pixmapco.h"

typedef struct widget_pmcontainer4
  {
    struct widget_pmcontainer super;
    char direction;
    char drawdirection;
  }
 *pmcontainer4_t;
extern struct item_type item_type_pmcontainer4;

#define pmcontainer4_direction(w)    ((struct widget_pmcontainer4 *)w)->direction
#define pmcontainer4_drawdirection(w)((struct widget_pmcontainer4 *)w)->drawdirection

#endif
