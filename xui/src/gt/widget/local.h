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
 * local.h -- 
 * 
 * $Id: local.h,v 1.2 1996/07/31 22:47:39 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Aug 24 18:32:04 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Aug 24 18:32:50 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */


 
 
#define widget_gdev(p)       ((widget_t)(p))->dev  
#define widget_left(p)       ((widget_t)(p))->left 
#define widget_top(p)        ((widget_t)(p))->top 
#define widget_height(p)     ((widget_t)(p))->height 
#define widget_width(p)      ((widget_t)(p))->width 
  
 
#define widget_fixed(p)       ((widget_t)(p))->abs.fixed 
#define widget_abs_x(p)       ((widget_t)(p))->abs.left 
#define widget_abs_y(p)       ((widget_t)(p))->abs.top 
 
  
#define widget_border_width(p)      ((widget_t)(p))->border_width 
#define widget_active_gc(p)         ((widget_t)(p))->active.main.gc 
#define widget_active_sub_gc(p)     ((widget_t)(p))->active.sub.gc 
#define widget_active_light_gc(p)   ((widget_t)(p))->active.light.gc 
#define widget_active_dark_gc(p)    ((widget_t)(p))->active.dark.gc 
#define widget_active_relief(p)     ((widget_t)(p))->active.relief 
#define widget_relief(p)            widget_active_relief(p)
#define widget_inactive_gc(p)       ((widget_t)(p))->inactive.main.gc 
#define widget_inactive_sub_gc(p)   ((widget_t)(p))->inactive.sub.gc 
#define widget_inactive_light_gc(p) ((widget_t)(p))->inactive.light.gc 
#define widget_inactive_dark_gc(p)  ((widget_t)(p))->inactive.dark.gc 
#define widget_inactive_relief(p)   ((widget_t)(p))->inactive.relief 
#define widget_cannot_change_model(p)  ((widget_t)(p))->cannot_change_model

/**/
