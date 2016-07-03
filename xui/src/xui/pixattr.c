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
 * pixattr.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Fri Apr 21 21:07:18 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "pixread.h"

#include "bitmap/attr/252/attrreac.xpm"
#include "bitmap/attr/252/attrron.xpm"
#include "bitmap/attr/252/attrkan.xpm"
#include "bitmap/attr/252/attrpon.xpm"
#include "bitmap/attr/252/attrtie.xpm"
#include "bitmap/attr/252/attrtg.xpm"
#include "bitmap/attr/252/attr4.xpm"
#include "bitmap/attr/252/attrdot.xpm"
#include "bitmap/attr/252/attrn.xpm"
#include "bitmap/attr/252/attrnon.xpm"
#include "bitmap/attr/252/attrt.xpm"

void
read_attr_pixmaps ()
{
  read_pix (attrreach, pm_attrreach);
  read_pix (attrtie, pm_attrtie);
  read_pix (attrtg, pm_attrtg);
  read_pix (attrkan, pm_attrkan);
  read_pix (attrpon, pm_attrpon);
  read_pix (attrron, pm_attrron);
  read_pix (attrnon, pm_attrnon);
  read_pix (attrn, pm_attrn);

  read_pix (attr4, pm_attr4);
  read_pix (attrdot, pm_attrdot);
  read_pix (attrT, pm_attrT);
}
