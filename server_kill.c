/*
 * NETMAJ - network mahjongg -  Copyright Notice
 *     Copyright (C) 1994, 1995, 1996 Koji Suzuki (suz@kt.rim.or.jp)
 *
 *  You can redistribute it and/or modify this software under the terms 
 * of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 *  This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * 5-7-2006 Modified by Wei Mingzhi <whistler@openoffice.org>
 */

#include "global.h"

int port = PORT_NUMBER;

#define USAGE "\
usage:\n\
    server_kill [-p port-number]\n"

int
main (int argc, char *argv[])
{
  SOCKETIO *ssp;
  int c;

  while ((c = getopt (argc, argv, "p:")) != EOF)
    {
      switch (c)
        {
        case 'p':
          port = atoi (optarg);
          break;
        default:
          fprintf (stderr, USAGE);
          exit (1);
        }
    }

  ssp = ConnectSock ("localhost", port);
  if (ssp)
    {
      SockWrites (MSG_KILL, ssp);
      SockClose (ssp);
      sleep (1);
    }

  return 0;
}
