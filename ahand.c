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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 * 5/7/2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"

extern FILE *df;

ahand_t result_ahand[MAX_RESULT];
int result_cnt;
int result_rest;
int ahand_kinds[35];

void
ah_new (ahand_t *a, hand_t *h, int rm)
{
  int i, j, k, n;
  int head, yao;

  n = h->closed_num;
  a->rest_num = 0;
  for (i = 0; i < n; i++)
    {
      if (i != rm)
        a->rest[a->rest_num++] = P_KIND (h->closed[i]);
    }
  a->part_num = 0;
  for (i = 0; i < 4; i++)
    {
      switch (h->opened_kind[i])
        {
        case H_TIE:
          for (j = 0; j < 3; j++)
            {
              a->part[i][j] = P_KIND (h->opened[i][j]);
            }
          a->part_kind[i] = AH_H3_FIXED;
          a->part_num++;
          break;
        case H_PON:
        case H_KAN_CLOSED:
        case H_KAN_OPENED:
          for (j = 0; j < 3; j++)
            {
              a->part[i][j] = P_KIND (h->opened[i][j]);
            }
          a->part_kind[i] = AH_V3_FIXED;
          a->part_num++;
          break;
        }
    }
  ah_sort (a);

  n = a->rest_num;
  for (j = 0; j < 35; j++)
    ahand_kinds[j] = 0;
  head = yao = 0;
  for (j = 0; j < n; j++)
    {
      ahand_kinds[a->rest[j]]++;
    }
  for (j = 0; j < 35; j++)
    if (IS_YAO (j))
      {
        if (ahand_kinds[j])
          yao++;
        if (ahand_kinds[j] >= 2)
          head = 1;
      }
  result_rest = (14 - head - yao);      /* for kokusi */
  result_cnt = 1;
  result_ahand[0] = *a;
  ah_result_add (a);
}

void
ah_remove (ahand_t *a, int i)
{
  for (i++; i < a->rest_num; i++)
    {
      a->rest[i - 1] = a->rest[i];
    }
  a->rest_num--;
}

int
ah_index (ahand_t *a, int pai)
{
  int i;
  for (i = 0; i < a->rest_num; i++)
    {
      if (pai == a->rest[i])
        {
          return i;
        }
    }
  return -1;
}

void
ah_sort (ahand_t *a)
{
  int x, y, n;
  int tmp;

  n = a->rest_num;
  for (x = 0; x < n; x++)
    for (y = x; y < n; y++)
      {
        if (a->rest[x] > a->rest[y])
          {
            tmp = a->rest[x];
            a->rest[x] = a->rest[y];
            a->rest[y] = tmp;
          }
      }
}

void
ah_result_add (ahand_t *a)
{
  int i;
  int rest;
  int v2 = 0;
  int two = 0;
  int three = 0;
  int total = 0;
  int m, over;

  for (i = 0; i < a->part_num; i++)
    {
      if (a->part_kind[i] == AH_V3
          || a->part_kind[i] == AH_H3
          || a->part_kind[i] == AH_V3_FIXED || a->part_kind[i] == AH_H3_FIXED)
        {
          three++;
          total += 3;
        }
      else
        {
          two++;
          total += 2;
        }
      if (a->part_kind[i] == AH_V2)
        v2++;
    }

  three += (14 - total - a->rest_num) / 3;

  if (v2)
    m = two + three - 1;
  else
    m = two + three;

  if (m > 4)
    over = m - 4;
  else
    over = 0;

  rest = 9 - (three + three + two) + over;
  if (rest > 7 - v2)
    {
      rest = 7 - v2;
      if (v2 == 6)
        {
          int same = 0, j;
          for (i = 0; i < a->rest_num; i++)
            for (j = 0; j < a->part_num; j++)
              if (a->rest[i] == a->part[j][0])
                same++;
          if (same == a->rest_num)
            rest++;
        }
    }
  /* check no-ten */
  if (rest == 1 && three == 4 && a->rest_num == 1)
    {
      for (i = 0; i < a->part_num; i++)
        {
          if ((a->part_kind[i] == AH_V3
               || a->part_kind[i] == AH_V3_FIXED) &&
              (a->rest[0] == a->part[i][0]))
            {
              rest++;
              break;
            }
        }
    }
  if (rest == 1 && three == 3)
    {
      int m1, m2;
      m1 = m2 = 0;
      for (i = 0; i < a->part_num; i++)
        {
          switch (a->part_kind[i])
            {
            case AH_H2a:
              m1 = K_PREV (a->part[i][0]);
              m2 = K_NEXT (a->part[i][1]);
              break;
            case AH_H2b:
              m1 = K_PREV (a->part[i][0]);
              m2 = K_NEXT (a->part[i][1]);
              break;
            case AH_H2c:
              m1 = K_NEXT (a->part[i][0]);
              break;
            }
        }
      if ((m1 && m2 && ahand_kinds[m1] + ahand_kinds[m2] == 8)
          || (m1 && !m2 && ahand_kinds[m1] == 4)
          || (!m1 && m2 && ahand_kinds[m2] == 4))
        rest++;
    }

  if (result_rest > rest)
    {
      result_rest = rest;
      result_cnt = 0;
    }
  else if (result_rest < rest)
    {
      return;
    }

  if (result_cnt < MAX_RESULT)
    {
      bcopy (a, result_ahand + result_cnt, sizeof (ahand_t));
      result_cnt++;
    }
}

void
ah_analize (ahand_t *a, int from)
{
  ahand_t c;
  int i, j, n, same;
  int found = 0;
  int pai0, pai1, pai2, pai3;
  int idx2, idx3;

  n = a->rest_num;
  i = from;

  while (i < n - 1)
    {
      pai1 = a->rest[i];
      pai2 = K_NEXT2 (pai1);
      pai3 = pai2 ? K_NEXT2 (pai2) : 0;
      pai0 = K_PREV2 (pai1);
      idx2 = pai2 ? (ah_index (a, pai2)) : -1;
      idx3 = pai3 ? (ah_index (a, pai3)) : -1;

      if (found && found < pai1)
        break;

      if (pai1 == a->rest[i + 1] && (i + 3 >= n || pai1 != a->rest[i + 3]))
        {
          same = 0;
          for (j = 0; j < a->part_num; j++)
            {
              if (a->part_kind[j] == AH_V2 && pai1 == a->part[j][0])
                {
                  same = 1;
                  break;
                }
            }
          if (!same)
            {
              if (!found)
                found = pai1;
              bcopy (a, &c, sizeof (ahand_t));
              ah_remove (&c, i);
              ah_remove (&c, i);
              c.part[c.part_num][0] = pai1;
              c.part[c.part_num][1] = pai1;
              c.part_kind[c.part_num] = AH_V2;
              c.part_num++;
              ah_analize (&c, i);
            }
        }
      if (pai1 == a->rest[i + 1] && i < n - 2 && pai1 == a->rest[i + 2])
        {
          if (!found)
            found = pai1;
          bcopy (a, &c, sizeof (ahand_t));
          ah_remove (&c, i);
          ah_remove (&c, i);
          ah_remove (&c, i);
          c.part[c.part_num][0] = pai1;
          c.part[c.part_num][1] = pai1;
          c.part[c.part_num][2] = pai1;
          c.part_kind[c.part_num] = AH_V3;
          c.part_num++;
          ah_analize (&c, i);
        }
      if (idx2 >= 0)
        {
          if (!found)
            found = pai2;
          bcopy (a, &c, sizeof (ahand_t));
          ah_remove (&c, idx2);
          ah_remove (&c, i);
          c.part[c.part_num][0] = pai1;
          c.part[c.part_num][1] = pai2;
          c.part_kind[c.part_num] = (pai0 && pai3) ? AH_H2a : AH_H2b;
          c.part_num++;
          ah_analize (&c, i);
        }
      if (idx3 >= 0)
        {
          if (!found)
            found = pai3;
          bcopy (a, &c, sizeof (ahand_t));
          ah_remove (&c, idx3);
          ah_remove (&c, i);
          c.part[c.part_num][0] = pai1;
          c.part[c.part_num][1] = pai3;
          c.part_kind[c.part_num] = AH_H2c;
          c.part_num++;
          ah_analize (&c, i);
        }
      if (idx2 >= 0 && idx3 >= 0)
        {
          if (!found)
            found = pai3;
          bcopy (a, &c, sizeof (ahand_t));
          ah_remove (&c, idx3);
          ah_remove (&c, idx2);
          ah_remove (&c, i);
          c.part[c.part_num][0] = pai1;
          c.part[c.part_num][1] = pai2;
          c.part[c.part_num][2] = pai3;
          c.part_kind[c.part_num] = AH_H3;
          c.part_num++;
          ah_analize (&c, i);
        }
      while (a->rest[i] == pai1 && i < n)
        i++;
    }
  if (!found)
    ah_result_add (a);
}

void
ah_print (ahand_t *a, char *name)
{
  int i, j, n;

  if (!df)
    return;

  fprintf (df, "%s:", name);
  for (i = 0; i < a->rest_num; i++)
    fprintf (df, "%s", pai2str (a->rest[i] << 2));

  for (i = 0; i < a->part_num; i++)
    {
      fprintf (df, "[");
      switch (a->part_kind[i])
        {
        case AH_V3:
        case AH_H3:
        case AH_V3_FIXED:
        case AH_H3_FIXED:
          fprintf (df, "%s%s%s", pai2str (a->part[i][0] << 2),
                   pai2str (a->part[i][1] << 2),
                   pai2str (a->part[i][2] << 2));
          break;
        case AH_V2:
        case AH_H2a:
        case AH_H2b:
        case AH_H2c:
          fprintf (df, "%s%s", pai2str (a->part[i][0] << 2),
                   pai2str (a->part[i][1] << 2));
        }
      fprintf (df, "]");
    }
  fprintf (df, "\n");
}
