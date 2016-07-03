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
 * error.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:48:57 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Thu Jun  1 18:18:20 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "nu.h"


void
grammer (s)
     char *s;
{
}

static char *
xop_string (op)
     integer_t op;
{
  switch (op)
    {
    case op_if:
      return "if";
    case op_while:
      return "while";
    case op_declare:
      return "declare";
    case op_proc:
      return "proc";
    case op_lambda:
      return "lambda";
    case op_break:
      return "break";
    case op_continue:
      return "continue";
    case op_define:
      return "define";
    case op_block:
      return "block";
    case op_prim:
      return "prim";
    case op_exit:
      return "exit";
    case op_begin:
      return "begin";
    case op_or:
      return "or";
    case op_and:
      return "and";
    default:
      abort ();
    }
}

#if MSDOS
#define LD  "%ld"
#define LO  "%lo"
#define LU  "%lu"
#define LX  "%lx"
#else
#define LD  "%d"
#define LO  "%o"
#define LU  "%u"
#define LX  "%x"
#endif

/* static FILE *outfp = stderr; */

static char *
message (msg, argp)
     char *msg;
     va_list *argp;
{
  char *start;

  for (;;)
    {
      if (*msg == '\0')
	{
	  break;
	}
      else if (*msg == '%')
	{
	  msg++;
	  switch (*msg)
	    {
	    case 's':
	      fprintf (stderr, "%s", va_arg (*argp, char *));
	      break;
	    case 'd':
	      fprintf (stderr, LD, va_arg (*argp, integer_t));
	      break;
	    case 'u':
	      fprintf (stderr, LU, va_arg (*argp, integer_t));
	      break;
	    case 'o':
	      fprintf (stderr, LO, va_arg (*argp, integer_t));
	      break;
	    case 'x':
	      fprintf (stderr, LX, va_arg (*argp, integer_t));
	      break;
	    case 'O':
	      fprintf (stderr, "%s", xop_string (va_arg (*argp, integer_t)));
	      break;
	    case '%':
	      fprintf (stderr, "%%");
	      break;
	    default:
	      fprintf (stderr, "unknown fmt char 0x%x\n", *msg);
	      break;
	    }
	  msg++;
	}
      else
	{
	  start = msg;
	  for (;;)
	    {
	      switch (*msg)
		{
		case '\0':
		  goto break1;
		case '%':
		  goto break1;
		}
	      msg++;
	    }
	break1:
	  fwrite (start, 1, msg - start, stderr);
	}
    }				/* for */
}


#define def_msg_func(funcname, string, done) \
void funcname(char *msg, ...)\
{\
  va_list args;\
  fprintf(stderr, "%s:%d:nu:%s: ", nufile, nuline, string);\
  va_start (args, msg);\
  message(msg, &args);\
  va_end(args);\
  fprintf(stderr, "\n"); done;\
}

def_msg_func (nu_error, "error", 0);
def_msg_func (nu_warning, "warning", 0);
def_msg_func (nu_fatal, "fatal", exit (1));


/* end of */
