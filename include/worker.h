/*
 * worker.h
 *
 *  Created on: Apr 5, 2012
 *      Author: abhinavsingh
 */

#ifndef WORKER_H_
#define WORKER_H_

#include <pthread.h>
typedef struct _server server;

typedef struct _worker worker;
struct _worker {
	pthread_t t;

	struct event_base *base;
	struct evhttp *http;

	server *s;
};

worker *
worker_new(server *s);

void
worker_start(worker *w);

void
worker_free(worker *w);

#endif /* WORKER_H_ */
