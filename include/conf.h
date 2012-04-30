/*
 * conf.h
 *
 *  Created on: Apr 3, 2012
 *      Author: abhinavsingh
 */

#ifndef CONF_H_
#define CONF_H_

#include "logger.h"
#include "common.h"

struct _conf {
	char *ip;
	unsigned short port;
	int workers;
};

void
conf_free();

conf *
conf_new(char *ip, unsigned short port, int workers);

#endif /* CONF_H_ */
