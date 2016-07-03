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
 * talkcmd.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:49:42 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:49:46 1996
 * description:
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "xnet.h"

void
comment_divide (char *src, char *dest1, char *dest2)
{
  *dest2 = 0;
  for (; *src;)
    {
      if (*src == '#' && (*(src + 1) == '!' || (*(src + 1) == '#')))
        {
          *dest1 = 0;
          strcpy (dest2, src + 2);
          break;
        }
      *dest1++ = *src++;
    }
  *dest1 = 0;
}

void
comment_exec_command (char *s)
{
  while (isspace (*s))
    s++;
  if (strncmp (s, "exec", 4))
    {
      s += 4;
      while (isspace (*s))
        s++;
      if (*s)
        {
          if (flag_stringexec)
            system (s);
        }
    }
  else if (strncmp (s, "play", 4))
    {
      char file[256], *p = file;

      s += 4;
      while (isspace (*s))
        s++;
      while (!isspace (*s) && *s != 0)
        *p++ = *s++;
      *p = 0;
#ifndef X11
      if (flag_stringexec)
        playsound (file);
#endif
    }
}
