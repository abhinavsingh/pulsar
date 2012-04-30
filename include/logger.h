/*
 * logger.h
 *
 *  Created on: Apr 7, 2012
 *      Author: abhinavsingh
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "common.h"

typedef enum {
	PULSAR_ERROR = 0,
	PULSAR_WARNING,
	PULSAR_NOTICE,
	PULSAR_INFO,
	PULSAR_DEBUG
} log_level;

struct _logger {
	pid_t self;
	int fd;

	char *logfile;
	log_level verbosity;
};

void
log_free(logger *log);

logger *
log_new(char *logfile, log_level level);

void
log_it(logger *log, log_level level, const char *body);

#endif /* LOGGER_H_ */
