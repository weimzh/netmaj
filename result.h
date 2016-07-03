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
 * 5-7-2006 Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#ifndef RESULT_H
#define RESULT_H

#define	Y_WORD(x)	((x)/15)
#define Y_BIT(x)	(1<<((x)%15))
#define Y_GET(y,x)	((y[Y_WORD(x)] & Y_BIT(x)) != 0)
#define Y_SET(y,x)	(y[Y_WORD(x)] |= Y_BIT(x))

#define Y_MAX	60

extern char *result_str[];

#endif

