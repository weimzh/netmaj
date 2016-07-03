#define widget_add_callback(widget, resource, func, arg) \
  _widget_add_callback ((widget), (resource), (integer_t (*)())(func), (void *)(arg))
