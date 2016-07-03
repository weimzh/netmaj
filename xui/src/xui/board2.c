/*  NETMAJ-XUI -- X User Interface --  Copyright Notice
 *
 *      Copyright (c) 1995, 1996 Todo Software All rights reserved.
 *      Copyright (C) 1994, 1995, 1996 Koji Suzuki (suz@kt.rim.or.jp)
 *
 *   You can redistribute it and/or modify this software under the terms
 *  of the GNU General Public License version 2 as published by
 *  the Free Software Foundation.
 *
 *   This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the author be held liable for any damages
 *  arising from the use of this software.
 */

#include "xnet.h"

static widget_t root;
widget_t board;
widget_t sub_board;
int in_result = 0;
static widget_t msg_frame[4], msg_bgframe[4];

integer_t xheight = 480;
integer_t xwidth = 640;
int board_type;
struct board_param board_param;

integer_t flag_enable_select = 0;
integer_t flag_enable_onboard = 0;
integer_t flag_effect = 0;
integer_t n_select;
integer_t choiced;

integer_t iam;

extern integer_t yellow, white, purple, blue;

static void
select_pai_callback callbackarg_param
{
  extern int hand_startpos;
  integer_t idx = val;
  integer_t x, y, n;
  if (!flag_enable_select)
    {
      return;
    }
  if (!(idx == 5 || idx == iam))
    {                           /* 5 means this func is called from choice */
      return;
    }
  if (idx != 5 && !flag_enable_onboard)
    {
      return;
    }
  if (BUILD4)
    {
      if (w == hand_frame[3])
        {
          x = (arg->x - hand_startpos); /* get clicked pos */
        }
      else
        {
          x = (arg->x);         /* get clicked pos */
        }
    }
  else
    {
      x = (arg->x - HANDPAISTART);
    }
  y = arg->y;
  n = x / PAI_W;
  if (n_select != 0 && n == n_select)
    {
      n = (x - HANDPAIMARGIN) / PAI_W;  /* choice widen */
    }
  if (!(0 <= n && n < n_select))
    {
      return;                   /* do nothing */
    }
  snd (audioplay_pai ());

  flag_enable_select = 0;
  flag_enable_onboard = 0;
  choiced = n;
  flag_effect = 1;
}

void
board_button3press_callback ()
{
  if (!widget_map_p (board))
    {
      return;
    }
  if (flag_enable_select && flag_enable_onboard)
    {
/* tumogiri: call from choice (mode, not res mode) */
/* n_select is n pais; tumo is always (n_select - 1) */
      choiced = n_select - 1;
      snd (audioplay_tumogiri ());
    }
  else
    {
      choiced = X_CANCEL;
      snd (audioplay_cancel ());
    }
  flag_enable_select = 0;
  flag_enable_onboard = 0;
  flag_effect = 1;
}

void
cancel_button_callback callbackarg_param
{
  if (in_result)
    {
      keywait_callback ();
    }
  else
    {
      keywait_callback ();
      flag_enable_select = 0;
      flag_effect = 1;
      choiced = X_CANCEL;
      snd (audioplay_cancel ());
    }
}

static void
tie_button_callback callbackarg_param
{
  flag_effect = 1;
  choiced = X_TIE;
  snd (audioplay_tie ());
}

static void
pon_button_callback callbackarg_param
{
  flag_effect = 1;
  choiced = X_PON;
  snd (audioplay_pon ());
}

static void
kan_button_callback callbackarg_param
{
  flag_effect = 1;
  choiced = X_KAN;
  snd (audioplay_kan ());
}

static void
ron_button_callback callbackarg_param
{
  flag_effect = 1;
  choiced = X_RON;
  snd (audioplay_ron ());
}

static void
tumo_button_callback callbackarg_param
{
  flag_effect = 1;
  choiced = X_TUMO;
  snd (audioplay_tumo ());
}

static void
reach_button_callback callbackarg_param
{
  flag_effect = 1;
  choiced = X_REACH;
  snd (audioplay_reach ());
}

extern void restart_callback callbackarg_param;
extern void flush_callback callbackarg_param;
extern void speed_callback callbackarg_param;
extern void one_callback callbackarg_param;
extern void auto_callback callbackarg_param;
extern void autoplay_callback callbackarg_param;
extern void auto_after_reach_callback callbackarg_param;
extern void visible_callback callbackarg_param;
extern void about_callback callbackarg_param;
extern void advice_button_callback callbackarg_param;
extern void stop_button_callback callbackarg_param;
extern void sound_button_callback callbackarg_param;
extern void config_callback callbackarg_param;
extern void rule_callback callbackarg_param;

void
quit_callback callbackarg_param
{
  extern global_t *game_global;
  do_disconnect (game_global);
  term ();
}

void
check_callback (global_t *gp)
{
  extern int readable_socket;
  int fd, fd2;
  if (!gp)
    return;
  fd = callback_fd (gp);
  fd2 = callback_fd2 (gp);
  if (readable_socket)
    {
      callback (gp);
    }
  else
    {
      ysleep (1);
    }
}

void
get_choice (global_t *gp)
{
  extern widget_t root_widget;
  flag_effect = 0;
  widget_event_discard (root_widget);
  for (; flag_effect == 0;)
    {
      if (XCLIENT)
        {
          if (widget_eventsqueued (root_widget))
            {
              _widget_handle_event ();
              invoke_handlers ();
            }
          else
            {
              check_callback (gp);
            }
        }
      else
        {
          if (widget_eventsqueued (root_widget))
            {
              _widget_handle_event ();
              invoke_handlers ();
            }
          else
            {
              ysleep (1);
            }
        }
    }
}

void
board_sync ()
{
  widget_sync (board);
}

void
board_flush_sfx ()
{
  static int i = 0;
  if (i++ % 2)
    {
      widget_flush_sfx_blind (board, 20, 20);
    }
  else
    {
      widget_flush_sfx_shatter (board, 10, 10);
    }
  widget_sync (board);
}


void
board_flush ()
{
  widget_flush (board);
}

void
board_button_flush ()
{
  if (!BUILD4)
    widget_flush (action);
  else
    {
      widget_flush (buttonaction);
      widget_flush (action);
      widget_flush (choicestate_frame);
    }
}

void
board_display ()
{
  widget_display (board);
}

extern widget_t root_widget;

int
build_status (widget_t status1)
{
  int n = 0;

  config_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                                 "config", white, TIMESI,
                                 config_callback, board);
  n++;
  rule_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                               "rule", white, TIMESI, rule_callback, board);
  n++;
#if 0                           /* cut flush */
  flush_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                                "flush", white, TIMESI, flush_callback, "");
  n++;
#endif
  visible_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                                  "visible", white, TIMESI,
                                  visible_callback, "");
  n++;
  speed_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                                "speed", white, TIMESI, speed_callback, "");
  n++;
  if (XCLIENT)
    {
      autoplay_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                                       "autoplay", white, TIMESI,
                                       autoplay_callback, "");
      n++;
      auto_after_reach_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                                               "auto(R)", white, TIMESI,
                                               auto_after_reach_callback, "");
      n++;
      auto_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                                   "run auto", white, TIMESI,
                                   auto_callback, "");
      n++;
    }
  about_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                                "about", white, TIMESI, about_callback, "");
  n++;
  quit_button = button_create (status1, MBUTTON_W, MBUTTON_H,
                               "quit", white, TIMESI, quit_callback, "");
  n++;

  return n;
}

extern integer_t tann, darkgreen, seagreen, darkorange;

void
make_board1 ()
{
  int i;
  widget_t root, menu, frame, status1, status2;
  widget_t dummy;
  widget_t plane[4];

  colorcombo_init ();           /* for board color change */

  /* main board */
  root = board = widget_create ("frame", root_widget);
  widget_set_geometry (board, xwidth, xheight);
  widget_configure (board, resource_name, "board");

  /* title and menu buttons */
  menu_frame = status1 = widget_create ("frame", root);
  widget_configure (status1, resource_packed, resource_horizontal);
  title_frame = widget_create ("text", status1);

  widget_configure (title_frame, resource_textfont, LARGETIMESIB);
  widget_configure (title_frame, resource_text, "netmaj");
  widget_configure (title_frame, resource_textcolor, darkorange);
  widget_configure (title_frame, resource_textshadow, 1);

  i = build_status (status1);
  widget_set_geometry (title_frame, xwidth - MBUTTON_W * i - 4, MBUTTON_H);
  speed_config ();

  /* case, dora and uradora frames */
  status_frame = status2 = widget_create ("frame", root);
  widget_configure (status1, resource_wrap, 0);
  widget_configure (status1, resource_packed, resource_horizontal);
  widget_configure (status2, resource_wrap, 0);
  widget_configure (status2, resource_packed, resource_horizontal);
  state_frame = widget_create ("pmcontainer", status2);
  reststr_frame = widget_create ("pmcontainer", status2);
  rest_frame = widget_create ("text", status2);
  widget_configure (rest_frame, resource_textfont, TIMESI);
  mt_reststr_frame = widget_create ("pmcontainer", status2);
  mt_rest_frame = widget_create ("text", status2);
  widget_configure (mt_rest_frame, resource_textfont, HELVB);
  dorastr_frame = widget_create ("pmcontainer", status2);
  dora_frame = widget_create ("pmcontainer", status2);
  uradorastr_frame = widget_create ("pmcontainer", status2);
  uradora_frame = widget_create ("pmcontainer", status2);

  widget_set_geometry (status1, xwidth, TITLE_H + 4);
  widget_configure (status1, resource_border_width, 2);
  widget_set_geometry (status2, xwidth, STATE_H + 4);
  widget_configure (status2, resource_border_width, 2);

  widget_set_geometry (state_frame, 140, STATE_H);
  widget_set_geometry (reststr_frame, STATE_W, STATE_H);
  widget_set_geometry (rest_frame, 30, STATE_H);
  widget_set_geometry (mt_reststr_frame, STATE_W, STATE_H);
  widget_set_geometry (mt_rest_frame, 30, STATE_H);
  widget_set_geometry (dorastr_frame, STATE_W, STATE_H);
  widget_set_geometry (dora_frame, PAI_W * 5, STATE_H);
  widget_set_geometry (uradorastr_frame, STATE_W, STATE_H);
  widget_set_geometry (uradora_frame, PAI_W * 5, STATE_H);

  widget_configure (rest_frame, resource_textcolor, yellow);
  widget_configure (rest_frame, resource_textshadow, 1);

  /* user frame */
  for (i = 0; i < 4; i++)
    {
      integer_t pai_width;
      widget_t w, w2;
      plane[i] = widget_create ("frame", root);
      widget_configure (plane[i], resource_packed, resource_horizontal);

      name_frame[i] = widget_create ("frame", plane[i]);
      widget_configure (name_frame[i], resource_border_width, 2);

      w = widget_create ("frame", name_frame[i]);
      widget_configure (w, resource_packed, resource_horizontal);
      user_kaze[i] = widget_create ("pmcontainer", w);
      w2 = widget_create ("frame", w);
      user_state[i] = widget_create ("pmcontainer", w2);
      mark_frame[i] = widget_create ("pmcontainer", w2);
      user_name[i] = widget_create ("text", name_frame[i]);

      w = widget_create ("frame", name_frame[i]);
      widget_configure (w, resource_packed, resource_horizontal);
      user_gpoint[i] = widget_create ("text", w);
      user_ppoint[i] = widget_create ("text", w);
      user_chip[i] = widget_create ("pmcontainer", name_frame[i]);
      if (!PAICARD || BUILD4)
        {
          user_chip2[i] = widget_create ("pmcontainer", name_frame[i]);
        }

      pai_width = xwidth - name_width;

      widget_configure (user_name[i], resource_textcolor, yellow);
      widget_configure (user_name[i], resource_textfont, MEDIUMJP);
      widget_configure (user_name[i], resource_textshadow, 1);
      widget_configure (user_gpoint[i], resource_textcolor, yellow);
      widget_configure (user_ppoint[i], resource_textcolor, white);
      widget_configure (user_ppoint[i], resource_textshadow, 1);
      widget_configure (user_gpoint[i], resource_textshadow, 1);
      widget_configure (user_ppoint[i], resource_textfont, TIMESI);
      widget_configure (user_gpoint[i], resource_textfont, TIMESI);

      pai_frame[i] = widget_create ("frame", plane[i]);
      widget_configure (pai_frame[i], resource_border_width, 2);

      {
        linepos_frame[i] = widget_create ("frame", pai_frame[i]);
        widget_set_geometry (linepos_frame[i], pai_width - 4, 1);
        widget_configure (linepos_frame[i], resource_wrap, 0);
      }
      river_attr1[i] = widget_create ("pmcontainer", pai_frame[i]);
      river_frame[i] = widget_create ("pmcontainer4", pai_frame[i]);
      river_attr2[i] = widget_create ("pmcontainer", pai_frame[i]);
      river_attr3[i] = widget_create ("pmcontainer", pai_frame[i]);
      river_attr4[i] = widget_create ("pmcontainer", pai_frame[i]);
      hand_frame[i] = widget_create ("pmcontainer4", pai_frame[i]);

      widget_set_geometry (river_attr1[i], pai_width - 4, ATTR_H);
      widget_set_geometry (river_frame[i], pai_width - 4, PAI_H);
      widget_set_geometry (river_attr2[i], pai_width - 4, ATTR_H);
      widget_set_geometry (river_attr3[i], pai_width - 4, ATTR_H);
      widget_set_geometry (river_attr4[i], pai_width - 4, ATTR_H);
      widget_set_geometry (hand_frame[i], pai_width - 4, PAI_H);

      widget_configure (hand_frame[i], resource_pixmapstart, HANDPAISTART);
      widget_configure (hand_frame[i], resource_direction, 0);
      widget_configure (river_frame[i], resource_direction, 0);
      if (i == 3)
        {
          hand_pai_advice_info = widget_create ("pmcontainer", pai_frame[i]);
          widget_configure (hand_pai_advice_info, resource_pixmapstart,
                            HANDPAISTART);
          widget_set_geometry (hand_pai_advice_info, pai_width - 4, 8);
        }
      widget_add_callback (hand_frame[i],
                           resource_buttonpress_callback,
                           (void *) select_pai_callback, i);
      widget_pack (pai_frame[i]);
      widget_configure (pai_frame[i], resource_wrap, 0);

      {
        integer_t w, h;
        widget_get_geometry (pai_frame[i], &w, &h);

        widget_set_geometry (user_name[i], name_width - 4, 18);
        widget_set_geometry (user_kaze[i], name_width - 28, 28);
        widget_set_geometry (mark_frame[i], 24, 14);
        widget_set_geometry (user_state[i], 24, 14);

        widget_set_geometry (user_gpoint[i], (name_width - 4) / 2, DIGIT_H);
        widget_set_geometry (user_ppoint[i], (name_width - 4) / 2, DIGIT_H);
        widget_set_geometry (user_chip[i], name_width - 4, 14);
        if (!PAICARD || BUILD4)
          {
            widget_set_geometry (user_chip2[i], name_width - 4, 14);
          }

        widget_pack (name_frame[i]);    /* calculation of sizeof name_frame  */
        widget_configure (name_frame[i], resource_wrap, 0);
        widget_set_geometry (name_frame[i], name_width, h);
      }
    }

  /* action frame */
  {
    widget_t paiselectaction;
    integer_t i = 0;
    action = widget_create ("frame", root);
    paiselectaction = widget_create ("frame", action);
    widget_configure (paiselectaction, resource_packed, resource_horizontal);

    choicestate_frame = widget_create ("pmcontainer", paiselectaction);
    widget_set_geometry (choicestate_frame, PAI_W * 3, PAI_H + 4);
    i += (PAI_W * 3);
    choice_frame = widget_create ("pmcontainer", paiselectaction);
    widget_set_geometry (choice_frame, PAI_W * 10, PAI_H + 4);
    i += (PAI_W * 10);
    advicepmframe = widget_create ("pmcontainer", paiselectaction);
    widget_set_geometry (advicepmframe, xwidth - i, PAI_H + 4);
  }
  {
    buttonaction = widget_create ("frame", action);
    widget_configure (buttonaction, resource_packed, resource_horizontal);
    cancel_button = widget_create ("button", buttonaction);
    advice_button = widget_create ("button", buttonaction);
    tie_button = widget_create ("button", buttonaction);
    pon_button = widget_create ("button", buttonaction);
    kan_button = widget_create ("button", buttonaction);
    reach_button = widget_create ("button", buttonaction);
    tumo_button = widget_create ("button", buttonaction);
    ron_button = widget_create ("button", buttonaction);
    actionbutton_setup ();

/*
    widget_configure (cancel_button, resource_text, "cancel");
    widget_configure (kan_button, resource_text, "kan");
    widget_configure (tumo_button, resource_text, "tumo");
    widget_configure (tie_button, resource_text, "tie");
    widget_configure (pon_button, resource_text, "pon");
    widget_configure (ron_button, resource_text, "ron");
    widget_configure (reach_button, resource_text, "reach");
*/

    widget_set_geometry (advice_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (cancel_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (kan_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (tumo_button, BUTTON_W, BUTTON_H);
    widget_set_geometry (tie_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (pon_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (reach_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (ron_button, BUTTON_W, BUTTON_H);
    widget_pack (buttonaction);
    widget_pack (action);
  }

  widget_add_callback (advice_button,
                       resource_buttonpress_callback,
                       (void *) advice_button_callback, "advice");
  widget_add_callback (cancel_button,
                       resource_buttonpress_callback,
                       (void *) cancel_button_callback, "cancel");
  widget_add_callback (kan_button,
                       resource_buttonpress_callback,
                       (void *) kan_button_callback, "kan");
  widget_add_callback (tumo_button, resource_buttonpress_callback,
                       (void *) tumo_button_callback, "tumo");
  widget_add_callback (tie_button, resource_buttonpress_callback,
                       (void *) tie_button_callback, "tie");
  widget_add_callback (pon_button, resource_buttonpress_callback,
                       (void *) pon_button_callback, "pon");
  widget_add_callback (reach_button, resource_buttonpress_callback,
                       (void *) reach_button_callback, "reach");
  widget_add_callback (ron_button, resource_buttonpress_callback,
                       (void *) ron_button_callback, "ron");
  widget_add_callback (choice_frame,
                       resource_buttonpress_callback,
                       (void *) select_pai_callback, 5);

  if (COMMUNICATION)
    {                           /* communication */
      widget_t comm = widget_create ("frame", root);
      widget_t strbox[3];
      widget_t strs[15];
      int i, j, k = 0;
      extern char talk_messages[15][60];
      void send_play_message_callback ();

      widget_configure (comm, resource_packed, resource_horizontal);
      widget_set_geometry (comm, xwidth, 5 * JPFONTHEIGHT);
      strbox[0] = widget_create ("frame", comm);
      widget_set_geometry (strbox[0], xwidth / 3, 5 * JPFONTHEIGHT);
      strbox[1] = widget_create ("frame", comm);
      widget_set_geometry (strbox[1], xwidth / 3, 5 * JPFONTHEIGHT);
      strbox[2] = widget_create ("frame", comm);
      widget_set_geometry (strbox[2], xwidth / 3, 5 * JPFONTHEIGHT);

      for (i = 0; i < 3; i++)
        {
          widget_t f = widget_create ("frame", strbox[i]);
          widget_set_geometry (f, 8, JPFONTHEIGHT + 2);
          widget_configure (f, resource_wrap, 0);
          for (j = 0; j < 5; j++)
            {
              strs[k] = widget_create ("text", strbox[i]);
              widget_set_geometry (strs[k], xwidth / 3 - 8, JPFONTHEIGHT + 2);
              widget_configure (strs[k], resource_text, &talk_messages[k]);
              widget_configure (strs[k], resource_textfont, MEDIUMJP);
              widget_add_callback (strs[k], resource_buttonpress_callback,
                                   send_play_message_callback, k);
              k++;
            }
          widget_pack (strbox[i]);
        }
      widget_pack (comm);
    }

  widget_pack (root);
  widget_fix (root);
  message_init ((widget_t *) & pai_frame, (widget_t *) & hand_frame);

  bind_save (resource_button3press_callback,
             (void *) board_button3press_callback, 0);

  widget_set_geometry (root, xwidth, xheight);
}

void
make_board2 ()
{
  int i, j;
  widget_t root, menu, frame, status1, status2;
  widget_t base, base_l, base_r, work, work2;
  widget_t dummy;
  widget_t plane[4];
  int size, hsize;

  colorcombo_init ();           /* for board color change */

  /* main board */
  root = board = widget_create ("frame", root_widget);
  widget_set_geometry (board, xwidth, xheight);
  widget_configure (board, resource_name, "board");

  /* title and menu buttons */
  menu_frame = status1 = widget_create ("frame", root);
  widget_configure (status1, resource_packed, resource_horizontal);
  title_frame = widget_create ("text", status1);
  widget_configure (title_frame, resource_text, "netmaj");
  widget_configure (title_frame, resource_textcolor, darkorange);
  widget_configure (title_frame, resource_textfont, 6);
  widget_configure (title_frame, resource_textshadow, 1);

  i = build_status (status1);
  widget_set_geometry (title_frame, xwidth - MBUTTON_W * i - 4, MBUTTON_H);

  if (PAICARD3D)
    {
      size = PAI_W * 23;
    }
  else
    {
      size = PAI_W * 20;
    }
  hsize = size / 2;
  base = widget_create ("frame", root);
  widget_configure (base, resource_packed, resource_horizontal);
  base_r = widget_create ("frame", base);
  base_l = widget_create ("frame", base);
  widget_configure (base_r, resource_border_width, 2);
  widget_configure (base_r, resource_wrap, 0);
  widget_configure (base_r, resource_packed, 0);        /* self locating */
  widget_set_geometry (base_r, size, size);


  /* case, dora and uradora frames */
  status_frame = status2 = widget_create ("frame", base_r);
  widget_configure (status2, resource_packed, resource_virtical);
  widget_set_geometry (status2, PAI_W * 6 + 8, PAI_W * 6 + 8);
  widget_set_location (status2, hsize - PAI_W * 3 - 4, hsize - PAI_W * 3 - 4);
  widget_configure (status2, resource_wrap, 0);
  widget_configure (status2, resource_border_width, 2);

  widget_configure (status1, resource_wrap, 0);
  widget_configure (status1, resource_packed, resource_horizontal);
  widget_configure (status1, resource_border_width, 2);

  state_frame = widget_create ("pmcontainer", status2);

  work = widget_create ("frame", status2);
  widget_configure (work, resource_packed, resource_horizontal);
  reststr_frame = widget_create ("pmcontainer", work);
  rest_frame = widget_create ("text", work);
  widget_configure (rest_frame, resource_textfont, TIMESI);
  mt_reststr_frame = widget_create ("pmcontainer", work);
  mt_rest_frame = widget_create ("text", work);
  widget_configure (mt_rest_frame, resource_textfont, HELVB);

  work = widget_create ("frame", status2);
  widget_configure (work, resource_packed, resource_horizontal);
  dorastr_frame = widget_create ("pmcontainer", work);
  dora_frame = widget_create ("pmcontainer", work);

  work = widget_create ("frame", status2);
  widget_configure (work, resource_packed, resource_horizontal);
  uradorastr_frame = widget_create ("pmcontainer", work);
  uradora_frame = widget_create ("pmcontainer", work);

  widget_set_geometry (status1, xwidth, TITLE_H + 4);

  widget_set_geometry (state_frame, 140, STATE_H);
  widget_set_geometry (reststr_frame, STATE_W, STATE_H);
  widget_set_geometry (rest_frame, 30, STATE_H);
  widget_set_geometry (mt_reststr_frame, STATE_W, STATE_H);
  widget_set_geometry (mt_rest_frame, 30, STATE_H);
  widget_set_geometry (dorastr_frame, STATE_W, STATE_H);
  widget_set_geometry (dora_frame, PAI_W * 5, STATE_H);
  widget_set_geometry (uradorastr_frame, STATE_W, STATE_H);
  widget_set_geometry (uradora_frame, PAI_W * 5, STATE_H);

  widget_configure (rest_frame, resource_textcolor, yellow);
  widget_configure (rest_frame, resource_textshadow, 1);
  widget_pack (status2);

  work = widget_create ("frame", base_l);
  widget_configure (work, resource_packed, resource_horizontal);
  work2 = widget_create ("pmcontainer", work);
  widget_set_geometry (work2, name_width / 2, NAME_H);
  name_frame[1] = widget_create ("frame", work);

  work = widget_create ("frame", base_l);
  widget_configure (work, resource_packed, resource_horizontal);
  name_frame[2] = widget_create ("frame", work);
  name_frame[0] = widget_create ("frame", work);
  work = widget_create ("frame", base_l);
  widget_configure (work, resource_packed, resource_horizontal);
  work2 = widget_create ("pmcontainer", work);
  widget_set_geometry (work2, name_width / 2, NAME_H);
  name_frame[3] = widget_create ("frame", work);


  for (i = 0; i < 4; i++)
    {
      widget_t w, w2;

      widget_configure (name_frame[i], resource_border_width, 2);

      w = widget_create ("frame", name_frame[i]);
      widget_configure (w, resource_packed, resource_horizontal);
      user_kaze[i] = widget_create ("pmcontainer", w);
      w2 = widget_create ("frame", w);
      user_state[i] = widget_create ("pmcontainer", w2);
      mark_frame[i] = widget_create ("pmcontainer", w2);
      user_name[i] = widget_create ("text", name_frame[i]);

      w = widget_create ("frame", name_frame[i]);
      widget_configure (w, resource_packed, resource_horizontal);
      user_gpoint[i] = widget_create ("text", w);
      user_ppoint[i] = widget_create ("text", w);

      user_chip[i] = widget_create ("pmcontainer", name_frame[i]);
      if (!PAICARD || BUILD4)
        {
          user_chip2[i] = widget_create ("pmcontainer", name_frame[i]);
        }

      widget_configure (user_name[i], resource_textcolor, yellow);
      widget_configure (user_name[i], resource_textfont, MEDIUMJP);
      widget_configure (user_name[i], resource_textshadow, 1);
      widget_configure (user_gpoint[i], resource_textcolor, yellow);
      widget_configure (user_ppoint[i], resource_textcolor, white);
      widget_configure (user_ppoint[i], resource_textshadow, 1);
      widget_configure (user_gpoint[i], resource_textshadow, 1);
      widget_configure (user_ppoint[i], resource_textfont, TIMESI);
      widget_configure (user_gpoint[i], resource_textfont, TIMESI);
      {
        widget_set_geometry (user_name[i], name_width - 4, 18);
        widget_set_geometry (user_kaze[i], name_width - 28, 28);
        widget_set_geometry (mark_frame[i], 24, 14);
        widget_set_geometry (user_state[i], 24, 14);

        widget_set_geometry (user_gpoint[i], (name_width - 4) / 2, DIGIT_H);
        widget_set_geometry (user_ppoint[i], (name_width - 4) / 2, DIGIT_H);
        widget_set_geometry (user_chip[i], name_width - 4, 14);
        if (!PAICARD || BUILD4)
          {
            widget_set_geometry (user_chip2[i], name_width - 4, 14);
          }

        widget_pack (name_frame[i]);    /* calculation of sizeof name_frame  */
        widget_configure (name_frame[i], resource_wrap, 0);
        /*widget_set_geometry (name_frame[i], name_width, h); */
      }
    }


  /* user frame */
  for (i = 0; i < 4; i++)
    {
      pai_frame[i] = widget_create ("frame", base_r);
      if (i & 1)
        {
          widget_configure (pai_frame[i], resource_packed, resource_virtical);
          if (i & 2)
            {
              linepos_frame[i] = widget_create ("frame", pai_frame[i]);
              for (j = 0; j < 4; j++)
                {
                  river_frames[i][j] =
                    widget_create ("pmcontainer4", pai_frame[i]);
                  river_attr1s[i][j] =
                    widget_create ("pmcontainer4", pai_frame[i]);
                }
            }
          else
            {
              for (j = 3; j >= 0; j--)
                {
                  river_attr1s[i][j] =
                    widget_create ("pmcontainer4", pai_frame[i]);
                  river_frames[i][j] =
                    widget_create ("pmcontainer4", pai_frame[i]);
                }
              linepos_frame[i] = widget_create ("frame", pai_frame[i]);
            }
          widget_set_geometry (linepos_frame[i], PAI_W * 7, 1);
          for (j = 0; j < 4; j++)
            {
              widget_set_geometry (river_attr1s[i][j], PAI_W * 7, 2);
              widget_set_geometry (river_frames[i][j], PAI_W * 7, PAI_H);
              widget_configure (river_frames[i][j],
                                resource_packdirect, resource_horizontal);
              widget_configure (river_attr1s[i][j],
                                resource_packdirect, resource_horizontal);
            }
        }
      else
        {
          widget_configure (pai_frame[i], resource_packed,
                            resource_horizontal);
          if (i & 2)
            {
              for (j = 3; j >= 0; j--)
                {
                  river_attr1s[i][j] =
                    widget_create ("pmcontainer4", pai_frame[i]);
                  river_frames[i][j] =
                    widget_create ("pmcontainer4", pai_frame[i]);
                }
              linepos_frame[i] = widget_create ("frame", pai_frame[i]);
            }
          else
            {
              linepos_frame[i] = widget_create ("frame", pai_frame[i]);
              for (j = 0; j < 4; j++)
                {
                  river_frames[i][j] =
                    widget_create ("pmcontainer4", pai_frame[i]);
                  river_attr1s[i][j] =
                    widget_create ("pmcontainer4", pai_frame[i]);
                }
            }
          widget_set_geometry (linepos_frame[i], 1, PAI_W * 7);
          for (j = 0; j < 4; j++)
            {
              widget_set_geometry (river_attr1s[i][j], 2, PAI_W * 7);
              widget_set_geometry (river_frames[i][j], PAI_H, PAI_W * 7);
              widget_configure (river_frames[i][j],
                                resource_packdirect, resource_virtical);
              widget_configure (river_attr1s[i][j],
                                resource_packdirect, resource_virtical);
            }
        }
      hand_frame[i] = widget_create ("pmcontainer4", base_r);
      widget_configure (linepos_frame[i], resource_wrap, 0);

      if (i & 1)
        {
          widget_set_geometry (hand_frame[i], size - PAI_H - 4, PAI_H);
          widget_configure (hand_frame[i],
                            resource_packdirect, resource_horizontal);
        }
      else
        {
          widget_configure (hand_frame[i],
                            resource_packdirect, resource_virtical);
          widget_set_geometry (hand_frame[i], PAI_H, size - PAI_H - 4);
        }
      for (j = 0; j < 4; j++)
        {
          widget_configure (river_frames[i][j], resource_direction,
                            (i + 1) % 4);
          widget_configure (river_attr1s[i][j], resource_direction,
                            (i + 1) % 4);
        }
      widget_configure (hand_frame[i], resource_direction, (i + 1) % 4);

      widget_pack (pai_frame[i]);
      widget_configure (pai_frame[i], resource_wrap, 0);


      widget_add_callback (hand_frame[i],
                           resource_buttonpress_callback,
                           (void *) select_pai_callback, i);
    }
  {
    hand_pai_advice_info = widget_create ("pmcontainer", base_r);
#if 1
    widget_configure (hand_pai_advice_info, resource_invisible, 1);
#else
    widget_configure (hand_pai_advice_info, resource_pixmapstart,
                      HANDPAISTART);
    widget_set_geometry (hand_pai_advice_info, pai_width - 4, 8);
#endif
  }
  widget_set_location (hand_frame[0], size - PAI_H - 2, 2);
  widget_set_location (hand_frame[1], 2, 2);
  widget_set_location (hand_frame[2], 2, PAI_H + 2);
  widget_set_location (hand_frame[3], PAI_H + 2, size - PAI_H - 2);

  widget_set_location (pai_frame[0], hsize + PAI_W * 3 + 8,
                       hsize - PAI_W * 4);
  widget_set_location (pai_frame[1], hsize - PAI_W * 4,
                       hsize - PAI_W * 3 - PAI_H * 4 - 8 - 9);
  widget_set_location (pai_frame[2], hsize - PAI_W * 3 - PAI_H * 4 - 8 - 9,
                       hsize - PAI_W * 3);
  widget_set_location (pai_frame[3], hsize - PAI_W * 3,
                       hsize + PAI_W * 3 + 8);

  for (i = 0; i < 4; i++)
    {
      msg_frame[i] = widget_create ("pmcontainer", base_r);
      msg_bgframe[i] = widget_create ("pmcontainer", base_r);
      widget_set_geometry (msg_bgframe[i], PAI_W * 4, PAI_H);
    }
  choicestate_frame = widget_create ("pmcontainer", base_r);
  widget_set_geometry (choicestate_frame, PAI_W * 3, PAI_H + 4);

  widget_set_location (msg_frame[0], size - PAI_W * 5 - 8, PAI_W * 6 - PAI_H);
  widget_set_location (msg_bgframe[0], size - PAI_W * 5 - 8,
                       PAI_W * 6 - PAI_H);
  widget_set_location (msg_frame[1], PAI_H + 8, PAI_H + 8);
  widget_set_location (msg_bgframe[1], PAI_H + 8, PAI_H + 8);
  widget_set_location (msg_frame[2], PAI_H + 8, size - PAI_H * 3 - 8);
  widget_set_location (msg_bgframe[2], PAI_H + 8, size - PAI_H * 3 - 8);
  widget_set_location (msg_frame[3], size - PAI_W * 5 - 8,
                       size - PAI_H * 3 - 8);
  widget_set_location (msg_bgframe[3], size - PAI_W * 5 - 8,
                       size - PAI_H * 3 - 8);
  widget_set_location (choicestate_frame, PAI_H + 8, size - PAI_H * 2 - 8);

  /* action frame */
  {
    widget_t paiselectaction, dmy;
    integer_t w, h;
    int i, height1 = 0;

    sub_board = widget_create ("frame", base_l);
    for (i = 1; i < 4; i++)
      {                         /* 1,2,3 */
        widget_get_geometry (name_frame[0], &w, &h);
        height1 += h;
      }
    widget_configure (sub_board, resource_wrap, 0);
    widget_configure (sub_board, resource_packed, 0);   /* self locating */
    widget_set_geometry (sub_board, name_width * 2,
                         size - height1 - (PAI_H + 4) - 4);

    action = widget_create ("frame", base_l);
    paiselectaction = widget_create ("frame", action);
    choice_frame = widget_create ("pmcontainer", paiselectaction);
    widget_set_geometry (choice_frame, PAI_W * 7, PAI_H + 4);
    advicepmframe = choice_frame;

    buttonaction = widget_create ("frame", root);
    widget_configure (buttonaction, resource_packed, resource_horizontal);
    cancel_button = widget_create ("button", buttonaction);
    advice_button = widget_create ("button", buttonaction);
    tie_button = widget_create ("button", buttonaction);
    pon_button = widget_create ("button", buttonaction);
    kan_button = widget_create ("button", buttonaction);
    reach_button = widget_create ("button", buttonaction);
    tumo_button = widget_create ("button", buttonaction);
    ron_button = widget_create ("button", buttonaction);

    actionbutton_setup ();

    widget_set_geometry (advice_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (cancel_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (kan_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (tumo_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (tie_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (pon_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (reach_button, ACTBUTTON_W, BUTTON_H);
    widget_set_geometry (ron_button, ACTBUTTON_W, BUTTON_H);
    widget_pack (buttonaction);
    widget_pack (action);
  }
  widget_add_callback (advice_button,
                       resource_buttonpress_callback,
                       (void *) advice_button_callback, "advice");
  widget_add_callback (cancel_button,
                       resource_buttonpress_callback,
                       (void *) cancel_button_callback, "cancel");
  widget_add_callback (kan_button,
                       resource_buttonpress_callback,
                       (void *) kan_button_callback, "kan");
  widget_add_callback (tumo_button, resource_buttonpress_callback,
                       (void *) tumo_button_callback, "tumo");
  widget_add_callback (tie_button, resource_buttonpress_callback,
                       (void *) tie_button_callback, "tie");
  widget_add_callback (pon_button, resource_buttonpress_callback,
                       (void *) pon_button_callback, "pon");
  widget_add_callback (reach_button, resource_buttonpress_callback,
                       (void *) reach_button_callback, "reach");
  widget_add_callback (ron_button, resource_buttonpress_callback,
                       (void *) ron_button_callback, "ron");
  widget_add_callback (choice_frame,
                       resource_buttonpress_callback,
                       (void *) select_pai_callback, 5);

  widget_pack (base_l);
  if (COMMUNICATION)
    {                           /* communication */
      widget_t comm = widget_create ("frame", root);
      widget_t strbox[3];
      widget_t strs[15];
      int i, j, k = 0;
      extern char talk_messages[15][60];
      void send_play_message_callback ();

      widget_configure (comm, resource_packed, resource_horizontal);
      widget_set_geometry (comm, xwidth, 5 * JPFONTHEIGHT);
      strbox[0] = widget_create ("frame", comm);
      widget_set_geometry (strbox[0], xwidth / 3, 5 * JPFONTHEIGHT);
      strbox[1] = widget_create ("frame", comm);
      widget_set_geometry (strbox[1], xwidth / 3, 5 * JPFONTHEIGHT);
      strbox[2] = widget_create ("frame", comm);
      widget_set_geometry (strbox[2], xwidth / 3, 5 * JPFONTHEIGHT);

      for (i = 0; i < 3; i++)
        {
          widget_t f = widget_create ("frame", strbox[i]);
          widget_set_geometry (f, 8, JPFONTHEIGHT + 2);
          widget_configure (f, resource_wrap, 0);
          for (j = 0; j < 5; j++)
            {
              strs[k] = widget_create ("text", strbox[i]);
              widget_set_geometry (strs[k], xwidth / 3 - 8, JPFONTHEIGHT + 2);
              widget_configure (strs[k], resource_text, &talk_messages[k]);
              widget_configure (strs[k], resource_textfont, MEDIUMJP);
              widget_add_callback (strs[k], resource_buttonpress_callback,
                                   send_play_message_callback, k);
              k++;
            }
          widget_pack (strbox[i]);
        }
      widget_pack (comm);
    }
  widget_pack (root);
  widget_fix (root);
  message_init ((widget_t *) & msg_bgframe, (widget_t *) & msg_frame);

  bind_save (resource_button3press_callback,
             (void *) board_button3press_callback, 0);

  widget_set_geometry (root, xwidth, xheight);
}

void
make_board ()
{
  if (BUILD4)
    make_board2 ();
  else
    make_board1 ();
}

void
linepos_clear ()
{
  integer_t i;
  for (i = 0; i < 4; i++)
    {
      widget_configure (linepos_frame[i], resource_cannot_change_model, 0);
      widget_model_change (linepos_frame[i],
                           get_basiccolorpixel (), white, white, white);
    }
}

static void
linepos_config ()
{
  if (!flag_linepos)
    {
      linepos_clear ();
    }
}

void
board_redraw (integer_t sfx)
{
  extern boolean button_map_p, button_map_flag;
  integer_t flag = button_map_flag;
  integer_t map = button_map_p;

  stop_config ();
  autoplay_config ();
  auto_after_reach_config ();
  speed_config ();
  sound_config ();
  invisible_config ();
  linepos_config ();

  widget_map (board);
  message_unmap ();
  advice_clear ();
  if (map)
    {
      board_button_unmap ();
      board_button_map (flag);
      board_display ();
      board_button_display (flag);
    }
  else
    {
      board_button_unmap ();
      board_display ();
    }

  if (flag_redrawsfx && sfx)
    {
      board_flush_sfx ();
    }
  else
    {
      board_flush ();
    }
  if (map && flag_advicealways)
    {
      advice_button_callback (0, 0, 0);
    }
  msg_bg_init ();
}

void
board_sfx_redraw ()
{
  board_redraw (1);
}


void
board_reset1 ()
{
  int i;
  extern boolean button_map_p;
  board_button_unmap ();
  for (i = 0; i < 4; i++)
    {
      pmcontainer_clear (river_attr1[i]);
      pmcontainer_clear (river_frame[i]);
      pmcontainer_clear (river_attr2[i]);
      pmcontainer_clear (river_attr3[i]);
      pmcontainer_clear (river_attr4[i]);
      pmcontainer_clear (hand_frame[i]);
      pmcontainer_clear (mark_frame[i]);
      widget_display (pai_frame[i]);

      widget_display (hand_frame[i]);
      widget_display (mark_frame[i]);

      widget_flush (pai_frame[i]);
      widget_flush (hand_frame[i]);
      widget_flush (mark_frame[i]);

    }
  widget_event_discard (root_widget);
  widget_sync (root_widget);
}

void
board_reset2 ()
{
  int i, j;
  extern boolean button_map_p;
  board_button_unmap ();
  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < 4; j++)
        {
          pmcontainer_clear (river_frames[i][j]);
          pmcontainer_clear (river_attr1s[i][j]);
        }
      pmcontainer_clear (hand_frame[i]);
      pmcontainer_clear (mark_frame[i]);
      for (j = 0; j < 4; j++)
        {
          widget_display (river_frames[i][j]);
          widget_display (river_attr1s[i][j]);
        }
      widget_display (hand_frame[i]);
      widget_display (mark_frame[i]);
      for (j = 0; j < 4; j++)
        {
          widget_flush (river_frames[i][j]);
          widget_flush (river_attr1s[i][j]);
        }
      widget_flush (hand_frame[i]);
      widget_flush (mark_frame[i]);

    }
  widget_event_discard (root_widget);
  widget_sync (root_widget);
}

void
board_reset ()
{
  if (BUILD4)
    board_reset2 ();
  else
    board_reset1 ();
}

int
board_setup (int kind, char *name)
{
  int d4 = 1;
  int l = 1;

  XCLIENT = (kind == PROG_NETMAJ);
  if (!strcmp (name, "large"))
    {
      d4 = 0;
      l = 1;
    }
  else if (!strcmp (name, "large4"))
    {
      d4 = 1;
      l = 1;
    }
  else if (!strcmp (name, "small"))
    {
      d4 = 0;
      l = 0;
    }
  else if (!strcmp (name, "small4"))
    {
      d4 = 1;
      l = 0;
    }

  if (l)
    {
      board_type = PAICARD3D_V;
    }
  else
    {
      board_type = PAICARD_V;
    }
  if (d4)
    board_type |= BUILD4_V;

  if (d4)
    {
      if (PAICARD)
        {                       /* small4 */
          ACTBUTTON_W = 70;
          BUTTON_H = 24;
          BUTTON_W = 85;
#ifdef HUGEPAI
          BITMAPWIDTH = 41;
          BITMAPWIDTH2 = 56;
          BITMAPHEIGHT = 56;
          XWIDTH = PAI_W * 22 + name_width * 2 + 4;
          XHEIGHT = PAI_W * 22 + TITLE_H + 8 + BUTTON_H;
#else
          COMMUNICATION = (XCLIENT);
          BITMAPWIDTH = 25;
          BITMAPWIDTH2 = 31;
          BITMAPHEIGHT = 31;
          XWIDTH = PAI_W * 20 + name_width * 2 + 4;
          XHEIGHT = PAI_W * 20 + TITLE_H + 8 + BUTTON_H;
#endif
          TOPMARGIN = (BUTTON_H);
          BOTTOMMARGIN = (170);
        }
      else if (PAICARD3D)
        {                       /* large4 */
          ACTBUTTON_W = 70;
          BUTTON_H = 28;
          BUTTON_W = 84;
#ifdef HUGEPAI
          BITMAPWIDTH = 41;
          BITMAPWIDTH2 = 56;
          BITMAPHEIGHT = 66;
          XWIDTH = PAI_W * 23 + name_width * 2 + 4;
          XHEIGHT = PAI_W * 23 + TITLE_H + 8 + BUTTON_H * 2;
#else
          COMMUNICATION = (XCLIENT);
          BITMAPWIDTH = 26;
          BITMAPWIDTH2 = 32;
          BITMAPHEIGHT = 38;
          XWIDTH = PAI_W * 23 + name_width * 2 + 4;
          XHEIGHT = PAI_W * 23 + TITLE_H + 8 + BUTTON_H;
#endif
          TOPMARGIN = (BUTTON_H - PAI_H);
          BOTTOMMARGIN = (120) /* height of logo */ ;
          if (COMMUNICATION)
            XHEIGHT += ((JPFONTHEIGHT + 6) * 3);
        }
    }
  else
    {
      if (PAICARD)
        {                       /* small */
          ACTBUTTON_W = 70;
          BUTTON_H = 24;
          BUTTON_W = 85;
#ifdef HUGEPAI
          BITMAPWIDTH = 41;
          BITMAPWIDTH2 = 56;
          BITMAPHEIGHT = 56;
          XWIDTH = (PAI_W * 21 + (4 * 2) + name_width);
          XHEIGHT = 690;
#else
          COMMUNICATION = (XCLIENT);
          BITMAPWIDTH = 25;
          BITMAPWIDTH2 = 31;
          BITMAPHEIGHT = 31;
          XWIDTH = (PAI_W * 21 + (4 * 2) + name_width);
          XHEIGHT = 430;
#endif
          TOPMARGIN = (BUTTON_H);
          BOTTOMMARGIN = (170);
        }
      else if (PAICARD3D)
        {                       /* large */
          ACTBUTTON_W = 70;
          BUTTON_H = 28;
          BUTTON_W = 84;
#ifdef HUGEPAI
          BITMAPWIDTH = 41;
          BITMAPWIDTH2 = 56;
          BITMAPHEIGHT = 66;
          XWIDTH = (PAI_W * 22 + (4 * 2) + name_width);
          XHEIGHT = 790;
#else
          COMMUNICATION = (XCLIENT);
          BITMAPWIDTH = 26;
          BITMAPWIDTH2 = 32;
          BITMAPHEIGHT = 38;
          XWIDTH = (PAI_W * 22 + (4 * 2) + name_width);
          XHEIGHT = (660 - (PAI_H * 4));
#endif
          if (XCLIENT)
            {
              XHEIGHT += ((JPFONTHEIGHT + 6) * 5);
            }
          TOPMARGIN = (BUTTON_H - PAI_H);
          BOTTOMMARGIN = (120) /* height of logo */ ;
        }
    }
  xwidth = XWIDTH;
  xheight = XHEIGHT;
}

int
middleframe_size ()
{
#ifdef HUGEPAI
  return 600;
#else
  return 380;
#endif
}
