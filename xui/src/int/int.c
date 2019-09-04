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
 * This file is part of xnetmaj
 * 
 * int.c -- 
 * 
 * $Id: int.c,v 1.2 1996/07/31 22:48:37 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:47:56 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Tue May  9 00:48:05 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

/* interface file for nu */
#include "nu.h"

#define xnil nil()

static void
assert_nil (expr_t p)
{
  if (p != xnil)
    {
      nu_error ("syntax error: nil expected");
    }
}

static void
assert_nil1 (expr_t p, char *s)
{
  if (p != xnil)
    {
      nu_error ("syntax error: nil expected: %s", s);
    }
}


static void
assert_expr (expr_t p)
{
  if (!expr_p (p))
    {
      nu_error ("syntax error: expr expected");
    }
}

static void
assert_integer (expr_t p)
{
  if (!integer_p (p))
    {
      nu_error ("syntax error: integer expected");
    }
}

static void
assert_integer1 (expr_t p, char *s)
{
  if (!integer_p (p))
    {
      nu_error ("syntax error: integer expected: %s", s);
    }
}


static void
assert_expr1 (expr_t p, char *s)
{
  if (xid (p) != id_expr)
    {
      nu_error ("syntax error: expr expected: %s", s);
    }
}

boolean
ident_p (expr_t e)
{
  if (xid (e) == id_ident)
    {
      return 1;
    }
  else
    {
      nu_error ("identifier expected");
      return 0;
    }
}

void
assert_ident (e)
     expr_t e;
{
  if (!ident_p (e))
    {
      nu_error ("syntax error: identifier expected");
    }
}
void
assert_ident1 (e, s)
     expr_t e;
     char *s;
{
  if (!ident_p (e))
    {
      nu_error (s);
    }
}

pointer 
interp_if (p)
     pointer p;
{
  return 0;
}
pointer 
interp_while (p)
     pointer p;
{
  return 0;
}
pointer 
interp_proc (p)
     pointer p;
{
  return 0;
}
pointer 
interp_lambda (p)
     pointer p;
{
  return 0;
}
pointer 
interp_break (p)
     pointer p;
{
  return 0;
}
pointer 
interp_continue (p)
     pointer p;
{
  return 0;
}
pointer 
interp_declare (p)
     pointer p;
{
  return 0;
}
pointer 
interp_define (p)
     pointer p;
{
  grammer ("(define identifier expr)");
  assert_expr1 (p, "define destination expected");
  assert_expr1 (cdr (p), "define source expected");

  if (ident_p (car (p)))
    {
      expr_t ident = car (p);
      expr_t vep;
      name_t np;

      np = search_name (ident);	/* identifier */
      if (!np)
	{
	  np = intern_name (ident);
	}

      vep = car (cdr (p));
      if (xid (vep) == id_integer)
	{
	  np->value = vep;
	}
      else if (xid (vep) == id_string)
	{
	  np->value = vep;
	}
      else
	{
	  np->value = 0;
	}
    }
  else
    {
      nu_error ("define: identifier expected");
    }
}

pointer 
interp_block (p)
     pointer p;
{
  return 0;
}
pointer 
interp_prim (p)
     pointer p;
{
  return 0;
}
pointer 
interp_exit (p)
     pointer p;
{
  return 0;
}
pointer 
interp_begin (p)
     pointer p;
{
  return 0;
}
pointer 
interp_or (p)
     pointer p;
{
  return 0;
}
pointer 
interp_and (p)
     pointer p;
{
  return 0;
}
pointer 
interp_macro (p)
     pointer p;
{
  return 0;
}


static pointer
interp_special_form (integer_t op, expr_t p)
{
  switch (op)
    {
    case op_if:
      return interp_if (p);
    case op_while:
      return interp_while (p);
    case op_proc:
      return interp_proc (p);
    case op_lambda:
      return interp_lambda (p);
    case op_break:
      return interp_break (p);
    case op_continue:
      return interp_continue (p);
    case op_declare:
      return interp_declare (p);
    case op_define:
      return interp_define (p);
    case op_block:
      return interp_block (p);
    case op_prim:
      return interp_prim (p);
    case op_exit:
      return interp_exit (p);
    case op_begin:
      return interp_begin (p);
    case op_or:
      return interp_or (p);
    case op_and:
      return interp_and (p);
    case op_macro:
      return interp_macro (p);
    default:
      abort ();
    }
}



expr_t
interp (expr_t p)
{
  expr_t sym;
  expr_t cp;

  switch (xid (p))
    {
    case id_expr:
      cp = car (p);
      if (xid (cp) == id_op)
	{
	  integer_t op = xop (car (p));
	  interp_special_form (op, cdr (p));
	}
      return 0;
      abort ();
      break;
    case id_ident:
      {
	name_t np;
	np = search_name (p);	/* identifier */
	if (!np)
	  {
	    np = intern_name (p);
	  }
	return (void *) np;
      }
      break;
    case id_string:
      return p;
    case id_integer:
      return p;
    case id_sym:
      return p;
    case id_op:
      nu_error ("syntax error %O", (integer_t) xop (p));
      break;
    default:
      break;
    }
  return 0;
}


boolean 
nu_source (fname)
     char *fname;
{
  pointer p;
  FILE *fp;

  if (!fname)
    {
      fp = stdin;
    }
  else
    {
      fp = fopen (fname, "r");
      if (!fp)
	{
	  return false;
	}
    }
  nu_init ();
  nu_set_input (fp);
  for (;;)
    {
      p = readexpr ();
      if (nu_eof (p))
	{
	  break;
	}
      interp (p);
    }
  nu_finish ();

  if (fp != stdin)
    {
      fclose (fp);
    }
  return true;
}


void *
name_value (name)
     char *name;
{
  name_t np = search_name (mk_ident (name));
  if (np)
    {
      expr_t vep = np->value;
      if (xid (vep) == id_integer)
	{
	  return (void *) xint (vep);
	}
      else if (xid (vep) == id_string)
	{
	  return (void *) xstr (vep);
	}
      return "";
    }
  return "";
}

boolean 
name_exist (name)
     char *name;
{
  name_t np = search_name (mk_ident (name));
  return !!np;
}
