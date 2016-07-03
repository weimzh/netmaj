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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef int integer_t;

#define cmdname "xnetmaj"

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
  fprintf(stderr, "%s:%s: ", cmdname, string);\
  va_start (args, msg);\
  message(msg, &args);\
  va_end(args);\
  fprintf(stderr, "\n"); done;\
}

def_msg_func (error, "error", 0);
def_msg_func (warning, "warning", 0);
def_msg_func (fatal, "fatal", exit (1));

