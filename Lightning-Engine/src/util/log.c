#include "li/util/log.h"

#include "li/assert.h"
#include "li/memory.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void li_log_init(li_logger_t *logger) {
	logger->offset = 0;
	logger->sinks = NULL;
	logger->fmt_proc = NULL;
}

void li_log_destroy(li_logger_t *logger) {
	free(logger->data);
	if (logger->sinks)
		li_lstclr(logger->sinks);
}

void li_log_set_fmt(li_logger_t *logger, li_log_fmt_proc fmt_proc) {
	logger->fmt_proc = fmt_proc;
}

void li_sink_init(li_sink_t *sink, li_sink_flush_proc flush_proc, void *param) {
	sink->flush_proc = flush_proc;
	sink->param = param;
}

int li_log_add_sink(li_logger_t *logger, li_sink_flush_proc flush_proc, void *param) {
	li_sink_t *sink;

	sink = li_safe_malloc(sizeof(*sink));
	li_sink_init(sink, flush_proc, param);
	li_lstadd_back(&logger->sinks, li_lstnew(sink));
	return 0;
}

int li_log_del_sink(li_logger_t *logger, li_sink_t *sink) {
	(void) logger;
	(void) sink;
	return -1;
}

void _li_sink_flush(void *sink, void *log_str) {
	((li_sink_t *) sink)->flush_proc(sink, log_str);
}

void _li_log_flush(li_logger_t *logger, const char *restrict buffer, size_t len) {
	li_log_str_t tmp_str;

	tmp_str.str = buffer;
	tmp_str.len = len;

	li_lstfor(logger->sinks, _li_sink_flush, &tmp_str);
	logger->offset = 0;
}

void _li_log_write(li_logger_t *logger, const char *restrict str, size_t len) {
	size_t rem;

	rem = LI_LOG_BUFFER_SIZE - logger->offset - 1;
	if (rem < len) {
		_li_log_write(logger, str, rem);
		li_log_flush(logger);
		if ((len - rem) >= LI_LOG_BUFFER_SIZE)
			_li_log_flush(logger, str + rem, len - rem);
		else
			_li_log_write(logger, str + rem, len - rem);
	} else {
		memcpy(&logger->data[logger->offset], str, len);
		logger->offset += len;
		logger->data[logger->offset] = '\0';
	}
}

int li_log(li_logger_t *logger, const char *restrict format, ...) {
	va_list args;
	char *buffer, *fmt_str;
	int len, fmt_len;

	va_start(args, format);
	len = vsnprintf(NULL, 0, format, args) + 1;
	
	buffer = li_safe_malloc(sizeof(char) * len);

	vsnprintf(buffer, len, format, args);
	if (logger->fmt_proc) {
		fmt_len = logger->fmt_proc(&fmt_str, buffer);
		_li_log_write(logger, fmt_str, fmt_len);
		free(fmt_str);
	} else
		_li_log_write(logger, buffer, len - 1);
	free(buffer);
	return (len - 1);
}

void li_log_flush(li_logger_t *logger) {
	if (logger->offset)
		_li_log_flush(logger, logger->data, logger->offset);
}
