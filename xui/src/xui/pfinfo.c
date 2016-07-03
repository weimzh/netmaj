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
 * pfinfo.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:50:27 1996
 * description:
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

/* player_info */
#include "xnet.h"
#include "xnetmaj.h"
#include "pfinfo.h"
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

struct playerinfo playerinfo;

static int
xbasename (src, dest, suffix)
     char *src, *dest, *suffix;
{
  int len = strlen (src);
  int suflen = strlen (suffix);
  int dlen;
  char *p = src;
  if (len == 0)
    {
      *dest = 0;
      return 0;
    }
  p += (len - 1);
  while (*p && p != src && *p != '/' && *p != '\\')
    {
      p--;
    }
  if (*p == '/' || *p == '\\')
    {
      p++;
    }
  strcpy (dest, p);
  dlen = strlen (dest);
  if (dlen < suflen)
    {
      return 0;
    }

  if (tolower (*(dest + dlen - 1)) == 'f'
      && tolower (*(dest + dlen - 2)) == 'p'
      && *(dest + dlen - 3) == '.')
    {
      *(dest + dlen - suflen) = 0;
    }
  for (p = dest; *p; p++)
    {
      *p = tolower (*p);
    }
  return 0;
}

static int
read_player_info_file (fullpfname, name)
     char *fullpfname;
     char *name;
{
  int i;
  FILE *fp;

  fp = fopen (fullpfname, "r");
  if (!fp)
    {
      return 0;
    }
/* read info */

  memset (&playerinfo, 0, sizeof (playerinfo));
  xbasename (name, playerinfo.name, ".pf");

  while (getlinefromfile (fp))
    {
      if (sscanf((char *) stringF (0), "param: name = %s",playerinfo.rname)==1)
      {}
      else if (!strncmp ((char *) stringF (0), "result: ", 8))
	{
	  int tumo_pnt, ron_pnt, furi_pnt;
	  playerinfo.game++;
	  playerinfo.juni[integerF (2)]++;
	  playerinfo.pnt += integerF (3);
	  playerinfo.tn += integerF (4);
	  playerinfo.pn += integerF (5);
	  playerinfo.rn += integerF (6);
	  playerinfo.nn += integerF (7);
	  playerinfo.tumo_pnt += (tumo_pnt = integerF (8));
	  playerinfo.tumo_cnt += integerF (9);
	  playerinfo.ron_pnt += (ron_pnt = integerF (10));
	  playerinfo.ron_cnt += integerF (11);
	  playerinfo.furi_pnt += (furi_pnt = integerF (12));
	  playerinfo.furi_cnt += integerF (13);

	  if (playerinfo.max_ron_pnt < ron_pnt)
	    playerinfo.max_ron_pnt = ron_pnt;
	  if (playerinfo.max_tumo_pnt < tumo_pnt)
	    playerinfo.max_tumo_pnt = tumo_pnt;
	  if (playerinfo.max_furi_pnt < furi_pnt)
	    playerinfo.max_furi_pnt = furi_pnt;

	}
      else if (!strncmp ((char *) stringF (0), "yaku: ", 6))
	{
	  for (i = 0; i < 60; i++)
	    {
	      char *p = (char *) stringF (2);
	      playerinfo.yakus[i] += p[i] - '0';
	    }
	}
    }
  fclose (fp);
  return 1;
}

static void
_read_player_info_last (fullpfname, name, count)
     char *fullpfname;
     char *name;
     integer_t count;
{
  FILE *fp;
  integer_t *array;
  integer_t i = 0;
  array = (integer_t *) malloc (sizeof (integer_t) * count);

  memset (array, 0, sizeof (integer_t) * count);

  fp = fopen (fullpfname, "r");
  if (!fp)
    {
      return;
    }
/* read info */

  memset (&playerinfo, 0, sizeof (playerinfo));
  xbasename (name, playerinfo.name, ".pf");

  while (getlinefromfile (fp))
    {
      if (!strncmp ((char *) stringF (0), "result: ", 8))
	{
	  playerinfo.game++;
	  array[i++ % count] = integerF (3);
#if 0
	  playerinfo.juni[integerF (2)]++;
	  playerinfo.pnt += integerF (3);
	  playerinfo.tn += integerF (4);
	  playerinfo.pn += integerF (5);
	  playerinfo.rn += integerF (6);
	  playerinfo.nn += integerF (7);
	  playerinfo.tumo_pnt += integerF (8);
	  playerinfo.tumo_cnt += integerF (9);
	  playerinfo.ron_pnt += integerF (10);
	  playerinfo.ron_cnt += integerF (11);
	  playerinfo.furi_pnt += integerF (12);
	  playerinfo.furi_cnt += integerF (13);
#endif
	}
#if 0
      if (!strncmp ((char *) stringF (0), "yaku: ", 6))
	{
	  for (i = 0; i < 60; i++)
	    {
	      char *p = (char *) stringF (2);
	      playerinfo.yakus[i] += p[i] - '0';
	    }
	}
#endif
    }				/* while */
  {
    integer_t total = 0;
    integer_t j;
    for (j = 0; j < count; j++)
      {
	total += array[j];
      }
    playerinfo.pnt = total;
  }

  free (array);
  fclose (fp);
}


void
read_player_info_last (name, count)
     char *name;
     integer_t count;
{
  char *fullpfname;
  fullpfname = (char *) malloc (strlen (netmajlibdir) + strlen (name) + 6);
  sprintf (fullpfname, "%s%c%s.pf", netmajlibdir, dirsep, name);
  _read_player_info_last (fullpfname, name, count);
  free (fullpfname);
}


void
tolowers (char *s)
{
  while (*s)
    {
      *s = tolower (*s), s++;
    }
  *s = 0;
}

int
read_player_info (name)
     char *name;
{
  int n_pf, i;
  char **array;
  char *pfname;
  char *fullpfname;

  read_dentry (NETMAJ_LIB);
  n_pf = n_pf_file ();
  array = (char **) malloc (sizeof (char *) * n_pf);
  memset (array, 0, sizeof (char *) * n_pf);
  get_pf_file (array);

  pfname = (char *) malloc (strlen (name) + 4);
  sprintf (pfname, "%s.pf", name);
  fullpfname = (char *) malloc (strlen (NETMAJ_LIB) + strlen (name) + 6);
  sprintf (fullpfname, "%s%c%s.pf", NETMAJ_LIB, dirsep, name);

  for (i = 0; i < n_pf; i++)
    {
      tolowers (array[i]);
      if (strcmp (array[i], pfname))
	{
	  continue;
	}
      if (0 == read_player_info_file (fullpfname, name))
	{
	  goto error;
	}
      close_dentry ();

      free_pf_file (n_pf, array);
      free (array);
      free (pfname);
      free (fullpfname);

      return 1;
    }
error:
  close_dentry ();
  memset (&playerinfo, 0, sizeof (playerinfo));

  free_pf_file (n_pf, array);
  free (array);
  free (pfname);
  free (fullpfname);

  return 0;
}

static char **pfnamearray;
static int n_pfnamearray;
static int n_pfidx;

int
read_player_first_info ()
{
  int n_pf, ret;
  char *fullpfname;

  n_pfidx = 0;
  read_dentry (NETMAJ_LIB);
  n_pf = n_pfnamearray = n_pf_file ();
  pfnamearray = (char **) xmalloc (sizeof (char *) * n_pf);
  memset (pfnamearray, 0, sizeof (char *) * n_pf);
  get_pf_file (pfnamearray);

  if (!n_pf)
    {
      return 0;
    }

  fullpfname = (char *) malloc (strlen (NETMAJ_LIB) + strlen (pfnamearray[0]) + 5);
  sprintf (fullpfname, "%s%c%s", NETMAJ_LIB, dirsep, pfnamearray[0]);
  if (0 == read_player_info_file (fullpfname, pfnamearray[0]))
    {
      ret = -1;
    }
  else
    {
      ret = 1;
    }
  n_pfidx++;
  free (fullpfname);
  close_dentry ();
  return ret;
}

int
read_player_next_info ()
{
  char *fullpfname;
  int ret;
  if (n_pfnamearray != 0 && n_pfidx >= n_pfnamearray)
    {
      free_pf_file (n_pfnamearray, pfnamearray);
      xfree ((void *) pfnamearray);
      return 0;
    }
  fullpfname = (char *) malloc (strlen (NETMAJ_LIB)
		       + strlen (pfnamearray[n_pfidx]) + 5);
  sprintf (fullpfname, "%s%c%s", NETMAJ_LIB, dirsep, pfnamearray[n_pfidx]);
  if (0 == read_player_info_file (fullpfname, pfnamearray[n_pfidx]))
    {
      ret = -1;
    }
  else
    {
      ret = 1;
    }
  n_pfidx++;
  free (fullpfname);
  return ret;
}

