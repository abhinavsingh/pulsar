/*
 * server.c
 *
 *  Created on: Apr 3, 2012
 *      Author: abhinavsingh
 */

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <event2/event.h>

#include "server.h"
#include "worker.h"
#include "conf.h"

int
server_setup_socket(const char *ip, short port) {
	int fd, reuse = 1, ret;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	memset(&(addr.sin_addr), 0, sizeof(addr.sin_addr));
	addr.sin_addr.s_addr = inet_addr(ip);

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(fd != -1);

	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	assert(ret != -1);

	ret = evutil_make_socket_nonblocking(fd);
	assert(ret != -1);

	ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
	assert(ret != -1);

	ret = listen(fd, SOMAXCONN);
	assert(ret != -1);

	return fd;
}

server *
server_new(conf *cfg, logger *log) {
	int i;

	server *s;
	s = calloc(1, sizeof(server));

	/* read cfg file */
	s->cfg = cfg;

	/* log */
	s->log = log;

	/* setup workers */
	s->w = calloc(s->cfg->workers, sizeof(worker *));
	for(i=0; i<s->cfg->workers; i++) {
		s->w[i] = worker_new(s);
	}
	log_it(s->log, PULSAR_DEBUG, "workers initialized ...");

	return s;
}

void
server_free(server *s) {
	int i;

	/* shutdown worker threads */
	for(i=0; i<s->cfg->workers; i++) {
		worker_free(s->w[i]);
	}

	/* free */
	event_del(s->signal);
	event_free(s->signal);
	event_base_free(s->base);
	close(s->fd);
	free(s->w);
	conf_free(s->cfg);
	log_free(s->log);
	free(s);
}

static void
server_sig_handler(evutil_socket_t fd, short event, void *arg) {
	server *s = arg;
	server_free(s);
	exit(EXIT_SUCCESS);
}

void
server_start(server *s) {
	int i;

	s->base = event_base_new();
	s->signal = event_new(s->base, SIGINT, EV_SIGNAL|EV_PERSIST, &server_sig_handler, s);
	event_add(s->signal, NULL);

	s->fd = server_setup_socket(s->cfg->ip, s->cfg->port);
	assert(s->fd != -1);

	/* start workers */
	for(i=0; i<s->cfg->workers; i++) {
		worker_start(s->w[i]);
	}

	event_base_dispatch(s->base);

	server_free(s);
	exit(EXIT_SUCCESS);
}
