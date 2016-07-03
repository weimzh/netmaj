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

#include "xnet.h"
#include "xnetmaj.h"
#include "pixread.h"
#include "mydir.h"

/* about.c */
void about_init (void);
void make_about (void);
void about_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* advice.c */
void read_advice_pixmaps (void);
integer_t tenpai_p (global_t * gp, integer_t who);
void advice_button_callback (widget_t w, integer_t val, struct callbackarg *arg);
void quick_log_button_callback (widget_t w, integer_t val, struct callbackarg * arg);
void advice_clear (void);
void advice_clear_flush (void);

/* advice2.c */
void read_tiny_pixmaps (void);
void hand_pai_advice_info_clear (void);
void hand_pai_advice_info_display (global_t *gp);

/* attr.c */
void attr_init (void);
gpixmap2_t attr2pix (attr_t n, integer_t kind);

/* auto.c */
void one_callback (widget_t w, integer_t val, struct callbackarg *arg);
void auto_callback (widget_t w, integer_t val, struct callbackarg *arg);
unsigned int randval (void);
void auto_init (void);
void autoplay_config (void);
void autoplay_callback (widget_t w, integer_t val, struct callbackarg *arg);
void auto_after_reach_config (void);
void auto_after_reach_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* bdcolor.c */
void colorcombo_init (void);
integer_t get_basiccolorpixel (void);
void set_basiccolorpixel (char *name);
widget_t bdcolor_init (void);
void bdcolor_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* bg.c */
void bg_init (void);

/* board.c */
void board_button3press_callback (void);
void quit_callback (widget_t w, integer_t val, struct callbackarg *arg);
void check_callback (global_t *gp);
void get_choice (global_t *gp);
void board_sync (void);
void board_flush_sfx (void);
void board_flush (void);
void board_button_flush (void);
void board_display (void);
void make_board (void);
void board_redraw (integer_t sfx);
void board_reset (void);

/* config.c */
void config_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* getline.c */
int integerF (integer_t i);
double floatF (integer_t i);
char *stringF (integer_t i);
integer_t numberofF (void);
integer_t div_into_fields (char *s, integer_t buflen);
char **pointerF (void);
integer_t getlinefromfile (FILE * fp);

/* handler.c */
void save_handler (void *object, int (*savedaction) ());
boolean invoke_handlers (void);
void save_iohandler (int fd, char *mode, int (*handler) ());
void save_X_iohandler (int fd);
void free_iohandler (int fd);
void invoke_iohandlers (fd_set * rfd, int count);
int get_iohandler_fd_set_width (char *mode);
fd_set *get_iohandler_fd_set (char *mode);

/* hora.c */
void hora_init (widget_t parent, integer_t width);
void hora_reset (void);
void hora_set (global_t *gp, integer_t who);
void draw_hora (void);

/* init.c */
void keywait_callback (void);
void wait_key (void);
void openning (void);
void setup (void);

/* itime.c */
void set_timer_handler (int (*func) (), int interval);
void clear_timer_handler (void);
void stop_timer (void);
void resume_timer (void);

/* lang.c */
void lang_set (void);

/* msg.c */
void message_unmap (void);
void message_init (widget_t r[4], widget_t msg[4]);
int msg_tick (void);
void msg_bg_init (void);
void msg_bg_done (void);
void ui_message_fail_connect (void);
void ui_message_connecting (void);
void ui_message_connect (void);
void ui_message_disconnect (void);
void ui_message_reach (int i);
void wait4key (void);
void stop_config (void);
void stop_button_callback (widget_t w, integer_t val, struct callbackarg *arg);
int ui_message_tumo (int i);
int ui_message_ron (int i);
int ui_message_player (global_t *gp);
int ui_message_play (global_t *gp);
void speed_config (void);
void speed_callback (widget_t w, integer_t val, struct callbackarg *arg);
void comment (int from, int i, char *body);
void popup (char *msg, int i);
void send_play_message_callback (widget_t w, integer_t val, struct callbackarg *arg);
void sound_config (void);
void sound_button_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* pfdir.c */
DIR *xopendir (char *name);
integer_t read_dentry (char *name);
void close_dentry (void);
integer_t n_pf_file (void);
integer_t get_pf_file (char **array);
void free_pf_file (unsigned int n, char **array);
void check_pf_file (char *name);

/* pfinfo.c */
void read_player_info_last (char *name, integer_t count);
void tolowers (char *s);
int read_player_info (char *name);
int read_player_first_info (void);
int read_player_next_info (void);

/* pixattr.c */
void read_attr_pixmaps (void);

/* pixbdig.c */
void pmcontainer_set_bdigit (widget_t w, integer_t val);
void read_bdigit_pixmap (void);
void pmcontainer_set_adigit (widget_t w, integer_t val, integer_t flag);
void read_adigit_pixmap (void);

/* pixdec.c */
void read_decorate_pixmaps (void);
void read_openning_pixmaps (void);
void actionbutton_setup (void);

/* pixmisc.c */
void read_misc_pixmaps (void);
void read_pinfo_pixmap (void);

/* pixpai.c */
gpixmap2_t pai2pix (integer_t n, integer_t pos, integer_t reach, integer_t opened);
void pai2pix_init (void);

/* pixread.c */
void read_pixmaps (void);

/* pixyaku.c */
gpixmap2_t yaku2pix (int n);
void read_result_name (void);

/* playinfo.c */
void pinfo_init (void);
void pinfo_clear (void);
void pinfo_dir_clear (void);
void pinfo_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* rank.c */
void rank_init (void);
void rank_clear (void);
struct playerinfo *playerinfo_sort_append (struct playerinfo *list, struct playerinfo *p,
  integer_t (*compare) (struct playerinfo *, struct playerinfo *));
void rank_display (void);
void rank_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* rc.c */
void save_rcfile (FILE *fp);
void load_rcfile (char *buf);

/* res.c */
int ui_res (global_t *gp, int *howp);
int _ui_res (global_t *gp, int *howp);
int ui_choice (global_t *gp);
int _ui_choice (global_t *gp);

/* talkcmd.c */
void comment_divide (char *src, char *dest1, char *dest2);
void comment_exec_command (char *s);

/* testpai.c */
void testpai_init (void);
void testpai_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* title.c */
widget_t title_create (void);

/* x11intin.c */
void save_rcfile (FILE *fp);
void load_rcfile (char *buf);
void save_xrcfile (void);
void load_xrcfile (void);

/* xui.c */
integer_t mytblpos (integer_t i);
integer_t pos2who (integer_t i);
void ui_init (global_t *gp, int kind, char *type);
void ui_term (void);
void _print_digit (widget_t w, integer_t val);
void print_pmdigit (widget_t w, integer_t val);
void _print_str (widget_t w, char *s);
void draw_title (global_t *gp);
int ui_key (void);
void ui_draw (global_t *gp, int redraw);
void ui_expose (global_t *gp);
int ui_menu (global_t *gp, int c);
integer_t ui_event_wait (global_t *gp, int time_out);
void board_button_map (integer_t flag);
void board_button_display (integer_t flag);
void board_button_unmap (void);
integer_t choice (global_t *gp, int n_draw, int argc, gpixmap2_t argv[], int st,
  int flag);
void mark (widget_t canvas, widget_t);
void restart_callback (widget_t w, integer_t val, struct callbackarg *arg);
void game_redraw (void);
void flush_callback (widget_t w, integer_t val, struct callbackarg *arg);
void invisible_config (void);
void visible_callback (widget_t w, integer_t val, struct callbackarg *arg);

/* yaku.c */
widget_t create_basic_info_frame (char *type, widget_t parent, widget_t pmframes[6], integer_t centerwidth,
     integer_t frame3_p, integer_t button_p);
void point_init (void);
void ui_result (global_t *gp);
void gameend_init (void);
void gameend_clear (void);
int ui_message_gend (global_t *gp);
void make_point (void);

/* lang.c */
void lang_config (widget_t w, int n);
