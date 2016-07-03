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
 * pixyaku.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:14:51 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Fri Aug 18 05:20:41 1995
 *   yaku pixmaps
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "xnet.h"
#include "pixread.h"

#include "bitmap/misc/rryukyok.xpm"
#include "bitmap/misc/rtumo.xpm"
#include "bitmap/misc/rron.xpm"
#include "bitmap/misc/rchonbo.xpm"
#include "bitmap/misc/pointh.xpm"
#include "bitmap/misc/gameendh.xpm"
#include "bitmap/misc/fusfx.xpm"
#include "bitmap/misc/fansfx.xpm"

void
read_result_name ()
{
  read_pix_idx (rryukyoku_xpm, pm_result_name[1]);
  pm_result_name[2] = pm_result_name[1];
  read_pix_idx (rchonbo_xpm, pm_result_name[3]);
  pm_result_name[4] = pm_result_name[3];
  read_pix_idx (rtumo_xpm, pm_result_name[5]);
  read_pix_idx (rron_xpm, pm_result_name[6]);

  read_pix (pointh, pm_point_header);
  read_pix (fusfx, pm_point_fu_sfx);
  read_pix (fansfx, pm_point_fan_sfx);
  read_pix (gameendh, pm_gameend_header);
}
