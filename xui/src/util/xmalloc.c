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
 * xmalloc.c -- 
 * 
 * $Id: xmalloc.c,v 1.2 1996/07/31 22:49:12 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Mon Apr 17 12:28:20 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Fri Jan 26 01:39:17 1996
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include <stdio.h>
/* #include <stddef.h> */
#include <stdlib.h>
#include <memory.h>

typedef unsigned int _size_t;

char *
xmalloc (size)
     register _size_t size;
{
  register char *p;

  if (size == 0)
    {
      return NULL;
    }

  p = (char *) malloc ((size_t) size);
  if (p == NULL)
    {
      abort ();
    }
  return p;
}

char *
xcalloc (size)
     register _size_t size;
{
  register char *p;

  if (size == 0)
    {
      size = 4;
    }
  p = (char *) malloc ((size_t) size);
  if (p == NULL)
    {
      abort ();
    }
  memset (p, 0, size);
  return p;
}

char *
xrealloc (p, size)
     register char *p;
     register _size_t size;
{
  register char *destp;

  if (size == 0)
    {
      return p;
    }
  destp = (char *) realloc ((char *) p, (size_t) size);
  if (destp == NULL)
    {
      abort ();
    }
  return destp;
}

void
xfree (p)
     char *p;
{
  free (p);
}
