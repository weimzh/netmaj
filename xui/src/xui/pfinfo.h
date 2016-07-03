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
 * This file is part of xui
 * 
 * pfinfo.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:50:34 1996
 * description:
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

struct playerinfo
{
  char name[40];
  char rname[40];
  int game;
  int juni[5];
  int pnt;
  int tn;
  int pn;
  int rn;
  int nn;
  int tumo_pnt;
  int tumo_cnt;
  int ron_pnt;
  int ron_cnt;
  int furi_pnt;
  int furi_cnt;
  int yakus[60];
  int used;
  struct playerinfo *next;
  int max_tumo_pnt;
  int max_ron_pnt;
  int max_furi_pnt;
};
