/*
 * server.h
 *
 *  Created on: Apr 3, 2012
 *      Author: abhinavsingh
 */

#ifndef SERVER_H_
#define SERVER_H_

#define SERVER_NAME "pulsar"
#define SERVER_VERSION "0.1"

typedef struct _conf conf;
typedef struct _worker worker;

typedef struct _server server;
struct _server {
	/* config */
	conf *cfg;

	/* socket */
	int fd;
	struct event_base *base;
	struct event *signal;

	/* workers */
	worker **w;
};

server *
server_new();

void
server_start(server *s);

#endif /* SERVER_H_ */
