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
 * This file is part of my project
 *
 * rank.c -- 
 *
 * Author          : NBC02365@niftyserve.or.jp
 * Created On      : Thu Aug 17 23:07:49 1995
 *
 *  Copyright (C) 1995,1996 Todo Software All rights reserved
 */

#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"
#include "flags.h"

extern integer_t xwidth, xheight;

static widget_t configframe;
static widget_t soundonoff;
static widget_t pauseonoff;
static widget_t adviceonoff;
static widget_t advicealwaysonoff;
static widget_t advicedigitonoff;
static widget_t advicepaionoff;
static widget_t adviceselectonoff;
static widget_t originalonoff;
static widget_t talkonoff;
static widget_t cancelbuttononoff;
static widget_t lineposonoff;
static widget_t posmarkonoff;
static widget_t stringexeconoff;
static widget_t analizeonoff;
static widget_t redrawsfxonoff;
static widget_t requestonoff;

static widget_t talktickbutton;

static widget_t colortext;
static widget_t savedtext;

static widget_t soundonoff_text;
static widget_t pauseonoff_text;
static widget_t adviceonoff_text;
static widget_t advicealwaysonoff_text;
static widget_t advicedigitonoff_text;
static widget_t advicepaionoff_text;
static widget_t adviceselectonoff_text;
static widget_t originalonoff_text;
static widget_t cancelbuttononoff_text;
static widget_t lineposonoff_text;
static widget_t posmarkonoff_text;
static widget_t analizeonoff_text;
static widget_t redrawsfxonoff_text;
static widget_t requestonoff_text;
static widget_t talkonoff_text;
static widget_t stringexeconoff_text;
static widget_t talktick_text;
static widget_t boardcolor_text;

/* static void config_ok_callback (); */
static void config_save_callback ();
static void config_cancel_callback ();
static void talktick_callback ();

static void
talktick_config ()
{
  char buf[24];
  sprintf (buf, "%d", flag_talktick);
  widget_configure (talktickbutton, resource_text, buf);
}

static void
talktick_callback ()
{
  flag_talktick++;
  if (flag_talktick > 9)
    {
      flag_talktick = 1;
    }
  talktick_config ();
}

static void
config_conf ()
{
  widget_configure (soundonoff, resource_radiobutton_value, flag_soundp);
  widget_configure (pauseonoff, resource_radiobutton_value, flag_pause);
  widget_configure (adviceonoff, resource_radiobutton_value, flag_advice);
  widget_configure (advicealwaysonoff,
                    resource_radiobutton_value, flag_advicealways);
  widget_configure (advicedigitonoff,
                    resource_radiobutton_value, flag_advicedigit);
  widget_configure (advicepaionoff,
                    resource_radiobutton_value, flag_advicepai);
  widget_configure (adviceselectonoff,
                    resource_radiobutton_value, flag_adviceselect);
  widget_configure (originalonoff, resource_radiobutton_value, flag_original);
  widget_configure (cancelbuttononoff,
                    resource_radiobutton_value, flag_cancelbutton);
  widget_configure (lineposonoff, resource_radiobutton_value, flag_linepos);
  widget_configure (posmarkonoff, resource_radiobutton_value, flag_posmark);
  widget_configure (analizeonoff, resource_radiobutton_value, flag_analize);
  widget_configure (requestonoff, resource_radiobutton_value, flag_request);
  widget_configure (redrawsfxonoff,
                    resource_radiobutton_value, flag_redrawsfx);
  talktick_config ();

  if (XCLIENT)
    {
      widget_configure (stringexeconoff,
                        resource_radiobutton_value, flag_stringexec);
    }

  widget_configure (talkonoff, resource_radiobutton_value, flag_talk);

  widget_configure (colortext, resource_text, flag_basiccolorname);
}

#include "lang.h"

extern integer_t red, white, yellow;

static widget_t
create_config_frame (widget_t w)
{
  widget_t form, baseform, xform;
  widget_t actframe;
  widget_t header, text;

  baseform = widget_create ("frame", w);

  if (PAICARD3D || BUILD4)
    {
      header = text_create (baseform, xwidth, 54,
                            "    NETMAJ Configuration", red, XLARGETIMESIB,
                            1);
    }

  xform = widget_create ("frame", baseform);
  widget_configure (xform, resource_frame_start, 100);
  widget_configure (xform, resource_packed, resource_horizontal);
  xform = widget_create ("frame", xform);

#define radiobutton_init(var, string) \
  form = widget_create("frame", xform); \
  widget_configure(form, resource_packed, resource_horizontal); \
  var = widget_create ("radiobutton", form); \
  var##_text = text_create(form, xwidth - 4, 20, \
		     "", white, MEDIUMJP, 0); \
  lang_config(var##_text, string)

#define radiobutton_reinit(var, string) \
  lang_config(var##_text, string)

  radiobutton_init (soundonoff, STRING_SOUND);
  radiobutton_init (pauseonoff, STRING_PAUSE);
  radiobutton_init (adviceonoff, STRING_ADVICE);
  radiobutton_init (advicealwaysonoff, STRING_ADVICEALWAYS);
  radiobutton_init (advicedigitonoff, STRING_ADVICEDIGIT);
  radiobutton_init (advicepaionoff, STRING_ADVICEPAI);
  radiobutton_init (adviceselectonoff, STRING_ADVICESELECT);
  radiobutton_init (originalonoff, STRING_ORIGINAL);
  radiobutton_init (cancelbuttononoff, STRING_CANCELBUTTON);
  radiobutton_init (lineposonoff, STRING_LINEPOS);
  radiobutton_init (posmarkonoff, STRING_POSMARK);
  radiobutton_init (analizeonoff, STRING_ANALIZE);
  radiobutton_init (redrawsfxonoff, STRING_REDRAWSFX);
  radiobutton_init (requestonoff, STRING_REQUEST);
  radiobutton_init (talkonoff, STRING_TALK);
  if (XCLIENT)
    {
      radiobutton_init (stringexeconoff, STRING_STRINGEXEC);
    }
  form = widget_create ("frame", xform);
  widget_configure (form, resource_packed, resource_horizontal);
  talktickbutton = button_create (form, 20, 20, "", white, TIMESI,
                                  talktick_callback, "");
  talktick_text = text_create (form, xwidth - 4, 20, "", white, MEDIUMJP, 0);
  radiobutton_reinit (talktick, STRING_TALKTICK);

  form = widget_create ("frame", xform);
  widget_configure (form, resource_packed, resource_horizontal);
  text = boardcolor_text = text_create (form, 120, 20,
                                        "", white, MEDIUMJP, 0);
  radiobutton_reinit (boardcolor, STRING_BOARDCOLOR);

  widget_configure (text, resource_border_width, 2);
  widget_configure (text, resource_relief, relief_frame);
  colortext = text_create (form, xwidth - 120, 20, "", white, MEDIUMJP, 0);

  actframe = widget_create ("frame", baseform);
  widget_configure (actframe, resource_packed, resource_horizontal);
  widget_set_geometry (actframe, xwidth, BUTTON_H + 4);

  widget_configure (actframe, resource_frame_start, 270);

  configsavebutton = button_create (actframe,
                                    BUTTON_W, BUTTON_H,
                                    "set/save",
                                    yellow, TIMESI,
                                    (void *) config_save_callback, "");
  configbyebutton = button_create (actframe,
                                   BUTTON_W, BUTTON_H,
                                   "cancel",
                                   yellow, TIMESI,
                                   (void *) config_cancel_callback, "");
  savedtext = text_create (form, xwidth - 120, 20, "", white, MEDIUMJP, 0);

  widget_pack (baseform);
  widget_fix (baseform);
  widget_configure (baseform, resource_wrap, 0);
  widget_set_geometry (baseform, xwidth, xheight);

  configframe = baseform;
  return configframe;
};

void
reinit_config_frame ()
{
  radiobutton_reinit (soundonoff, STRING_SOUND);
  radiobutton_reinit (pauseonoff, STRING_PAUSE);
  radiobutton_reinit (adviceonoff, STRING_ADVICE);
  radiobutton_reinit (advicealwaysonoff, STRING_ADVICEALWAYS);
  radiobutton_reinit (advicedigitonoff, STRING_ADVICEDIGIT);
  radiobutton_reinit (advicepaionoff, STRING_ADVICEPAI);
  radiobutton_reinit (adviceselectonoff, STRING_ADVICESELECT);
  radiobutton_reinit (originalonoff, STRING_ORIGINAL);
  radiobutton_reinit (cancelbuttononoff, STRING_CANCELBUTTON);
  radiobutton_reinit (lineposonoff, STRING_LINEPOS);
  radiobutton_reinit (posmarkonoff, STRING_POSMARK);
  radiobutton_reinit (analizeonoff, STRING_ANALIZE);
  radiobutton_reinit (redrawsfxonoff, STRING_REDRAWSFX);
  radiobutton_reinit (requestonoff, STRING_REQUEST);
  radiobutton_reinit (talkonoff, STRING_TALK);
  radiobutton_reinit (stringexeconoff, STRING_STRINGEXEC);
  radiobutton_reinit (talktick, STRING_TALKTICK);
  radiobutton_reinit (boardcolor, STRING_BOARDCOLOR);
}

static widget_t from;
static integer_t flag_original_org;
static integer_t flag_request_org;
static integer_t saved;

void
config_callback callbackarg_param
{
  from = (void *) val;
  if (!configframe)
    {
      extern widget_t root_widget;
        create_config_frame (root_widget);
        lang_set ();
    }
  else
    {
      reinit_config_frame ();
    }

  flag_original_org = flag_original;
  flag_request_org = flag_request;
  saved = 0;

  config_conf ();
  widget_unmap (from);
  widget_map (configframe);
  widget_display (configframe);
  widget_flush (configframe);
}


extern widget_t about, board;


static void
config_save_callback callbackarg_param
{
  extern integer_t darkorange;

  flag_soundp = radiobutton_get_value (soundonoff);
  flag_pause = radiobutton_get_value (pauseonoff);
  flag_advice = radiobutton_get_value (adviceonoff);
  flag_advicealways = radiobutton_get_value (advicealwaysonoff);
  flag_advicedigit = radiobutton_get_value (advicedigitonoff);
  flag_advicepai = radiobutton_get_value (advicepaionoff);
  flag_adviceselect = radiobutton_get_value (adviceselectonoff);
  flag_original = radiobutton_get_value (originalonoff);
  flag_cancelbutton = radiobutton_get_value (cancelbuttononoff);
  flag_linepos = radiobutton_get_value (lineposonoff);
  flag_posmark = radiobutton_get_value (posmarkonoff);
  flag_analize = radiobutton_get_value (analizeonoff);
  flag_redrawsfx = radiobutton_get_value (redrawsfxonoff);
  flag_request = radiobutton_get_value (requestonoff);
  if (XCLIENT)
    {
      flag_stringexec = radiobutton_get_value (stringexeconoff);
    }
  flag_talk = radiobutton_get_value (talkonoff);

  save_xrcfile ();
  saved = 1;
  widget_configure (savedtext, resource_text,
                    "configuration saved to $(HOME)/.xnetmaj");
  widget_configure (savedtext, resource_textcolor, darkorange);
  widget_display (savedtext);
  widget_flush (savedtext);

  widget_configure (savedtext, resource_text, "");
  widget_unmap (configframe);

  if (saved)
    {
      if (flag_original_org != flag_original)
        lang_set ();
      if (flag_request_org != flag_request && flag_request == 0)
        {
          board_button_map (~0);
        }
    }
  if (from == board)
    {
      board_redraw (1);
    }
  else
    {
      about_callback (0, 0, 0);
    }
}

static void
config_cancel_callback callbackarg_param
{
  widget_configure (savedtext, resource_text, "");
  widget_unmap (configframe);

  if (from == board)
    {
      board_redraw (1);
    }
  else
    {
      about_callback (0, 0, 0);
    }
}
