/*
 * logger.c
 *
 *  Created on: Apr 7, 2012
 *      Author: abhinavsingh
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "server.h"
#include "conf.h"

void
log_free(logger *log) {
	free(log);
}

logger *
log_new(char *logfile, log_level level) {
	logger *log;
	log = (logger *)calloc(1, sizeof(logger));

	log->self = getpid();
	log->logfile = logfile;
	log->verbosity = level;

	if(log->logfile) {
		log->fd = open(log->logfile, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR|S_IWUSR);
	}
	else {
		/* stderr */
		log->fd = 2;
	}

	return log;
}

void
log_it(logger *log, log_level level, const char *body) {
	const char *c = ".-*#";

	time_t now;
	char time_buf[64];

	size_t sz;
	char msg[124];

	char line[256];
	int line_sz, ret;

	if(level > log->verbosity) return;
	if(!log->fd) return;

	/* limit max log size */
	sz = strlen(body);
	snprintf(msg, sz + 1 > sizeof(msg) ? sizeof(msg) : sz + 1, "%s", body);

	/* time */
	now = time(NULL);
	strftime(time_buf, sizeof(time_buf), "%d %b %H:%M:%S", localtime(&now));

	/* out line */
	line_sz = snprintf(line, sizeof(line), "[%d] %s %d %s\n", (int)log->self, time_buf, c[level], msg);

	/* write to log and flush to disk. */
	ret = write(log->fd, line, line_sz);
	ret = fsync(log->fd);

	(void)ret;
}
