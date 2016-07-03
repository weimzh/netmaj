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
 *  See the gpl.text for more details.
 *
 *   If you would like to do something with NETMAJ that this copyright
 *  prohibits (such as distributing it with a commercial product,
 *  using portions of the source in some other program, etc.), please
 *  contact the author (preferably via email).  Arrangements can
 *  probably be worked out.
 *
 *   Todo Software : NBC02365@niftyserve.or.jp
 */
/*  -*- Mode: C -*- 
 * This file is part of my project
 * 
 * util.h -- 
 * 
 * $Id: util.h,v 1.2 1996/07/31 22:49:11 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Mon Apr 17 12:28:10 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:40:15 1996
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */



#if defined(__STDC__) || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif

/* error.c */
void fatal P_((char * s));
void error P_((char * s));
/* xmalloc.c */
char *xmalloc P_((register unsigned int size));
char *xcalloc P_((register unsigned int size));
char *xrealloc P_((register char *p, register unsigned int size));
void xfree P_((char *p));
