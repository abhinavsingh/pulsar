/*
 * worker.c
 *
 *  Created on: Apr 5, 2012
 *      Author: abhinavsingh
 */

#include <stdlib.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/http.h>

#include "server.h"
#include "worker.h"

void
worker_handler(struct evhttp_request *req, void *arg) {
	//worker *w = arg;
	evhttp_send_error(req, 404, "not_found");
}

void
worker_free(worker *w) {
	int i;
	void *res;

	i = pthread_cancel(w->t);
	assert(i == 0);

	i = pthread_join(w->t, &res);
	assert(i == 0);
	assert(res == PTHREAD_CANCELED);

	evhttp_free(w->http);
	event_base_free(w->base);
	free(w);
}

static void *
worker_main(void *arg) {
	int ret;
	worker *w = arg;

	w->base = event_base_new();
	assert(w->base != NULL);

	w->http = evhttp_new(w->base);
	assert(w->http != NULL);

	ret = evhttp_accept_socket(w->http, w->s->fd);
	assert(ret == 0);

	evhttp_set_gencb(w->http, &worker_handler, w);

	ret = event_base_dispatch(w->base);
	assert(ret == 0);

	evhttp_free(w->http);
	event_base_free(w->base);

	return NULL;
}

worker *
worker_new(server *s) {
	worker *w;
	w = calloc(1, sizeof(worker));

	w->s = s;

	return w;
}

void
worker_start(worker *w) {
	pthread_create(&w->t, NULL, worker_main, w);
}
