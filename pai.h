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
 * 5-7-2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#ifndef _PAI_H
#define _PAI_H

#ifdef SYSV
#define srandom		srand
#define random		rand
#endif

typedef unsigned char pai_t;

#define P_NULL          0
#define P_INV           1
#define P_INV2          2
#define P_INV3          3

#define P_KIND(n)       ((n)>>2)

#define K_MAN           1
#define K_PIN           10
#define K_SOU           19
#define K_KAZ           28
#define K_3GN           32
#define K_MAX           35

#define K_NULL		 0

#define K_MAN_1		 1
#define K_MAN_2		 2
#define K_MAN_3		 3
#define K_MAN_4		 4
#define K_MAN_5		 5
#define K_MAN_6		 6
#define K_MAN_7		 7
#define K_MAN_8		 8
#define K_MAN_9		 9

#define K_PIN_1		10
#define K_PIN_2		11
#define K_PIN_3		12
#define K_PIN_4		13
#define K_PIN_5		14
#define K_PIN_6		15
#define K_PIN_7		16
#define K_PIN_8		17
#define K_PIN_9		18

#define K_SOU_1		19
#define K_SOU_2		20
#define K_SOU_3		21
#define K_SOU_4		22
#define K_SOU_5		23
#define K_SOU_6		24
#define K_SOU_7		25
#define K_SOU_8		26
#define K_SOU_9		27

#define K_TON		28
#define K_NAN		29
#define K_SHA		30
#define K_PEI		31

#define K_HAKU		32
#define K_HATU		33
#define K_CHUN		34

#define IS_TAN(x)	( (K_MAN_1 < (x) && (x) < K_MAN_9) \
			||(K_PIN_1 < (x) && (x) < K_PIN_9) \
			||(K_SOU_1 < (x) && (x) < K_SOU_9) )

#define IS_YAO(x)	(!(IS_TAN(x)))

#define IS_JUNYAO(x)	(  K_MAN_1 == (x) || (x) == K_MAN_9 \
			|| K_PIN_1 == (x) || (x) == K_PIN_9 \
			|| K_SOU_1 == (x) || (x) == K_SOU_9 )

#define IS_MAN(x)	(K_MAN_1 <= (x) && (x) <= K_MAN_9)
#define IS_PIN(x)	(K_PIN_1 <= (x) && (x) <= K_PIN_9)
#define IS_SOU(x)	(K_SOU_1 <= (x) && (x) <= K_SOU_9)
#define IS_JI(x)	(K_TON  <= (x) && (x) <= K_CHUN)

#define IS_GREEN(x)	((x) == K_HATU || (x) == K_SOU_2 || (x) == K_SOU_3 \
			|| (x) == K_SOU_4 || (x) == K_SOU_6 || (x) == K_SOU_8)

#define K_NEXT(x)	((x)==K_NULL?K_NULL \
			:(x)==K_MAN_9?K_MAN_1 \
			:(x)==K_PIN_9?K_PIN_1 \
			:(x)==K_SOU_9?K_SOU_1 \
			:(x)==K_PEI?K_TON \
			:(x)==K_CHUN?K_HAKU \
			:(x)+1)

#define K_PREV(x)	((x)==K_NULL?K_NULL \
			:(x)==K_MAN_1?K_MAN_9 \
			:(x)==K_PIN_1?K_PIN_9 \
			:(x)==K_SOU_1?K_SOU_9 \
			:(x)==K_TON?K_PEI \
			:(x)==K_HAKU?K_CHUN \
			:(x)-1)

#define K_NEXT2(p) (( (K_MAN_1 <= (p) && (p) <= K_MAN_8) \
		    ||(K_PIN_1 <= (p) && (p) <= K_PIN_8) \
		    ||(K_SOU_1 <= (p) && (p) <= K_SOU_8) )?((p)+1):0)

#define K_PREV2(p) (( (K_MAN_2 <= (p) && (p) <= K_MAN_9) \
		    ||(K_PIN_2 <= (p) && (p) <= K_PIN_9) \
		    ||(K_SOU_2 <= (p) && (p) <= K_SOU_9) )?((p)-1):0)


typedef int attr_t;

#define R_MASK		0007
#define F_MASK		0070
#define T_MASK		0700

#define R_SHFT		0
#define F_SHFT		3
#define T_SHFT		6

#define R_GET(x)	((x) & R_MASK)
#define F_GET(x)	((x) & F_MASK)
#define T_GET(x)	((x) & T_MASK)

#define R_SET(x,val)	(x = x & ~R_MASK | (val) & R_MASK)
#define F_SET(x,val)	(x = x & ~F_MASK | (val) & F_MASK)
#define T_SET(x,val)	(x = x & ~T_MASK | (val) & T_MASK)

#define F_MT		(0<<F_SHFT)
#define F_MT_TOP	(1<<F_SHFT)
#define F_RV_TIE	(2<<F_SHFT)
#define F_RV_PON	(3<<F_SHFT)
#define F_RV_KAN	(4<<F_SHFT)

#define T_RV		(0<<T_SHFT)
#define T_RV_RCH	(1<<T_SHFT)
#define T_HN_KAN	(2<<T_SHFT)
#define T_HN_TSM	(3<<T_SHFT)
#define T_HN_KAN_C	(4<<T_SHFT)

#define R_NUL		(0<<R_SHFT)
#define R_TIE		(1<<R_SHFT)
#define R_PON		(2<<R_SHFT)
#define R_KAN		(3<<R_SHFT)
#define R_RON		(4<<R_SHFT)
#define R_ACK		(5<<R_SHFT)

#define T_CHARS		" !4T4..."
#define F_CHARS		"  nnn..."
#define R_CHARS		" tpkR..."


#define H_NULL		0
#define H_TIE		1
#define H_PON		2
#define H_KAN_OPENED	3
#define H_KAN_CLOSED	4

extern char *pai2str (pai_t n);
extern char *attr2str (attr_t n);

/* program kind --- parameter of ui_init() */
#define PROG_NETMAJ 1
#define PROG_NETMAJ1 2
#define PROG_PLVIEW 3

#endif
