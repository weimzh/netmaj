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
 * intint.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Fri Apr 28 16:52:31 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "xnet.h"
#include "xnetmaj.h"

#include "rc.c"

void
save_xrcfile ()
{
  FILE *fp;
  char *p;
  char buf[512];

  p = getenv ("HOME");
  if (p)
    {
      sprintf (buf, "%s/.xnetmaj", p);
      fp = fopen (buf, "w");
      if (fp)
        {
          save_rcfile (fp);
        }
      fclose (fp);
    }
}

void
load_xrcfile ()
{
  char *p;
  char buf[512];

  p = getenv ("NETMAJ_LIB");
  if (p)
    {
      sprintf (buf, "%s/.xnetmaj", p);
      load_rcfile (buf);
    }
  p = getenv ("HOME");
  if (p)
    {
      sprintf (buf, "%s/.xnetmaj", p);
      load_rcfile (buf);
    }
}

/* end of file */
