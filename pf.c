/*
 * NETMAJ - network mahjongg -
 * Copyright (C) 1994, 1995 Koji Suzuki (suz@d2.bs1.fc.nec.co.jp)
 *
 * NETMAJ is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * NETMAJ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * 5/7/2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"

#define PF_PREIDENT	"#!/bin/sh\n"
#define PF_IDENT	"# netmaj personal file\n"
#define PF_RESULT	"result: %d %4d %3d %2d %2d %2d %7d %2d %7d %2d %7d %2d\n"
#define PF_PARAM	"param: %s = %s\n"
#define PF_YAKU		"yaku: "

typedef struct pf
{
  struct pf *next;
  int value;
  char str[60];
  char name[1];
} pf_t;

typedef struct
{
  char _personal_file[256];
  int _kyoku_cnt;
  int _reach_cnt;
  int _naki_cnt;
  int _tumo_cnt;
  int _tumo_point;
  int _ron_cnt;
  int _ron_point;
  int _furi_cnt;
  int _furi_point;
  int _yakus[Y_MAX];
  pf_t *_pf_top;
} pf_global_t;

#define personal_file	(((pf_global_t *)(gp->ext[0]))->_personal_file)
#define kyoku_cnt	(((pf_global_t *)(gp->ext[0]))->_kyoku_cnt)
#define reach_cnt	(((pf_global_t *)(gp->ext[0]))->_reach_cnt)
#define naki_cnt	(((pf_global_t *)(gp->ext[0]))->_naki_cnt)
#define tumo_cnt	(((pf_global_t *)(gp->ext[0]))->_tumo_cnt)
#define tumo_point	(((pf_global_t *)(gp->ext[0]))->_tumo_point)
#define ron_cnt		(((pf_global_t *)(gp->ext[0]))->_ron_cnt)
#define ron_point	(((pf_global_t *)(gp->ext[0]))->_ron_point)
#define furi_cnt	(((pf_global_t *)(gp->ext[0]))->_furi_cnt)
#define furi_point	(((pf_global_t *)(gp->ext[0]))->_furi_point)
#define yakus		(((pf_global_t *)(gp->ext[0]))->_yakus)
#define pf_top		(((pf_global_t *)(gp->ext[0]))->_pf_top)

void
pf_init (global_t *gp, char *file, char *prog)
{
  FILE *pf;                     /* personal file */
  char buf[100];
  char buf2[100];
  char buf3[100];
  char *p;
  int v;

  gp->ext[0] = (pf_global_t *) malloc (sizeof (pf_global_t));
  bzero (gp->ext[0], sizeof (pf_global_t));

  pf = 0;
  if (p = getenv ("NETMAJ_LIB"))
    {
      strcpy (personal_file, p);
      strcat (personal_file, "/");
      strcat (personal_file, file);
      strcat (personal_file, ".pf");
      pf = fopen (personal_file, "r");
    }
  if (!pf)
    {
      strcpy (personal_file, NETMAJ_LIB);
      strcat (personal_file, "/");
      strcat (personal_file, file);
      strcat (personal_file, ".pf");
      pf = fopen (personal_file, "r");
    }
  if (!pf)
    {
      strcpy (personal_file, prog);
      if (p = rindex (personal_file, '/'))
        {
          p[1] = 0;
        }
      else
        {
          personal_file[0] = 0;
        }
      strcat (personal_file, file);
      strcat (personal_file, ".pf");
      pf = fopen (personal_file, "r");
    }
  if (!pf)
    {
      personal_file[0] = 0;
    }
  else
    {
      for (;;)
        {
          if (fgets (buf, 100, pf))
            {
              if (!strcmp (buf, PF_PREIDENT))
                continue;
              if (!strcmp (buf, PF_IDENT))
                break;
            }
          fclose (pf);
          personal_file[0] = 0;
          return;
        }

      while (fgets (buf, 100, pf))
        {
          if (buf[0] < 'a' || buf[0] > 'z')
            continue;
          if (sscanf (buf, PF_PARAM, buf2, buf3) == 2)
            {
              v = 0;
              sscanf (buf3, "%d", &v);
              pf_add_param (gp, buf2, v, buf3);
            }
        }
      fclose (pf);
    }
}

void
pf_term (global_t *gp)
{
}

void
pf_gend (global_t *gp)
{
  FILE *pf;
  int i, x;

  pf = 0;
  if (gp->ext[0] && personal_file[0])
    {
      pf = fopen (personal_file, "a");
    }
  if (pf && (kyoku_cnt > 0))
    {
      for (x = i = 0; i < 4; i++)
        {
          if (player[i].rpoint >= player[self].rpoint)
            x++;
        }
      fprintf (pf, PF_RESULT, x, player[self].rpoint, player[self].tpoint,
               kyoku_cnt, reach_cnt, naki_cnt, tumo_point, tumo_cnt,
               ron_point, ron_cnt, furi_point, furi_cnt);
      fprintf (pf, PF_YAKU);
      print_yakus (gp, pf);
      fprintf (pf, "\n");
      fclose (pf);
    }
  pf_clear (gp);
}

void
pf_pend (global_t *gp)
{
  int i;

  kyoku_cnt++;
  if (hand[vself].reach)
    reach_cnt++;
  if (hand[vself].naki)
    naki_cnt++;
  for (i = 0; i < 5; i++)
    {
      if (result[i].flag == RESULT_AGARI_TUMO)
        {
          if (result[i].points[vself] > 0)
            {
              tumo_cnt++;
              tumo_point += result[i].points[vself];
              add_yakus (gp, i);
            }
        }
      else if (result[i].flag == RESULT_AGARI_RON)
        {
          if (result[i].points[vself] > 0)
            {
              ron_cnt++;
              ron_point += result[i].points[vself];
              add_yakus (gp, i);
            }
          if (result[i].points[vself] < 0)
            {
              furi_cnt++;
              furi_point -= result[i].points[vself];
            }
        }
      else if (result[i].flag)
        {
          add_yakus (gp, i);
        }
    }
}

void
add_yakus (global_t *gp, int i)
{
  int j;
  int iy[4];

  if (result[i].flag)
    {
      result_cvt_to_int (result + i, iy);
      for (j = 0; j < Y_MAX; j++)
        {
          if (Y_GET (iy, j))
            yakus[j]++;
        }
    }
}

void
print_yakus (global_t *gp, FILE *fp)
{
  int i;
  for (i = 0; i < Y_MAX; i++)
    {
      if (yakus[i] > 9)
        yakus[i] = 9;
      fprintf (fp, "%d", yakus[i]);
    }
}

void
pf_add_param (global_t *gp, char *name, int value, char *str)
{
  pf_t *p;
  p = (pf_t *) malloc (sizeof (pf_t) + strlen (name));
  strcpy (p->name, name);
  strcpy (p->str, str);
  p->value = value;
  p->next = pf_top;
  pf_top = p;
}

int
pf_param (global_t *gp, char *name, int *vp)
{
  pf_t *p;

  if (!gp->ext[0])
    return -1;

  p = pf_top;
  while (p)
    {
      if (!strcmp (name, p->name))
        {
          *vp = p->value;
          return 0;
        }
      p = p->next;
    }
  return -1;
}

int
pf_param_str (global_t *gp, char *name, char *buf)
{
  pf_t *p;

  if (!gp->ext[0])
    return -1;

  p = pf_top;
  while (p)
    {
      if (!strcmp (name, p->name))
        {
          strcpy (buf, p->str);
          return 0;
        }
      p = p->next;
    }
  return -1;
}

int
pf_param_strn (global_t *gp, char *name, char *buf, int n)
{
  pf_t *p;

  if (!gp->ext[0])
    return -1;

  p = pf_top;
  while (p)
    {
      if (!strcmp (name, p->name))
        {
          strncpy (buf, p->str, n);
          return 0;
        }
      p = p->next;
    }
  return -1;
}

void
pf_clear (global_t *gp)
{
  int i;
  kyoku_cnt = reach_cnt = naki_cnt = tumo_cnt = tumo_point = 0;
  ron_cnt = ron_point = furi_cnt = furi_point = 0;
  for (i = 0; i < Y_MAX; i++)
    yakus[i] = 0;
}
