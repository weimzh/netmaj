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
 * This file is part of xnetmaj
 * 
 * pmutil.h -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:14:44 1995
 * 
 */


#define pmcontainer_print(w, p) \
    if (p.pm) { pmcontainer_clear(w), pmcontainer_add(w, p.pm, p.Mpm, 1); }

#define print_yakupoint(w, p) \
  {\
      char buf[32]; \
      sprintf(buf,"%6d",p); \
      widget_configure(w, resource_text, buf); \
  }

  /* assert(widget_text_p(w)); */
  /*_print_digit(w, p);*/

/* #define      print_pixmap(xpm) \
   if(xpm) {pmcontainer_add(canvas, xpm, 0, 0);} */

#define	print_colorpixmap(xpm) \
	     if(xpm.pm) {pmcontainer_add(canvas, xpm.pm, xpm.Mpm, 1);}

#define	print_colorpixmap2(arg) \
	     { gpixmap2_t xpm = arg; \
		 if((*xpm).pm) {pmcontainer_add(canvas, (*xpm).pm, (*xpm).Mpm, 1);} }

#define	print_colorpixmap2_withmargin(arg, margin) \
	     { gpixmap2_t xpm = arg; \
		 if((*xpm).pm) {pmcontainer_add_withmargin(canvas, (*xpm).pm, (*xpm).Mpm, 1, margin);} }

#define	print_colorpixmap2_withofs(arg, ofs) \
	     { gpixmap2_t xpm = arg; \
		 if((*xpm).pm) {pmcontainer_add_withofs(canvas, (*xpm).pm, (*xpm).Mpm, 1, ofs);} }

#define	print_colorpixmap_attr(arg, ofs) \
	     { gpixmap2_t xpm = arg; \
               if((*xpm).pm) {\
	 pmcontainer_add_withofs(canvas, (*xpm).pm, (*xpm).Mpm, 1, ofs);} }



#define	_print_colorpixmap(w, xpm) \
         if(xpm.pm) {pmcontainer_add(w, xpm.pm, xpm.Mpm, 1);}

#define	canvas_clear() \
         pmcontainer_clear(canvas)
