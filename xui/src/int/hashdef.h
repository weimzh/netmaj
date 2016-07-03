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
 *  See the gpl.text for more details.
 *
 *   If you would like to do something with NETMAJ that this copyright
 *  prohibits (such as distributing it with a commercial product,
 *  using portions of the source in some other program, etc.), please
 *  contact the author (preferably via email).  Arrangements can
 *  probably be worked out.
 *
 *   Todo Software : NBC02365@niftyserve.or.jp
 */
/*  -*- Mode: C -*- 
 * This file is part of my project
 * 
 * hashdef.h -- 
 * 
 * $Id: hashdef.h,v 1.2 1996/07/31 22:48:35 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:48:31 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Tue May  9 00:48:32 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

typedef struct name {
  integer_t       kind;
  boolean         extern_p;
  integer_t       allocpos;     /**/
  integer_t       ofs;		/* extern_p == 0 then ofs from fp */
  cardinal_t      i_string;	/* then i_string is index in string */
  cardinal_t      i_nlist;	/* extern_p == 1 then i_nlist is index in
				 * nlist */
  cardinal_t      level;	/* block level */
  struct name    *sameblocklink;
  struct name    *samenamelink;
  struct node    *node;
  expr_t          macrodef;     /* macro tree */
  char *key;                    /* for debug */
  struct name    *next;
  integer_t n_type;

/* for interp */
  void *value;
} *name_t;

#define namealloc_local   1
#define namealloc_global  2
#define namealloc_param   3
#define namealloc_static  4
typedef struct node {
  integer_t       kind;
  struct node    *hashnext;
  char           *key;
  cardinal_t      i_string;
  name_t          name;
}              *node_t;
