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
 * readexpr.c -- 
 * 
 * $Id: readexpr.c,v 1.2 1996/07/31 22:48:48 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:48:48 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jul 20 18:53:33 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "nu.h"

static integer_t inputc (void);
/* static FILE *inputfp = stdin; */

void 
nu_set_input (fp)
     FILE *fp;
{
  stdin = fp;
}

static integer_t
inputc ()
{
  return getc (stdin);
}

enum tok_t
{
  L_NAME,
  L_INT,
  L_STR,
  L_LP,
  L_RP,
  L_VEC,
  L_CHAR,
  L_QUOTE,
  L_BQUOTE,
  L_EOF,
};

#define MAXTOKENLEN 1024

static struct input
  {
    char buffer[MAXTOKENLEN];
    cardinal_t pos;
  }
input;

static integer_t cchar = ' ';
#define input_reset()  input.pos = 0;
#define setc(c)        \
  if (input.pos >= MAXTOKENLEN-1) { nu_error("too long token"); } \
  else { input.buffer[input.pos++] = (signed char)c; }
#define input_str      input.buffer

/* readexpr.c */
static integer_t inputc (void);
static integer_t token (void);

static integer_t
token ()
{
restart:
  input_reset ();

  while (isspace (cchar) && cchar != '\n')
    {
      cchar = inputc ();
    }
  if (cchar == '#')
    {
      cchar = inputc ();
      while (cchar != '\n')
	{
	  cchar = inputc ();
	}
      goto restart;
    }
  if (isalpha (cchar) || cchar == '_')
    {
    ident:
      while (isalpha (cchar) || isdigit (cchar) || cchar == '_')
	{
	  setc (cchar);
	  cchar = inputc ();
	}
      setc ('\0');
      return L_NAME;
    }
  else if (isdigit (cchar))
    {
    digit:
      while (isdigit (cchar))
	{
	  setc (cchar);
	  cchar = inputc ();
	}
      setc ('\0');
      return L_INT;
    }
  switch (cchar)
    {
    case '-':
      {
	setc (cchar);
	cchar = inputc ();
	goto digit;
      }
    case '%':
      {
	cchar = inputc ();
	if (cchar == '(')
	  {
	    cchar = inputc ();
	    return L_VEC;
	  }
	else
	  {
	    nu_error ("illegal character %%: '(' required to describe vector");
	    goto restart;
	  }
      }
    case '`':
    case '"':
    case '\'':
      {
	integer_t endch = cchar;
	integer_t ret = cchar != '\'' ? L_STR : L_CHAR;
	cchar = inputc ();
	while (cchar != endch && cchar != EOF)
	  {
	    if (cchar == '\\')
	      {
		cchar = inputc ();
		setc (cchar);
	      }
	    else
	      {
		setc (cchar);
	      }
	    cchar = inputc ();
	  }
	setc ('\0');
	cchar = inputc ();
	return ret;
      }
    case '(':
      cchar = inputc ();
      return L_LP;
    case ')':
      cchar = inputc ();
      return L_RP;
    case EOF:
      return L_EOF;
    case '\n':
      nuline++;
      cchar = inputc ();
      goto restart;
    }
  nu_error ("illegal character 0x%x", cchar);
  cchar = inputc ();
}


/* transition */
#define READBEGIN  0
#define READEND    1
#define READEXPR   2
#define READLIST   3
#define READVECTOR 4

#define MAXREADSTACKNEST 256
static struct
{
  integer_t op;
  pointer v;
}
read_stack[MAXREADSTACKNEST];
static integer_t read_sp;


#define _push(_op, pos) \
  read_sp++; read_stack[read_sp].op = _op; read_stack[read_sp].v = pos;

#define _goto(_op) \
  op = _op; goto LOOP;

#define _return(_v) \
  value = _v, op = read_stack[read_sp].op; \
  v = read_stack[read_sp].v; read_sp--; goto LOOP;

#define xnil nil()


pointer
readexpr ()
{
  integer_t op = READBEGIN;
  pointer v;
  pointer value = xnil;
  integer_t tok;

  v = xnil;

LOOP:
  switch (op)
    {
    case READBEGIN:
      _push (READEND, v);
      tok = token ();
      _goto (READEXPR);
    case READEND:
      return value;
    case READEXPR:
      switch (tok)
	{
	case L_NAME:
	  _return (mk_name (input_str));
	  /* not reach here */
	case L_INT:
	  _return (mk_integer (_atoi (input_str)));
	  /* not reach here */
	case L_STR:
	  _return (mk_string (input_str));
	  /* not reach here */
	case L_CHAR:
	  _return (mk_char (input_str));
	  /* not reach here */
	case L_LP:
	  tok = token ();
	  if (tok == L_RP)
	    {
	      _return (xnil);
	    }
	  else
	    {
	      _push (READLIST, xnil);
	      _goto (READEXPR);
	    }
	  /* not reach here */
	case L_RP:
	  break;
	case L_VEC:
	  break;
	case L_EOF:
	  return 0;
	default:
	  abort ();
	}
      break;
    case READVECTOR:
      v = xcons (value, v);
      tok = token ();
      if (tok == L_RP)
	{
	  nu_error ("illegal '('");
	}
      else if (tok == L_EOF)
	{
	  nu_error ("illegal eof");
	}
      else
	{
	  _push (READVECTOR, v);
	  _goto (READEXPR);
	}
      break;
    case READLIST:
      v = xcons (value, v);
      tok = token ();
      if (tok == L_RP)
	{
	  _return (xreverse (xnil, v));
	}
      else if (tok == L_EOF)
	{
	  nu_error ("illegal eof");
	}
      else
	{
	  _push (READLIST, v);
	  _goto (READEXPR);
	}
      break;
    default:
      nu_error ("internal error in readexpr");
    }
  return value;
}

boolean 
nu_eof (p)
     pointer p;
{
  if (p == 0 || p == nil ())
    {
      return 1;
    }
  return 0;
}
