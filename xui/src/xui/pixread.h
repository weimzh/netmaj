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
 * pixread.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:14:36 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:51:21 1996
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#ifndef _READPIX_H
#define _READPIX_H

/*
   pixread.h
 */

#include "gt/widget/widget.h"
#include "gt/dev/gdev.h"

extern gdev_t main_gdev;

#define read_pix(_name, _xpm)  \
   gdev_create_pixmapfromdata (main_gdev, identstr(_name), \
                                     fbitmap(_name), \
                                     &_xpm.pm, \
                                     &_xpm.Mpm);

#define read_pix_idx(_name, _xpm)  \
   gdev_create_pixmapfromdata (main_gdev, identstr(_name), \
                                     _name, \
                                     &_xpm.pm, \
                                     &_xpm.Mpm);

#define fbitmap(a)   identcat(a,_xpm)

typedef struct gpixmap2
{
  gpixmap_t pm;
  gpixmap_t Mpm;
}
*gpixmap2_t;

#if pixread
#define _extern_
#else
#define _extern_ extern
#endif

#include "pixdec.h"
#include "pixbdig.h"
#include "pixmisc.h"
#include "pixyaku.h"
#include "pmutil.h"

#endif

#define PAIOPEN 1
#define PAIDARK 2
