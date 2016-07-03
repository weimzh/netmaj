struct widget_text
  {
    _base_widget_body;
    char *text;
    integer_t length;
    gpixmap_t pixmap;
    integer_t textcolor;
    integer_t textfont;
    byte_t textshadow;
  };

#define text_text(_text) ((struct widget_text *)_text)->text
extern struct item_type item_type_text;
