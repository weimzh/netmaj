/*  NETMAJ-XUI -- X User Interface --  Copyright Notice
 *
 *      Copyright (c) 1995,1996 Todo Software All rights reserved.
 *      Copyright (C) 1994, 1995, 1996 Koji Suzuki (suz@kt.rim.or.jp)
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
 *   Koji Suzuki   : suz@kt.rim.or.jp
 */
/*  -*- Mode: C -*- 
 * This file is part of xui
 * 
 * x11jpstr.c -- 
 * 
 * $Id: x11jpstr.c,v 1.2 1996/07/31 22:46:57 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 22:55:47 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Fri Feb 23 01:48:12 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#include <stdio.h>
#include "gt/dev/x11gdev.h"

#if 1
#undef mbtowc
#define USE_XMBTOWC
#define USE_XWCHAR_STRING
#ifndef mbtowc
#ifdef USE_XWCHAR_STRING
#define wcslen(c) _Xwcslen(c)
#define wcscpy(d,s) _Xwcscpy(d,s)
#define wcsncpy(d,s,l) _Xwcsncpy(d,s,l)
#ifdef USE_XMBTOWC
#define mbtowc(wc,s,l) _Xmbtowc(wc,s,l)
#define mbstowcs(wc,s,l) _Xmbstowcs(wc,s,l)
#endif
#endif
#endif

#else
#include "byteord.h"

static 
mymbstowcs (wctext, text, len)
     wchar_t *wctext;
     char *text;
{
  wchar_t *base = wctext;
#if ENDIAN == 4321
  while (*text)
    {
      if (*text & 0x80)
	{
	  *(((char *) wctext) + 0) = 0;
	  *(((char *) wctext) + 1) = 0;
	  *(((char *) wctext) + 2) = *text;
	  *(((char *) wctext) + 3) = *(text + 1);
	  text += 2;
	}
      else
	{
	  *(((char *) wctext) + 0) = 0;
	  *(((char *) wctext) + 1) = 0;
	  *(((char *) wctext) + 2) = 0;
	  *(((char *) wctext) + 3) = *(text);
	  text++;
	}
      wctext++;
      if (wctext - base >= len)
	break;
    }
#else
  while (*text)
    {
      if (*text & 0x80)
	{
	  *(((char *) wctext) + 0) = *(text + 1);
	  *(((char *) wctext) + 1) = *text;
	  *(((char *) wctext) + 2) = 0;
	  *(((char *) wctext) + 3) = 0;
	  text += 2;
	}
      else
	{
	  *(((char *) wctext) + 0) = *(text);
	  *(((char *) wctext) + 1) = 0;
	  *(((char *) wctext) + 2) = 0;
	  *(((char *) wctext) + 3) = 0;
	  text++;
	}
      wctext++;
      if (wctext - base >= len)
	break;
    }
#endif
  *wctext = 0;
}
#define mbstowcs mymbstowcs
#endif

void 
DrawString16 (display, window, jf,f, gc, x, y, text, len)
     Display *display;
     Window window;
     XFontStruct *jf,*f;
     GC gc;
     integer_t x, y;
     char *text;
     integer_t len;
{
  int i;
  XChar2b kbuf[1];
  for (i=0; i< len; i++) {
		if (text[i] & 0x80) {
			kbuf[0].byte1 = text[i] & 0x7f;
			i++;
			kbuf[0].byte2 = text[i] & 0x7f;
			XSetFont (display, gc, jf->fid);
  			XDrawString16 (display, window, gc, x, y, kbuf, 1);
			x += XTextWidth16(jf,kbuf,1)+1;
		} else {
			XSetFont (display, gc, f->fid);
  			XDrawString (display, window, gc, x, y, text+i, 1);
			x += XTextWidth(f,text+i,1)+1;
		}
  }
}
