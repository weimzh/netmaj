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

#ifndef _WIDGET_H
#define _WIDGET_H
#include <assert.h>
#include "gt/include/deftype.h"
#include "gt/dev/gdev.h"

#if __STDC__ || ANSIC
#define identcat(a,b)   a##b
#define identstr(a)     #a
#else
#define identcat(a,b)   a/**/b
#define identstr(a)     "a"
#endif

typedef struct widget *widget_t;
#include "gt/widget/itemtype.h"

typedef struct model
  {
    struct u
      {
	gc_t gc;
      }
    main;
    struct u sub;
    struct u light;
    struct u dark;
    byte_t relief;
  }
 *model_t;

struct widget
  {
#define _base_widget_body \
  item_type_t type;\
  gdev_t dev;\
  char *name;\
  integer_t left, top, width, height; \
  struct {byte_t fixed; integer_t left, top; } abs;\
  byte_t map; \
  byte_t invisible; \
  byte_t packed; \
  byte_t border_width; \
  byte_t wrap;\
  byte_t transparent;\
  void *callbackproc;\
  byte_t cannot_change_model; \
  struct model active;\
  struct model inactive;\
  struct widget * parent; \
  struct widget * child; \
  struct widget * next

    _base_widget_body;
  };

#include <stdarg.h>

#include "gt/widget/resource.h"
#include "gt/widget/event.h"
#include "gt/widget/funcdecl.h"
#include "gt/widget/widgetde.h"
#include "util/util.h"

#endif
