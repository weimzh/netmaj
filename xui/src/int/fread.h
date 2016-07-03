/*  -*- Mode: C -*- 
 * This file is part of my project
 * 
 * fread.h -- 
 * 
 * $Id: fread.h,v 1.1 1996/06/21 04:22:39 suz Exp $
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue May  9 00:49:41 1995
 * Last Modified By: NBC02365@niftyserve.or.jp
 * Last Modified On: Tue May  9 00:49:41 1995
 * 
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

/* fread.c */
boolean         fileread(char *fname, FILE * fp, char **o_buf, _size_t * o_size);
