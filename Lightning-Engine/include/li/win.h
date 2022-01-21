#ifndef LI_WIN_H
#define LI_WIN_H

typedef union li_win {
	void *p;
	unsigned long lu;
} li_win_t;

typedef enum li_win_event_type {
	li_win_event_close
} li_win_event_type_t;

typedef struct li_win_event {
	li_win_event_type_t type;
	li_win_t window;
} li_win_event_t;

int li_win_init(void);
void li_win_exit(void);
void li_win_poll(void);
int li_win_create(li_win_t *win, int width, int height);
void li_win_destroy(li_win_t win);
void li_win_map(li_win_t win);

void li_win_cb(li_win_event_t event);

#endif