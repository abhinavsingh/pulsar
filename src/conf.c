/*
 * conf.c
 *
 *  Created on: Apr 7, 2012
 *      Author: abhinavsingh
 */

#include <stdlib.h>

#include "conf.h"

void
conf_free(conf *cfg) {
	free(cfg->ip);
	free(cfg);
}

conf *
conf_new(char *ip, unsigned short port, int workers) {
	conf *cfg;
	cfg = calloc(1, sizeof(conf));

	cfg->ip = ip;
	cfg->port = port;
	cfg->workers = workers;

	return cfg;
}
