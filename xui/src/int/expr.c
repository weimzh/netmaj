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
 * expr.c -- 
 * 
 * $Id: expr.c,v 1.2 1996/07/31 22:48:31 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:49:06 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Tue Sep 26 19:21:13 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "nu.h"

static struct expr *expr_new (void);
static void string_init (void);
static char *string_new (unsigned int l);
static char *stringsave (char *s);
static char *identsave (char *s);

#define MAXEXPR 4096
static expr_t exprlist;
static cardinal_t n_expr;
static cardinal_t freeexpr;
static expr_t xnil;

expr_t
nil ()
{
  return xnil;
}

static expr_t
expr_new ()
{
#if 0
  if (!exprlist)
    {
      expr_init ();
    }
  if (freeexpr >= n_expr)
    {
      abort ();
    }
  return (expr_t) memset ((char *) &exprlist[freeexpr++], 0, _sizeof (struct expr));
#else
  return (expr_t) xcalloc (sizeof (struct expr));
#endif
}


#define MAXSTRING 10000
static char *string;
static cardinal_t n_string;
static cardinal_t freestring;

#if 0
static void
string_init ()
{
  n_string = MAXSTRING;
  string = (char *) xmalloc (n_string);
}

static char *
string_new (l)
     cardinal_t l;
{
  char *p;
  if (!string)
    string_init ();
  p = string + freestring;
  freestring += l;
  if (freestring >= MAXSTRING)
    {
      abort ();
    }
  return p;
}

#endif

static char *
stringsave (s)
     char *s;
{
#if 0
  cardinal_t len = strlen (s) + 1;
  char *p = string_new (len);
  return strcpy (p, s);
#else
  return strdup (s);
#endif
}

static char *
identsave (s)
     char *s;
{
  return stringsave (s);
}

expr_t
mk_char (s)
     char *s;
{
  char *p = xstr (mk_string (s));
  return mk_integer (*p);
}

expr_t
mk_integer (i)
     integer_t i;
{
  expr_t p = expr_new ();
  xid (p) = id_integer;
  xint (p) = i;
  return p;
}

expr_t
mk_expr (a, b)
     expr_t a, b;
{
  return xcons (a, b);
}

expr_t
mk_string (s)
     char *s;
{
  expr_t p = expr_new ();
  xid (p) = id_string;
  xstr (p) = stringsave (s);
  return p;
}

expr_t
mk_ident (s)
     char *s;
{
  expr_t p = expr_new ();
  xid (p) = id_ident;
  xident (p) = identsave (s);
  return p;
}

expr_t
mk_sym (s)
     integer_t s;
{
  expr_t p = expr_new ();
  xid (p) = id_sym;
  xsym (p) = s;
  return p;
}

expr_t
mk_op (op)
     integer_t op;
{
  expr_t p = expr_new ();
  xid (p) = id_op;
  xop (p) = op;
  return p;
}

expr_t
mk_name (s)
     char *s;
{
  static struct
    {
      char *s;
      integer_t op;
    }
  reserved[] =
  {
    {
      "if", op_if
    }
    ,
    {
      "while", op_while
    }
    ,
    {
      "break", op_break
    }
    ,
    {
      "continue", op_continue
    }
    ,
    {
      "break", op_break
    }
    ,
    {
      "lambda", op_lambda
    }
    ,
    {
      "block", op_block
    }
    ,
    {
      "declare", op_declare
    }
    ,
    {
      "define", op_define
    }
    ,
    {
      "prim", op_prim
    }
    ,
    {
      "exit", op_exit
    }
    ,
    {
      "begin", op_begin
    }
    ,
    {
      "or", op_or
    }
    ,
    {
      "and", op_and
    }
    ,
    {
      "macro", op_macro
    }
    ,
    {
      "proc", op_proc
    }
    ,
    {
      0, 0
    }
    ,
  }
  ,
    *p;
  p = reserved;

  for (; p->s; p++)
    {
      if (!strcmp (p->s, s))
	{
	  return mk_op (p->op);
	}
    }

  return mk_ident (s);
}

void
expr_reset ()
{
  freeexpr = 0;
  freestring = 0;
  xnil = mk_sym (sym_nil);
}

void
expr_init ()
{
  exprlist = (expr_t) xcalloc (_sizeof (struct expr) * (n_expr = MAXEXPR));
  expr_reset ();
}

pointer
xreverse (term, list)
     pointer term, list;
{
  register pointer p = list, result = term, q;

  while (p != xnil)
    {
      q = cdr (p);
      cdr (p) = result;
      result = p;
      p = q;
    }
  return result;
}

pointer
xcons (a, b)
     pointer a, b;
{
  pointer p;

  p = expr_new ();

  car (p) = a;
  cdr (p) = b;
  xid (p) = id_expr;

  return p;
}

pointer
xappend (c, d)
     pointer c, d;
{
  pointer l = c;
  pointer x = c;

  while (cell_p (x))
    {
      l = x;
      x = cdr (x);
    }
  if (cell_p (l))
    {
      cdr (l) = d;
      return c;
    }
  return d;
}

integer_t
xlength (p)
     pointer p;
{
  integer_t i = 0;
  while (cell_p (p))
    {
      i++;
      p = cdr (p);
    }
  return i;
}


/** print **/
/* static FILE *fp = stderr; */

void
expr_print (p)
     pointer p;
{
  switch (xid (p))
    {
    case id_expr:
      fprintf (stderr, "(");
      for (; cell_p (p); p = cdr (p))
	{
	  fprintf (stderr, " ");
	  expr_print (car (p));
	}
      if (p != xnil)
	{
	  fprintf (stderr, " . ");
	  expr_print (p);
	}
      fprintf (stderr, " )");
      break;
    case id_ident:
      fprintf (stderr, "%s", xident (p));
      break;
    case id_string:
      fprintf (stderr, "\"%s\"", xstr (p));
      break;
    case id_integer:
      fprintf (stderr, "%d", xint (p));
      break;
    case id_sym:
      fprintf (stderr, ".%d", xsym (p));
      break;
    case id_op:
      fprintf (stderr, "#%d", xop (p));
      break;
    default:
      fprintf (stderr, "unknown %d", xid (p));
    }
  fflush (stderr);
}

/* end of file */
