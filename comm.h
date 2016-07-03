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
 * 5/7/2006 - Modified by Wei Mingzhi <whistler@openoffice.org>
 */
/* message format */

#define PROTOCOL_VERSION	2
extern int protocol_version;

/*
 changed from ver 1 to ver 2
   :add MSG_OPT
   :chg MSG_HANDO		add 1 arg (from)
   :chg MSG_CONNECT_DONE	add 1 arg (version)
*/

#define PORT_NUMBER	4112
#define PROXY_PORT	4212
#define INPUT_PORT	4312

#define MSG_MAX		256

/* connecting message */
#define MSG_KILL		"K\n"
#define MSG_OPT			"O%c '%s\n"
#define MSG_CONNECT		"C (%s %o)\n"
#define MSG_CONNECT_DONE	"C=(%d) (%d) (%d)\n"
#define MSG_CONNECT_PLAYER	"PN= ('%s '%s '%s '%s )\n"

/* talk message */
#define MSG_COMMENTX	"c '"
#define MSG_COMMENT		"c '%s\n"       /* client -> server */
#define MSG_COMMENT2X		"c '%d>"
#define MSG_COMMENT2		"c '%d>%s\n"    /* server -> client */

/* ask message */
#define MSG_ASK		"A (%o %o)\n"
#define MSG_REPL	"R (%o %o)\n"

/* game status message */
#define MSG_GAME    "G=(('%s %d %o) ('%s %d %o) ('%s %d %o) ('%s %d %o) %d)\n"
#define MSG_GAMEEND "GE=((%d %d %d %d) (%d %d %d %d))\n"

#define MSG_PLAY    "P=(%d %d %d %d)\n"
#define MSG_PLAYEND "PE\n"

/* play message */
#define MSG_DORA    "D=(%o %o %o %o %o)\n"
#define MSG_URADORA "U=(%o %o %o %o %o)\n"
#define MSG_RIVER   "R[%d,%d]=(%o %o %o)\n"
#define MSG_RIVERIN "RI[%d]=(%o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o %o)\n"
#define MSG_HANDI   "HI[%d]=(%d (%o %o %o %o %o %o %o %o %o %o %o %o %o %o))\n"
#define MSG_HANDC   "HC[%d]=(%d (%o %o %o %o %o %o %o %o %o %o %o %o %o %o))\n"
#define MSG_HANDO   "HO[%d,%d]=(%o (%o %o %o %o) %o)\n"
#define MSG_RESULT  "RT[%d]=(%d (%d %d %d %d) (%d %d) (%d %d %d) (%o %o %o %o))\n"
#define MSG_POINT   "PO((%d %d %d %d) %d) (%d %d %d %d)\n"


/* for proxyclient */
#define MSG_HOST	"HP=('%s %d)\n"
#define MSG_AUTO0	"RA '"
#define MSG_AUTO	"RA '%s\n"
