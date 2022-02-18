#ifndef LI_LOG_H
#define LI_LOG_H

#include <li/util/list.h>
#include <sys/types.h>

#define li_log_asink li_log_add_sink
#define li_log_dsink li_log_del_sink

#ifndef LI_LOG_BUFFER_SIZE
#define LI_LOG_BUFFER_SIZE 1024
#endif

typedef struct li_log_str li_log_str_t;
typedef struct li_flush_param li_flush_param_t;
typedef struct li_sink li_sink_t;
typedef struct li_logger li_logger_t;

struct li_log_str {
	const char *str;
	size_t len;
};

typedef void (*li_sink_flush_proc)(li_sink_t*, li_log_str_t*);

struct li_flush_param {
	const char *str;
	size_t len;
	void *param;
};

struct li_sink {
	li_sink_flush_proc flush_proc;
	void *param;
};

struct li_logger {
	char data[LI_LOG_BUFFER_SIZE];
	size_t offset;
	li_list_t *sinks;
};

void li_log_init(li_logger_t *logger);
void li_log_destroy(li_logger_t *logger);

void li_sink_init(li_sink_t *sink, li_sink_flush_proc flush_proc, void *param);

int li_log_add_sink(li_logger_t *logger, li_sink_t *sink);
int li_log_del_sink(li_logger_t *logger, li_sink_t *sink);

int li_log(li_logger_t *logger, const char *format, ...);
void li_log_flush(li_logger_t *logger);
#endif
