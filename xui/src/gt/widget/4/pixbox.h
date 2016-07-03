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

#ifndef _pixbox_h
#define _pixbox_h

#include "gt/widget/frame.h"
#include "gt/widget/4/pixmap4.h"

typedef struct widget_pixbox
  {
    struct widget_pmcontainer4 super;
    integer_t row, col;
    integer_t unit_w, unit_h;
  }
 *pixbox_t;
extern struct item_type item_type_pixbox;

#define pixbox_row(w)    ((struct widget_pixbox *)w)->row
#define pixbox_col(w)    ((struct widget_pixbox *)w)->col
#define pixbox_unit_w(w) ((struct widget_pixbox *)w)->unit_w
#define pixbox_unit_h(w) ((struct widget_pixbox *)w)->unit_h

#endif
