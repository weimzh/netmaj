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
 * pixread.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:14:47 1995
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#define pixread 1

#include "pixread.h"

void
read_pixmaps ()
{
  read_decorate_pixmaps ();
  read_openning_pixmaps ();
  read_misc_pixmaps ();
  read_advice_pixmaps ();
  read_tiny_pixmaps ();
}
