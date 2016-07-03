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
 * attr.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:13:59 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:53:19 1996
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"

/*
   #define T_CHARS              " !4T4..."
   #define F_CHARS              "  nnn..."
   #define R_CHARS              " tpkR..."
 */

static gpixmap2_t PIXMAP_T_CHARS[8];
static gpixmap2_t PIXMAP_F_CHARS[8];
static gpixmap2_t PIXMAP_R_CHARS[8];

void
attr_init ()
{
  PIXMAP_T_CHARS[0] = &pm_attrnon;
  PIXMAP_T_CHARS[1] = &pm_attrreach;
  PIXMAP_T_CHARS[2] = &pm_attr4;
  PIXMAP_T_CHARS[3] = &pm_attrT;
  PIXMAP_T_CHARS[4] = &pm_attr4;
  PIXMAP_T_CHARS[5] = &pm_attrdot;
  PIXMAP_T_CHARS[6] = &pm_attrdot;
  PIXMAP_T_CHARS[7] = &pm_attrdot;

  PIXMAP_F_CHARS[0] = &pm_attrnon;
  PIXMAP_F_CHARS[1] = &pm_attrnon;
  PIXMAP_F_CHARS[2] = &pm_attrn;
  PIXMAP_F_CHARS[3] = &pm_attrn;
  PIXMAP_F_CHARS[4] = &pm_attrn;
  PIXMAP_F_CHARS[5] = &pm_attrdot;
  PIXMAP_F_CHARS[6] = &pm_attrdot;
  PIXMAP_F_CHARS[7] = &pm_attrdot;

  PIXMAP_R_CHARS[0] = &pm_attrnon;
  PIXMAP_R_CHARS[1] = &pm_attrtie;
  PIXMAP_R_CHARS[2] = &pm_attrpon;
  PIXMAP_R_CHARS[3] = &pm_attrkan;
  PIXMAP_R_CHARS[4] = &pm_attrron;
  PIXMAP_R_CHARS[5] = &pm_attrdot;
  PIXMAP_R_CHARS[6] = &pm_attrdot;
  PIXMAP_R_CHARS[7] = &pm_attrdot;

}

gpixmap2_t
attr2pix (attr_t n, integer_t kind)
{
  if (kind == 't')
    {
      return PIXMAP_T_CHARS[T_GET (n) >> T_SHFT];
    }
  if (kind == 'f')
    {
      return PIXMAP_F_CHARS[F_GET (n) >> F_SHFT];
    }
  if (kind == 'r')
    {
      return PIXMAP_R_CHARS[R_GET (n) >> R_SHFT];
    }
  return &pm_attrnon;
}
