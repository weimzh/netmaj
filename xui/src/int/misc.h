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
 * misc.h -- 
 * 
 * $Id: misc.h,v 1.2 1996/07/31 22:48:40 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:49:36 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Tue May  9 00:49:37 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#ifndef _misc_h
#define _misc_h

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#ifndef EZERO
#define EZERO 0
#endif

typedef struct object {
  integer_t       kind;
}              *object_t;

#define xkind(p)        ((object_t)(p))->kind

typedef struct chunk {
  _size_t         used;
  _size_t         pos;
  _size_t         size;
  byte_t         *text;
}              *chunk_t;

typedef unsigned int vaddr_t;
typedef unsigned int unsigned_t;

typedef enum {
  kind_expr,
  kind_name,
  kind_node,
  kind_chunk,
}               kind_t;
#endif				/* _misc_h */
