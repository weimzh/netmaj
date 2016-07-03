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
 * pfdir.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:50:21 1996
 * description:
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include <stdio.h>
#include "mydir.h"
#include <errno.h>
#include <memory.h>
#include <ctype.h>

typedef struct
{
  struct dirent d;
  char *name;
}
dentry;

#include "flags.h"

static void
direrror (char *name)
{
  fprintf (stderr, "error: %s\n", name);
}

DIR *
xopendir (char *name)
{
  DIR *d;

  errno = 0;
  d = opendir (name);
  if (errno)
    {
      direrror ("opendir");     /* strerror(errno)); */
    }
  return d;
}

static dentry *entry;
static integer_t n_entry;
static integer_t max_entry = 20;

static void
dentry_reset ()
{
  n_entry = 0;
}

static void
dentry_save (struct dirent *p)
{
  if (!entry)
    {
      entry = (dentry *) xmalloc (sizeof (dentry) * max_entry);
    }
  if (max_entry <= n_entry)
    {
      max_entry += 20;
      entry =
        (dentry *) xrealloc ((void *) entry, sizeof (dentry) * max_entry);
    }
  memcpy (&entry[n_entry].d, p, sizeof (struct dirent));
  entry[n_entry].name = strdup (p->d_name);
  n_entry++;
}

integer_t
read_dentry (char *name)
{
  DIR *dir = xopendir (name);
  struct dirent *ep;

  if (!dir)
    {
      direrror ("cannot open directory");
      return 0;
    }

  dentry_reset ();
  for (;;)
    {
      ep = readdir (dir);
      if (!ep)
        break;
      dentry_save (ep);
    }
  closedir (dir);
  return 1;
}

void
close_dentry ()
{
  if (entry)
    {
      int i;
      for (i = 0; i < n_entry; i++)
        {
          xfree (entry[i].name);
        }
      xfree ((void *) entry);
    }
  entry = 0;
  dentry_reset ();
}

static integer_t
pf_p (char *p)
{
  int len = strlen (p);
  return (len >= 3
          && *(p + len - 3) == '.'
          && tolower (*(p + len - 2)) == 'p'
          && tolower (*(p + len - 1)) == 'f');
}

integer_t
n_pf_file ()
{
  int i, j = 0;
  for (i = 0; i < n_entry; i++)
    {
      int len;
      char *p = entry[i].name;
      len = strlen (p);
      if (pf_p (p))
        {
          j++;
        }
    }
  return j;
}

integer_t
get_pf_file (char **array)
{
  int i, j = 0;
  for (i = 0; i < n_entry; i++)
    {
      char *p = entry[i].name;
      if (pf_p (p))
        {
          array[j] = (char *) strdup (p);
          j++;
        }
    }
  return j;
}

void
free_pf_file (unsigned int n, char **array)
{
  int i;
  for (i = 0; i < n; i++)
    {
      xfree (array[i]);
    }
}

#include "lang.h"

void
check_pf_file (char *name)
{
  FILE *fp;
  char buffer[256];

  sprintf (buffer, "%s/%s.pf", netmajlibdir, name);
  fp = fopen (buffer, "r");
  if (fp)
    {
      fclose (fp);
      return;
    }
  fp = fopen (buffer, "w");
  if (!fp)
    {
      return;
    }
  fprintf (fp, "#!/bin/sh\n");
  fprintf (fp, "# netmaj personal file\n");
  fprintf (fp, t1);
  fprintf (fp, t2);
  fprintf (fp, t3);
  fprintf (fp, t4);
  fprintf (fp, t5);
  fprintf (fp, t6);
  fprintf (fp, t7);
  fprintf (fp, t8);
  fprintf (fp, t9);
  fprintf (fp, "param: talk_10 = \n");
  fprintf (fp, "param: talk_11 = \n");
  fprintf (fp, "param: talk_12 = \n");
  fprintf (fp, "param: talk_13 = \n");
  fprintf (fp, "param: talk_14 = \n");
  fprintf (fp, "param: talk_15 = \n");

  fclose (fp);
}
