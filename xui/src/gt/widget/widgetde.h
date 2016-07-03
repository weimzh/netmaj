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

#if ANSIC || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* _3d.c */
void _3D_Draw3DRectangle P_((gdev_t dev, integer_t x, integer_t y, integer_t width, integer_t height, integer_t border_width, gc_t mainGC, gc_t topGC, gc_t bottomGC));
void _3D_Draw3DFrame P_((gdev_t dev, integer_t left, integer_t top, integer_t width, integer_t height, integer_t border_width, gc_t mainGC, gc_t topGC, gc_t bottomGC));
void _3D_DrawFrame P_((gdev_t dev, integer_t x, integer_t y, integer_t width, integer_t height, integer_t border_width, gc_t mainGC, gc_t topGC, gc_t bottomGC));

/* bind.c */
void bind_call P_((integer_t resource));
integer_t bind_save P_((integer_t resource, integer_t (*func)(), void *val));

/* canvas.c */
widget_t canvas_create P_((widget_t p));
void canvas_destroy P_((widget_t p));
void canvas_configure P_((widget_t item, integer_t resourceid, void *value));
void canvas_setup P_((widget_t w, integer_t row, integer_t col));
void canvas_add_text P_((widget_t w, integer_t row, integer_t col, char *s, integer_t color, integer_t font));
void canvas_add_pixmap P_((widget_t w, integer_t row, integer_t col, integer_t colorp, gpixmap_t content, gpixmap_t mask));
void canvas_clear P_((widget_t w));
void canvas_display P_((widget_t o));
void canvas_action P_((widget_t p));
void canvas_invoke P_((widget_t p, struct callbackarg *arg));

/* conf.c */
boolean configure_proc P_((widget_t item, configure_t config, integer_t resourceid, void *arg));

/* convini.c */
widget_t button_create P_((widget_t parent, integer_t w, integer_t h, char *text, integer_t color, integer_t font, void (*callback)(), void *arg));
widget_t text_create P_((widget_t parent, integer_t w, integer_t h, char *text, integer_t color, integer_t font, integer_t shadow));

/* core.c */
widget_t core_create P_((widget_t p));
void core_destroy P_((widget_t p));
void core_clear P_((widget_t p));
void core_configure P_((widget_t p, integer_t resourceid, void *value));
void core_display P_((widget_t p));
void core_action P_((widget_t p));
void core_invoke P_((widget_t p));

/* frame.c */
widget_t frame_create P_((widget_t p));
void frame_destroy P_((widget_t p));
void frame_configure P_((widget_t item, integer_t resourceid, void *value));
void frame_outline_draw P_((widget_t o));
void rectanglewidget_display P_((widget_t o));
void frame_display P_((widget_t p));
void frame_action P_((widget_t p));
void frame_invoke P_((widget_t p));
void frame_clear P_((widget_t p));
void widget_pack P_((widget_t o));
void widget_fix P_((widget_t o));
void widget_print_geometry P_((widget_t o, int indent));
void pixmapframe_free P_((widget_t p));

/* item.c */
widget_t widget_add_child P_((widget_t parent, widget_t child));
widget_t widget_create P_((char *s, widget_t parent));
void _widget_configure P_((widget_t s, int ac, char *av));
void widget_invoke P_((widget_t s, struct callbackarg *arg));
void widget_clear P_((widget_t s, struct callbackarg *arg));
void widget_delete P_((widget_t w));
void widget_destory P_((widget_t o));
void install_widget P_((struct item_type *p));
void item_init P_((void));

/* menu.c */
widget_t menu_create P_((widget_t p));
void menu_destroy P_((widget_t p));
void menu_clear P_((widget_t p));
void menu_configure P_((widget_t p, integer_t resourceid, void *value));
void menu_display P_((widget_t p));
void menu_action P_((widget_t p));
void menu_invoke P_((widget_t p));
widget_t menuitem_create P_((widget_t p));
void menuitem_destroy P_((widget_t p));
void menuitem_clear P_((widget_t p));
void menuitem_configure P_((widget_t p, integer_t resourceid, void *value));
void menuitem_display P_((widget_t p));
void menuitem_action P_((widget_t p));
void menuitem_invoke P_((widget_t p));

/* pixmapco.c */
widget_t pmcontainer_create P_((widget_t p));
void pmcontainer_destroy P_((widget_t p));
void pmcontainer_configure P_((widget_t item, integer_t resourceid, void *value));
void pmcontainer_add_withofs P_((widget_t w, gpixmap_t content, gpixmap_t mask, integer_t color, integer_t paramofs));
void pmcontainer_add P_((widget_t w, gpixmap_t content, gpixmap_t mask, integer_t color));
void pmcontainer_add_withmargin P_((widget_t w, gpixmap_t content, gpixmap_t mask, integer_t color, integer_t margin));
void pmcontainer_set_margin P_((widget_t w, integer_t margin));
void pmcontainer_clear P_((widget_t w));
void pmcontainer_clear_frame P_((widget_t o));
void pmcontainer_display P_((widget_t o));
void pmcontainer_action P_((widget_t p));
void pmcontainer_invoke P_((widget_t p, struct callbackarg *arg));

/* pixmapfr.c */
widget_t pixmapframe_create P_((widget_t p));
void pixmapframe_destroy P_((widget_t p));
void pixmapframe_configure P_((widget_t item, integer_t resourceid, void *value));
void pixmapframe_add_pixmap P_((widget_t w, gpixmap_t pixmap, integer_t color));
void pixmapframe_display P_((widget_t o));
void pixmapframe_action P_((widget_t p));
void pixmapframe_invoke P_((widget_t p));
void pixmapframe_clear P_((widget_t p));

/* radio.c */
integer_t radiobutton_get_value P_((widget_t o));

/* text.c */
char *widget_get_text P_((widget_t w));
boolean text_have_text_p P_((widget_t w));
integer_t text_string_n_pixels P_((widget_t w, char *str, integer_t len));

/* toplevel.c */
widget_t toplevel_create P_((widget_t p));
void toplevel_destroy P_((widget_t p));
void toplevel_clear P_((widget_t p));
void toplevel_configure P_((widget_t p));
void toplevel_display P_((widget_t p));
void toplevel_action P_((widget_t p));
void toplevel_invoke P_((widget_t p));

/* widget.c */
void widget_display P_((widget_t w));
void widget_grab P_((widget_t w));
void widget_ungrab P_((void));
widget_t widget_pointed P_((widget_t parent, integer_t x, integer_t y, integer_t *px, integer_t *py));
integer_t widget_map_p P_((widget_t parent));
void widget_map P_((widget_t parent));
void widget_unmap P_((widget_t parent));
void widget_xunmap P_((widget_t parent));
widget_t widget_init P_((integer_t w, integer_t h));
void widget_event_discard P_((widget_t w));
integer_t widget_eventsqueued P_((widget_t w));
void widget_event_loop P_((void));
void _widget_add_callback P_((widget_t widget, integer_t resource, integer_t (*func)(), void *arg));
integer_t widget_call_callback P_((widget_t widget, integer_t resource, void *callbackarg));
void widget_force_flush P_((widget_t o));
void widget_flush P_((widget_t o));
void widget_root_flush P_((widget_t o));
void widget_sync P_((widget_t o));
void widget_set_geometry P_((widget_t w, integer_t width, integer_t height));
void widget_get_geometry P_((widget_t w, integer_t *width, integer_t *height));
void wigdet_set_pack_pixmap P_((widget_t item, gpixmap_t pixmap));
void widget_abs_xy P_((widget_t o, integer_t x, integer_t y, integer_t *l, integer_t *t));
void widget_sleep P_((widget_t w));
void widget_set_color_change P_((widget_t w, integer_t fg, integer_t bg, integer_t inactivefg, integer_t inactivebg));
void widget_set_background P_((widget_t w, gpixmap_t pixmap));
void widget_dev_map P_((widget_t w));
void widget_change_models P_((widget_t root, integer_t fg));

/* widsfx.c */
void widget_flush_sfx_shatter P_((widget_t w, integer_t a, integer_t b));
void widget_flush_sfx_blind P_((widget_t w, integer_t a, integer_t b));
void widget_flush_sfx_zoom P_((widget_t w, integer_t a, integer_t b));
void widget_flush_sfx_shaffle P_((widget_t w, integer_t a, integer_t b));
void widget_flush_sfx_doubleblind P_((widget_t w, integer_t a, integer_t b));

/* widtype.c */
boolean widget_text_p P_((widget_t ));
boolean widget_menu_p P_((widget_t ));
boolean widget_frame_p P_((widget_t ));
boolean widget_menuitem_p P_((widget_t ));
boolean widget_button_p P_((widget_t ));
boolean widget_toplevel_p P_((widget_t ));
boolean widget_pixmapframe_p P_((widget_t ));
boolean widget_pmcontainer_p P_((widget_t ));
boolean widget_canvas_p P_((widget_t ));
char *widget_type_string P_((widget_t w));

/* x11evth.c */
void widget_palette_init P_((void));
void widget_define_button_hook P_((integer_t number, void (*hook)()));
void _widget_handle_event P_((void));

/* x11itemt.c */
void model_color_alloc P_((gdev_t dev, integer_t pixel));
void model_init P_((gdev_t dev, model_t m, integer_t mainfg, integer_t mainbg));
void model_change P_((gdev_t gdev, model_t m, integer_t mainfg, integer_t mainbg));
void widget_model_change P_((widget_t w, integer_t mainfg, integer_t mainbg, integer_t fg, integer_t bg));

#undef P_

