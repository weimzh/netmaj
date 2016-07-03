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
 */

#ifndef _XNET_H
#define _XNET_H

#if __STDC__ || ANSIC
#define identcat(a,b)   a##b
#define identstr(a)     #a
#else
#define identcat(a,b)   a/**/b
#define identstr(a)     "a"
#endif

#include <assert.h>
#include "gt/widget/widget.h"

#define X_MIN X_CANCEL
#define X_CANCEL 20
#define X_PON 21
#define X_TIE 22
#define X_RON 23
#define X_KAN 24
#define X_REACH 26
#define X_TUMO 25

#define CH_NORM 0
#define CH_TIE  1
#define CH_KAN  2

struct view
{
  widget_t _title_frame;
  widget_t _user_kaze[4];
  widget_t _mark_frame[4];
  widget_t _user_name[4];
  widget_t _user_state[4];
  widget_t _user_ppoint[4];
  widget_t _user_gpoint[4];
  widget_t _user_chip[4];
  widget_t _user_chip2[4];
  widget_t _user_message[4];
  widget_t _hand_frame[4];
  widget_t _openhand_frame[4];
  widget_t _river_attr1s[4][4];
  widget_t _river_frames[4][4];
  widget_t _river_attr2[4];
  widget_t _river_attr3[4];
  widget_t _river_attr4[4];
  widget_t _reststr_frame;
  widget_t _rest_frame;
  widget_t _choice_frame;
  widget_t _choicestate_frame;
  widget_t _dorastr_frame, _uradorastr_frame;
  widget_t _action;
  widget_t _buttonaction;
  widget_t _kan_button, _tumo_button, _tie_button, _pon_button,
    _cancel_button, _ron_button, _reach_button;
  widget_t _state_frame, _dora_frame, _uradora_frame;
  widget_t _mt_reststr_frame;
  widget_t _mt_rest_frame;

  widget_t _pai_frame[4], _name_frame[4];
  widget_t _status_frame;
  widget_t _menu_frame;
  widget_t _one_button, _restart_button, _auto_button, _quit_button,
    _flush_button, _speed_button, _autoplay_button, _auto_after_reach_button,
    _visible_button, _about_button, _stop_button, _config_button;
  widget_t _advice_button, _sound_button, _rule_button;
  widget_t _hand_pai_advice_info;
  widget_t _advicepmframe;
  widget_t _linepos_frame[4];
};

#define title_frame ((view)->_title_frame)
#define user_kaze ((view)->_user_kaze)
#define mark_frame ((view)->_mark_frame)
#define user_name ((view)->_user_name)
#define user_state ((view)->_user_state)
#define user_ppoint ((view)->_user_ppoint)
#define user_gpoint ((view)->_user_gpoint)
#define user_chip ((view)->_user_chip)
#define user_chip2 ((view)->_user_chip2)
#define user_message ((view)->_user_message)
#define hand_frame ((view)->_hand_frame)
#define openhand_frame ((view)->_openhand_frame)
#define river_attr1 ((view)->_river_attr1s[0])
#define river_attr1s ((view)->_river_attr1s)
#define river_frame ((view)->_river_frames[0])
#define river_frames ((view)->_river_frames)
#define river_attr2 ((view)->_river_attr2)
#define river_attr3 ((view)->_river_attr3)
#define river_attr4 ((view)->_river_attr4)
#define reststr_frame ((view)->_reststr_frame)
#define rest_frame ((view)->_rest_frame)
#define choice_frame ((view)->_choice_frame)
#define choicestate_frame ((view)->_choicestate_frame)
#define dorastr_frame ((view)->_dorastr_frame)
#define uradorastr_frame ((view)->_uradorastr_frame)
#define kan_button ((view)->_kan_button)
#define tumo_button ((view)->_tumo_button)
#define advice_button ((view)->_advice_button)
#define tie_button ((view)->_tie_button)
#define pon_button ((view)->_pon_button)
#define cancel_button ((view)->_cancel_button)
#define ron_button ((view)->_ron_button)
#define reach_button ((view)->_reach_button)
#define state_frame ((view)->_state_frame)
#define dora_frame ((view)->_dora_frame)
#define uradora_frame ((view)->_uradora_frame)
#define action ((view)->_action)
#define buttonaction ((view)->_buttonaction)
#define mt_reststr_frame ((view)->_mt_reststr_frame)
#define mt_rest_frame ((view)->_mt_rest_frame)
#define pai_frame  ((view)->_pai_frame)
#define name_frame ((view)->_name_frame)
#define status_frame ((view)->_status_frame)
#define menu_frame ((view)->_menu_frame)
#define restart_button ((view)->_restart_button)
#define one_button  ((view)->_one_button)
#define auto_button  ((view)->_auto_button)
#define quit_button  ((view)->_quit_button)
#define flush_button  ((view)->_flush_button)
#define speed_button  ((view)->_speed_button)
#define rule_button  ((view)->_rule_button)
#define autoplay_button  ((view)->_autoplay_button)
#define auto_after_reach_button  ((view)->_auto_after_reach_button)
#define visible_button  ((view)->_visible_button)
#define about_button  ((view)->_about_button)
#define stop_button  ((view)->_stop_button)
#define sound_button  ((view)->_sound_button)
#define hand_pai_advice_info ((view)->_hand_pai_advice_info)
#define advicepmframe ((view)->_advicepmframe)
#define config_button ((view)->_config_button)
#define linepos_frame ((view)->_linepos_frame)

#include "flags.h"
#include "geom.h"
#include "util/util.h"
#include "decl.h"

#define callbackarg_param    \
  (widget_t w, integer_t val, struct callbackarg *arg)

#if X11
#define snd(x)
#define snd1(x)
#else
#define snd(x)               if (flag_soundp) { x ; }
#define snd1(x)               if (flag_soundp) { x ; xsleep(500); }
#endif

#define CAN_RON    (1<<0)
#define CAN_TUMO   (1<<1)
#define CAN_PON    (1<<2)
#define CAN_KAN    (1<<3)
#define CAN_TIE    (1<<4)
#define CAN_REACH  (1<<5)
#define CAN_CANCEL (1<<6)

#endif


#define STOPVALUE  (-1)
