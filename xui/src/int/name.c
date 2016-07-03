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
 * This file is part of my project
 * 
 * name.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:48:43 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "nu.h"

#define NODEHASHTABLESIZE 517
node_t node_table[NODEHASHTABLESIZE];

static node_t
node_new (void)
{
  node_t node = (node_t) xcalloc (_sizeof (struct node));
  xkind (node) = kind_node;
  return node;
}

static name_t
name_new (void)
{
  name_t name = (name_t) xcalloc (_sizeof (struct name));
  xkind (name) = kind_name;
  return name;
}

#define         name_table  s_str
#define         nlist_table s_syms

extern chunk_t name_table;
extern chunk_t nlist_table;


static node_t
get_node (expr_t ident)
{
  unsigned int key;
  node_t p;
  char *sp = xident (ident);
  char *name = sp;

  assert_i (xid (ident) == id_ident);

  for (key = 0; *sp; sp++)
    {
      key += (*sp);
    }
  key = key % NODEHASHTABLESIZE;

  p = node_table[key];
  while (p != 0)
    {
      if (!strcmp (p->key, name))
	{
	  return p;
	}
      p = p->hashnext;
    }
  p = (pointer) node_new ();
  p->key = xident (ident);

  p->hashnext = node_table[key];
  node_table[key] = p;
  return p;
}


name_t
search_name (expr_t ident)
{
  node_t np;

  np = get_node (ident);
  return np->name;
}


static name_t name_list;

name_t
intern_name (expr_t ident)
{
  node_t np;
  name_t p;
  np = get_node (ident);
  p = name_new ();
  p->samenamelink = np->name;
  np->name = p;
  p->node = np;
  p->key = xident (ident);

  p->next = name_list;
  name_list = p;
  return p;
}

