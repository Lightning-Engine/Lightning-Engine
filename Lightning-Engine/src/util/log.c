#include "li/log/log.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void li_log_init(li_logger_t *logger) {
	logger->data = NULL;
	logger->offset = 0;
	logger->size = 0;
	logger->sinks = 0;
}

void li_log_destroy(li_logger_t *logger) {
	free(logger->data);
	if (logger->sinks)
		li_lstclr(logger->sinks);
}

void li_sink_init(li_sink_t *sink, li_sink_flush_proc *flush_proc, void *param) {
	sink->flush_proc = flush_proc;
	sink->param = param;
}

int li_log_add_sink(li_logger_t *logger, li_sink_t *sink) {
	if (li_lstadd_back(&logger->sinks, sink))
		return (0);
	return -1;
}

int li_log_del_sink(li_logger_t *logger, li_sink_t *sink) {
	if (li_lstrem(&logger->sinks, li_ptreq, sink))
		return 0;
	return -1;
}

void _li_sink_flush(void *sink, void *log_str) {
	sink->flush_proc(sink, log_str);
}

void _li_log_flush(li_logger_t *logger, const char *buffer, size_t len) {
	li_log_str_t log_str;

	log_str.str = buffer;
	log_str.len = len;
	li_lstfor(logger->sinks, _li_sink_flush, &log_str);
	logger->offset = 0;
}

void _li_log_write(li_logger_t *logger, const char *restrict str, size_t len) {
	int count, rem;

	rem = LI_LOGGER_BUFFER_SIZE - logger->offset;
	if (rem < len) {
		count = len - LI_LOGGER_BUFFER_SIZE - logger->offset;
		if (count > LI_LOGGER_BUFFER_SIZE) {
			li_log_flush(logger);
			_li_log_flush(str, len);
		} else {
			_li_log_write(logger, str, rem);
			memcpy(logger->data, str + rem, len - rem);
			logger->offset = len - rem;
		}
	} else
		memcpy(&logger->data[logger->offset], str, len);
}

int li_log(li_logger_t *logger, const char *restrict format, ...) {
	va_list args;
	char *buffer;
	int len;

	args = va_start(args, format);
	len = vsprintf(NULL, format, args);
	
	buffer = malloc(sizeof(char) * len);
	li_assert(buffer);
	
	vsprintf(buffer, format, args);
	_li_log_write(logger, buffer, len);
	return (len);
}

void li_log_flush(li_logger_t *logger) {
	_li_log_flush(logger, logger->data, logger->offset);
}