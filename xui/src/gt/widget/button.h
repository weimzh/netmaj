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
 * button.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:01:29 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

struct widget_button
  {
    _base_widget_body;
    char *text;
    integer_t textcolor;
    integer_t textfont;
    gpixmap_t pixmap;
    gpixmap_t pixmap_mask;
  };
extern struct item_type item_type_button;
