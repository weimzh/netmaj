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
 * declare.h -- 
 * 
 * $Id: declare.h,v 1.2 1996/07/31 22:48:29 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:48:11 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Fri Sep 29 19:51:09 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */
#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif

/* expr.c */
expr_t nil P_((void));
expr_t mk_char P_((char *s));
expr_t mk_integer P_((integer_t i));
expr_t mk_expr P_((expr_t a, expr_t b));
expr_t mk_string P_((char *s));
expr_t mk_ident P_((char *s));
expr_t mk_sym P_((integer_t s));
expr_t mk_op P_((integer_t op));
expr_t mk_name P_((char *s));
void expr_reset P_((void));
void expr_init P_((void));
pointer xreverse P_((pointer term, pointer list));
pointer xcons P_((pointer a, pointer b));
pointer xappend P_((pointer c, pointer d));
integer_t xlength P_((pointer p));
void expr_print P_((pointer p));

#undef P_
#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif

/* int.c */
boolean ident_p P_((expr_t e));
void assert_ident P_((expr_t e));
void assert_ident1 P_((expr_t e, char *s));
pointer interp_if P_((pointer p));
pointer interp_while P_((pointer p));
pointer interp_proc P_((pointer p));
pointer interp_lambda P_((pointer p));
pointer interp_break P_((pointer p));
pointer interp_continue P_((pointer p));
pointer interp_declare P_((pointer p));
pointer interp_define P_((pointer p));
pointer interp_block P_((pointer p));
pointer interp_prim P_((pointer p));
pointer interp_exit P_((pointer p));
pointer interp_begin P_((pointer p));
pointer interp_or P_((pointer p));
pointer interp_and P_((pointer p));
pointer interp_macro P_((pointer p));
expr_t interp P_((expr_t p));
boolean nu_source P_((char *fname));
void *name_value P_((char *name));
boolean name_exist P_((char *name));

#undef P_
#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif

/* interror.c */
void grammer P_((char *s));
void nu_error P_((char *msg, ...));
void nu_warning P_((char *msg, ...));
void nu_fatal P_((char *msg, ...));

#undef P_
#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif

/* intinit.c */
void nu_init P_((void));
void nu_finish P_((void));

#undef P_
#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif

/* name.c */
name_t search_name P_((expr_t ident));
name_t intern_name P_((expr_t ident));

#undef P_
#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif

/* numalloc.c */
char *nu_malloc P_((register unsigned int size));
char *nu_calloc P_((register unsigned int size));
char *nu_realloc P_((register char *p, register unsigned int size));
void nu_free P_((char *p));

#undef P_
#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif

/* readexpr.c */
void nu_set_input P_((FILE *fp));
pointer readexpr P_((void));
boolean nu_eof P_((pointer p));

#undef P_
#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif


#undef P_

