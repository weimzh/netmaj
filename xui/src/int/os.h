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
 * os.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:49:27 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jun  1 18:20:02 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#ifdef MSDOS

#if TURBOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

#include <stdarg.h>

#endif

#if MSC

#endif

#else

/*
 * SUNOS4 is defined in case of sunos4
 */

#if SUNOS4
#define POSIX 1
#endif

/*
 * SOL2 is defined in case of Solaris-2.x;
 */

#if SOL2
#define POSIX 1
#endif

/*
 * FREEBSD is defined in case of FreeBSD-2.0-RELEASE or later;
 */
#if FREEBSD
#define POSIX 1
#endif

#if POSIX
/* POSIX and ANSI C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#endif

#endif


#if MSDOS
#define _atoi  atol
#else
#define _atoi  atol
#endif

#ifndef S_ISFIFO
#define S_ISFIFO(st_mode) ((st_mode) & S_IFIFO)
#define S_ISCHR(st_mode)  ((st_mode) & S_IFCHR)
#define S_ISREG(st_mode)  ((st_mode) & S_IFREG)
#define S_ISDIR(st_mode)  ((st_mode) & S_IFDIR)
#endif

#if MSDOS
#define _sizeof(t)  (sizeof(t))
#else
#define _sizeof(t)  ((unsigned int)sizeof(t))
#endif

#if MSDOS
#define R_MODE "rb"
#define W_MODE "wb"
#else
#define R_MODE "r"
#define W_MODE "w"
#endif

/* end of file */
