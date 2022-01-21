#ifndef LI_WIN_H
#define LI_WIN_H

typedef union li_win {
	void *p;
	unsigned long lu;
} li_win_t;

void li_win_init(void);
void li_win_exit(void);
void li_win_poll(void);
li_win_t li_win_create(int width, int height);
void li_win_destroy(li_win_t win);
void li_win_map(li_win_t win);

#endif
