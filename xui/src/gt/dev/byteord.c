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
 * This file is part of j2c
 * 
 * byteord.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Wed Jan 17 18:45:27 1996
 * 
 * Copyright (C) 1995 Todo Software All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  int i = 0x04030201;
  char *p = (char *) &i;
  printf ("#define ENDIAN %c%c%c%c\n", 
          "01234"[p[0]], "01234"[p[1]],"01234"[p[2]],"01234"[p[3]]);
  return 0;
}

