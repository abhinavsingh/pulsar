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
#include <event2/buffer.h>

#include "server.h"
#include "worker.h"

void
worker_handler(struct evhttp_request *req, void *arg) {
	//worker *w = arg;

	struct evhttp_connection *conn;
	enum evhttp_cmd_type method;
	struct evhttp_uri *uri;
	struct evkeyvalq *headers;

	const char *host, *uri_host, *uri_path, *uri_query, *uri_user_info, *uri_scheme;
	ev_uint16_t port;
	char *addr;
	int uri_port;

	conn = evhttp_request_get_connection(req);
	host = evhttp_request_get_host(req);
	evhttp_connection_get_peer(conn, &addr, &port);
	method = evhttp_request_get_command(req);
	uri = evhttp_request_get_evhttp_uri(req);
	headers = evhttp_request_get_input_headers(req);
	uri_host = evhttp_uri_get_host(uri);
	uri_path = evhttp_uri_get_path(uri);
	uri_query = evhttp_uri_get_query(uri);
	uri_user_info = evhttp_uri_get_userinfo(uri);
	uri_scheme = evhttp_uri_get_scheme(uri);
	uri_port = evhttp_uri_get_port(uri);

	printf("addr:%s, port:%d, host:%s, method:%d, host:%s, path:%s, query:%s, user info:%s, scheme:%s, port:%d\n",
			addr, port, host, method, uri_host, uri_path, uri_query, uri_user_info, uri_scheme, uri_port);

	struct evbuffer *buffer;
	buffer = evbuffer_new();
	evbuffer_add_printf(buffer, "%s", "hello world!");
	evhttp_send_reply(req, HTTP_OK, "OK", buffer);
	evbuffer_free(buffer);
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
