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
 * exprdef.h -- 
 * 
 * $Id: exprdef.h,v 1.2 1996/07/31 22:48:34 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:49:32 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Tue May  9 00:49:33 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

typedef struct expr {
  integer_t       kind;
  integer_t       id;
  union {
    integer_t       sym;
    integer_t       op;
    integer_t       ival;
    char           *strval;
    void           *operand[2];
  }               u;
}              *expr_t;

#define xid(p)    ((expr_t)(p))->id
#define xop(p)    ((expr_t)(p))->u.op
#define xint(p)   ((expr_t)(p))->u.ival
#define xstr(p)   ((expr_t)(p))->u.strval
#define xident(p) ((expr_t)(p))->u.strval
#define xcar(p)   ((expr_t)(p))->u.operand[0]
#define xcdr(p)   ((expr_t)(p))->u.operand[1]
#define car(p)   ((expr_t)(p))->u.operand[0]
#define cdr(p)   ((expr_t)(p))->u.operand[1]
#define xsym(p)   ((expr_t)(p))->u.sym

enum op_t {
  op_if,
  op_while,
  op_proc,
  op_lambda,
  op_break,
  op_continue,
  op_declare,
  op_define,
  op_block,
  op_prim,
  op_exit,
  op_begin,
  op_or,
  op_and,
  op_macro,
};

enum sym_t {
  sym_nil,
  sym_t,
  sym_f,
};

#define cell_p(p)  expr_p(p)
#define expr_p(p)  (xid(p) == id_expr)
#define integer_p(p)  (xid(p) == id_integer)


 enum {
  id_expr,
  id_ident,
  id_string,
  id_integer,
  id_sym,
  id_op,
};
