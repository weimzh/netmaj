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
 * deftype.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Sat Aug 26 00:55:24 1995
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#ifndef _DEFTYPE_H
#define _DEFTYPE_H

#ifndef __STDC__
#define signed
#define const
#define volatile
#endif

typedef signed char char_t;
typedef signed short shalf_t;
typedef signed int sword_t;
typedef unsigned char byte_t;
typedef unsigned short half_t;
typedef unsigned int word_t;
typedef unsigned int cardinal_t;
typedef unsigned char boolean;
typedef signed int integer_t;
typedef void   *pointer;

#define true  ((boolean)1)
#define false ((boolean)0)

typedef unsigned int _size_t;
#endif

