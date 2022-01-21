#ifndef LI_WIN_H
#define LI_WIN_H

typedef union li_win {
	void *p;
	unsigned long lu;
} li_win_t;

typedef enum li_event_type {
	li_event_close,
	li_event_key_press,
	li_event_key_release,
	li_event_button_press,
	li_event_button_release,
	li_event_motion_notify,
} li_event_type_t;

typedef struct li_event_key {
	int key;
} li_event_key_t;

typedef struct li_event_button {
	int x;
	int y;
	int button;
} li_event_button_t;

typedef struct li_event_motion {
	int x;
	int y;
} li_event_motion_t;

typedef union li_event_data {
	li_event_key_t key;
	li_event_button_t button;
	li_event_motion_t motion;
} li_event_data_t;

typedef struct li_event {
	li_event_type_t type;
	li_win_t window;
	li_event_data_t data;
} li_event_t;

int li_win_init(void);
void li_win_exit(void);
void li_win_poll(void);
int li_win_create(li_win_t *win, int width, int height);
void li_win_destroy(li_win_t win);
void li_win_map(li_win_t win);

void li_win_cb(li_event_t event);

#endif