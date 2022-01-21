#ifndef LI_DL_H
#define LI_DL_H

typedef union li_dl {
	void *p;
} li_dl_t;

li_dl_t li_dl_open(const char *filename);
void *li_dl_sym(li_dl_t dl, const char *symbol);
void li_dl_close(li_dl_t dl);

#endif