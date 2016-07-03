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
 * pixmisc.c -- 
 *
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:14:57 1995
 *   read misc pixmaps 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "pixread.h"

#include "bitmap/misc/hometon.xpm"
#include "bitmap/misc/homenan.xpm"
#include "bitmap/misc/homesya.xpm"
#include "bitmap/misc/homepei.xpm"
#include "bitmap/misc/chipred.xpm"
#include "bitmap/misc/chipblue.xpm"

void
read_misc_pixmaps ()
{
  read_pix (hometon, pm_home_names[0]);
  read_pix (homenan, pm_home_names[1]);
  read_pix (homesya, pm_home_names[2]);
  read_pix (homepei, pm_home_names[3]);
  read_pix (chipred, pm_chipred);
  read_pix (chipred5, pm_chipred5);
  read_pix (chipblue, pm_chipblue);
  read_pix (chipblue5, pm_chipblue5);
}

#include "bitmap/misc/statis.xpm"

void
read_pinfo_pixmap ()
{
  read_pix (statistics, pm_pinfo);
}
