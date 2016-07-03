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
 * pixbdig.c -- 
 * 
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Tue Apr 11 11:15:22 1995
 *   digit like digital
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include <stdio.h>

#include "pixread.h"

#include "bitmap/misc/bdigit0.xpm"
#include "bitmap/misc/bdigit1.xpm"
#include "bitmap/misc/bdigit2.xpm"
#include "bitmap/misc/bdigit3.xpm"
#include "bitmap/misc/bdigit4.xpm"
#include "bitmap/misc/bdigit5.xpm"
#include "bitmap/misc/bdigit6.xpm"
#include "bitmap/misc/bdigit7.xpm"
#include "bitmap/misc/bdigit8.xpm"
#include "bitmap/misc/bdigit9.xpm"
#include "bitmap/misc/bdigitm.xpm"


static struct gpixmap2 pm_bdigit[11];

static char **bdigit_data[] = {
  fbitmap (bdigit0),
  fbitmap (bdigit1),
  fbitmap (bdigit2),
  fbitmap (bdigit3),
  fbitmap (bdigit4),
  fbitmap (bdigit5),
  fbitmap (bdigit6),
  fbitmap (bdigit7),
  fbitmap (bdigit8),
  fbitmap (bdigit9),
  fbitmap (bdigitm),
};

void
pmcontainer_set_bdigit (widget_t w, integer_t val)
{
  char buf[64];
  char *p;
  integer_t i;

  sprintf (buf, "%d", val);

  for (p = buf; *p; p++)
    {
      if (isdigit (*p))

        {
          i = *p - '0';
        digit:
          if (pm_bdigit[i].pm == 0)
            {
              read_pix_idx (bdigit_data[i], pm_bdigit[i]);
            }
          pmcontainer_print (w, pm_bdigit[i]);
        }
      else if (*p == '-')
        {
          i = 10;
          goto digit;
        }
      else
        {
          ;
        }
    }
}

void
read_bdigit_pixmap ()
{
  int i;
  for (i = 0; i < 11; i++)
    {
      read_pix_idx (bdigit_data[i], pm_bdigit[i]);
    }
}

#include "bitmap/misc/adigit0.xpm"
#include "bitmap/misc/adigit1.xpm"
#include "bitmap/misc/adigit2.xpm"
#include "bitmap/misc/adigit3.xpm"
#include "bitmap/misc/adigit4.xpm"
#include "bitmap/misc/adigit5.xpm"
#include "bitmap/misc/adigit6.xpm"
#include "bitmap/misc/adigit7.xpm"
#include "bitmap/misc/adigit8.xpm"
#include "bitmap/misc/adigit9.xpm"
#include "bitmap/misc/adigitm.xpm"

static struct gpixmap2 pm_adigit[11];

static char **adigit_data[] = {
  fbitmap (adigit0),
  fbitmap (adigit1),
  fbitmap (adigit2),
  fbitmap (adigit3),
  fbitmap (adigit4),
  fbitmap (adigit5),
  fbitmap (adigit6),
  fbitmap (adigit7),
  fbitmap (adigit8),
  fbitmap (adigit9),
  fbitmap (adigitm),
};

void
pmcontainer_set_adigit (widget_t w, integer_t val, integer_t flag)
{
  char buf[64];
  char *p;
  integer_t i;

  sprintf (buf, "%d", val);

  for (p = buf; *p; p++)
    {
      if (isdigit (*p))

        {
          i = *p - '0';
        digit:
          if (pm_adigit[i].pm == 0)
            {
              read_pix_idx (adigit_data[i], pm_adigit[i]);
            }
          _print_colorpixmap (w, pm_adigit[i]);
        }
      else if (*p == '-')
        {
          i = 10;
          goto digit;
        }
      else
        {
          ;
        }
    }
}

void
read_adigit_pixmap ()
{
  int i;
  for (i = 0; i < 11; i++)
    {
      read_pix_idx (adigit_data[i], pm_adigit[i]);
    }
}
