#if __STDC__ || ANSIC
#define P_(s) s
#else
#define P_(s) ()
#endif


boolean nu_source P_((char *fname));
void *name_value P_((char *name));
boolean name_exist P_((char *name));

#undef P_
