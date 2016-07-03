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
 * name.h -- 
 * 
 * $Id: name.h,v 1.2 1996/07/31 22:48:43 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:49:45 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Tue May  9 00:49:46 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

/* name.c */
name_t name_new(void);
void name_table_init(void);
cardinal_t name_table_regist(char *s);
void nlist_table_init(void);
void nlist_table_set(integer_t, integer_t n_type, cardinal_t n_strx, integer_t n_other, integer_t n_value);
name_t search_name(expr_t ident);
void level_inc(boolean param_p);
void level_dec(void);
boolean parameter_level_p(integer_t level);
boolean local_level_p(integer_t level);
boolean global_level_p(integer_t level);
void regist_sameblocklink(name_t np);

name_t intern_name(expr_t ident);
name_t static_name(expr_t ident);
name_t declare_name(expr_t ident);
void name_init(void);
