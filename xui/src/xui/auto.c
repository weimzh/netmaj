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

#include "xnet.h"
#include "xnetmaj.h"

static struct person
{
  char *name;
}
persons[] =
{
  {"auto",},
  {".ksr",},
  {".fcc",},
  {".suz",},
  {".momokuri",},
  {".nomi",},
  {".yakuman",},
  {".taco",},
  {".naki",},
  {".monkey",},
  {".puyo",},
  {".puyo2",},
  {".lonleylily",},
};

static unsigned int auto_index;
static unsigned int name_size = (sizeof (persons) / sizeof (struct person));

void
one_callback callbackarg_param
{
  char cmd[256];
  char *netmaj_lib;

  netmaj_lib = getenv ("NETMAJ_LIB");

  if (!netmaj_lib)
    {
      netmaj_lib = NETMAJ_LIB;
    }
  sprintf (cmd, "%s/auto -n %s", netmaj_lib,
           persons[(auto_index++) % name_size].name);
  execute_auto (cmd);

  sprintf (cmd, "%s/auto -n %s", netmaj_lib,
           persons[(auto_index++) % name_size].name);
  execute_auto (cmd);

  sprintf (cmd, "%s/auto -n %s", netmaj_lib,
           persons[(auto_index++) % name_size].name);
  execute_auto (cmd);
}

void
auto_callback callbackarg_param
{
  extern int player_cnt;
  extern global_t *game_global;
  global_t *gp;

  gp = game_global;
  if (in_game != 0)
    {
      do_disconnect (gp);
    }
  else
    {
      char cmd[256];
      char *netmaj_lib;

      netmaj_lib = getenv ("NETMAJ_LIB");
      if (!netmaj_lib)
        {
          netmaj_lib = NETMAJ_LIB;
        }
      sprintf (cmd, "%s/auto -n %s", netmaj_lib,
               persons[(auto_index++) % name_size].name);
      execute_auto (cmd);
    }
}

#ifdef USE_RAND
#define random	rand
#define srandom srand
#endif

unsigned int
randval ()
{
  return (random ());
}

void
auto_init ()
{
  struct person temp;
  unsigned int xindex;
  unsigned int yindex;
  unsigned int i;

  auto_index = ((unsigned int) (randval ())) % (name_size);

  /* shake member */
  if (auto_index == 0)
    {
      auto_index = 2;
    }
  for (i = 0; i < name_size / auto_index; i++)
    {
      xindex = ((unsigned int) (randval ())) % name_size;
      yindex = ((unsigned int) (randval ())) % name_size;

      /* old C may not compile these lines */
      temp = persons[yindex];
      persons[yindex] = persons[xindex];
      persons[xindex] = temp;
    }
}

extern int white, black, red, yellow;

void
autoplay_config ()
{
  int color;
  extern int auto_play;
  color = auto_play ? red : white;
  if (autoplay_button)
    {
      widget_configure (autoplay_button, resource_textcolor, color);
      widget_display (autoplay_button);
      widget_flush (autoplay_button);
    }
}

void
autoplay_callback callbackarg_param
{
  extern int auto_play;
  auto_play = !auto_play;
  autoplay_config ();
}

void
auto_after_reach_config ()
{
  extern int auto_after_reach;
  int color;

  color = auto_after_reach ? white : yellow;
  if (auto_after_reach_button)
    {
      widget_configure (auto_after_reach_button, resource_textcolor, color);
      widget_display (auto_after_reach_button);
      widget_flush (auto_after_reach_button);
    }
}


void
auto_after_reach_callback callbackarg_param
{
  extern int auto_after_reach;
  auto_after_reach = !auto_after_reach;
  auto_after_reach_config ();
}
