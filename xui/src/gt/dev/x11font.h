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
 * This file is part of xnetmaj
 * 
 * x11font.h -- 
 * 
 * $Id: x11font.h,v 1.2 1996/07/31 22:46:48 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Jan 25 23:30:21 1996
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Wed Jan 31 22:43:43 1996
 * 
 * Copyright (C) 1995,1996 Todo Software All rights reserved.
 */

#if 1 /*defined(__FreeBSD__)*/

#define ANYJPFONTSET14  \
   "-jis-fixed-medium-r-normal--16-110-100-100-c-160-jisx0208.1983-0,\
    -sony-fixed-medium-r-normal--16-120-100-100-c-80-jisx0201.1976-0,\
    -sony-fixed-medium-r-normal--16-120-100-100-c-80-iso8859-1"

#define JPFONTSET14  \
   "-misc-fixed-medium-r-normal--14-100-100-100-c-140-jisx0208.1983-0,\
    -misc-fixed-medium-r-normal--14-100-100-100-c-70-iso8859-1,\
    -misc-fixed-medium-r-normal--14-100-100-100-c-70-jisx0201.1976-0"

#define JPFONTSET10  \
   "-misc-fixed-medium-r-normal--14-100-100-100-c-140-jisx0208.1983-0,\
    -misc-fixed-medium-r-normal--14-100-100-100-c-70-iso8859-1,\
    -misc-fixed-medium-r-normal--14-100-100-100-c-70-jisx0201.1976-0"

#else

#define ANYJPFONTSET14  \
  "-*-fixed-medium-r-normal-*-14-*-*-*-*-*-jisx0208.1983-*,\
   -*-fixed-medium-r-normal-*-14-*-*-*-*-*-jisx0201.1976-*,\
   -*-*-*-*-*-*-14-*-*-*-*-*-iso8859-1,\
   -*-*-*-*-*-*-14-*-*-*-*-*-iso8859-2,\
   -*-*-*-*-*-*-14-*-*-*-*-*-iso8859-3,\
   -*-*-*-*-*-*-14-*-*-*-*-*-iso8859-4,\
   -*-*-*-*-*-*-14-*-*-*-*-*-iso8859-5,\
   -*-*-*-*-*-*-14-*-*-*-*-*-iso8859-7,\
   -*-*-*-*-*-*-14-*-*-*-*-*-iso8859-8,\
   -*-*-*-*-*-*-14-*-*-*-*-*-iso8859-9"

#define JPFONTSET14  \
    "-misc-marumoji-medium-r-normal--14-*-*-*-c-140-jisx0208.1983-0,\
     -misc-marumoji-medium-r-normal--14-*-*-*-c-70-iso8859-1,\
     -misc-marumoji-medium-r-normal--14-*-*-*-c-70-jisx0201.1976-0"

#define JPFONTSET10  \
    "-misc-marumoji-medium-r-normal--14-*-*-*-c-140-jisx0208.1983-0,\
     -misc-marumoji-medium-r-normal--14-*-*-*-c-70-iso8859-1,\
     -misc-marumoji-medium-r-normal--14-*-*-*-c-70-jisx0201.1976-0"

#endif
