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
 * nu.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:48:18 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Tue May  9 00:48:19 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "config.h"
#include "os.h"
#include "def.h"
#include "misc.h"
#include "var.h"

#include "exprdef.h"

#include "hashdef.h"

#include "declare.h"

#ifndef NDEBUG
#define assert_i(p) \
 if (!(p)) { nu_error("internal error %s %d", __FILE__, __LINE__);}
#else
#define assert_i(p)
#endif

#if sparc
#include <alloca.h>
#endif

#define xmalloc nu_malloc
#define xcalloc nu_calloc
#define xfree   nu_free
#define xrealloc nu_realloc
