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

#ifndef AHAND_H
#define AHAND_H

typedef struct
{
  pai_t *h;
  int rest[14];
  int rest_num;
  int part_num;
  int part[7][3];
  int part_kind[7];
} ahand_t;

#define AH_V3_FIXED	1
#define AH_H3_FIXED	2
#define AH_V3		3
#define AH_V2		4
#define AH_H3		5
#define AH_H2a		6
#define AH_H2b		7
#define AH_H2c		8

#define MAX_RESULT	100

extern ahand_t result_ahand[MAX_RESULT];
extern int result_cnt;
extern int result_rest;

#endif
