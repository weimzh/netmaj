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
 * getline.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:51:59 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jan 25 22:52:03 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef int integer_t;

#define MAXFIELD   64
#define BUFFERSIZE 4096

static uint NF;
static char *F[MAXFIELD];
static char *separator = ",: \t\n";

int
integerF (integer_t i)
{
  return atoi (F[i]);
}

double
floatF (integer_t i)
{
  return atof (F[i]);
}

char *
stringF (integer_t i)
{
  return F[i];
}

integer_t
numberofF ()
{
  return NF;
}

static int
split (char *p, char *sep)
{
  char *q;
  unsigned int min = strlen (p);

  while (*sep)
    {
      q = strchr (p, *sep);
      if (q != 0)
        {
          if (q - p < min)
            {
              min = q - p;
            }
        }
      sep++;
    }
  return min;
}

static char *
skip_separator (char *src, char *sep)
{
  if (!*src)
    return src;
  while (*src == ' ' || *src == '\t')
    src++;
  if (strchr (sep, *src))
    {
      src++;
    }
  while (*src == ' ' || *src == '\t')
    {
      src++;
    }
  return src;
}

integer_t
div_into_fields (char *s, integer_t buflen)
{
  uint index = 0, count, length;
  char *dest, *src, *buffer;

  buffer = s;
  dest = buffer;
  length = strlen (buffer);
  F[index++] = dest;
  dest = buffer + length + 1;

  src = buffer;
  src = skip_separator (src, separator);

  for (;;)
    {
      if (*src == 0)
        {
          break;
        }
      count = split (src, separator);
      if ((dest - buffer) + count + 1 >= buflen)
        {
          return 1;
        }
      memcpy (dest, src, count);
      if (index >= MAXFIELD)
        {
          return 1;
        }
      F[index++] = dest;
      *(dest + count) = 0;
      dest += (count + 1);
      src += (count);
      src = skip_separator (src, separator);
    }
  NF = index - 1;
  return 1;
}

char **
pointerF ()
{
  return F;
}

static char readbuffer[BUFFERSIZE * 2];

integer_t
getlinefromfile (FILE *fp)
{
  if (!fp)
    fp = stdin;

  if (0 == (fgets (readbuffer, BUFFERSIZE, fp)))
    return 0;

  readbuffer[strlen (readbuffer) - 1] = 0;
  return div_into_fields (readbuffer, BUFFERSIZE);
}

