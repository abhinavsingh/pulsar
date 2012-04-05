/*
 * pulsar.c
 *
 *  Created on: Apr 3, 2012
 *      Author: abhinavsingh
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "conf.h"
#include "server.h"

void
print_usage(char *exe) {
	printf("Usage: %s [-p port] [-w workers] [-h]\n\n"
			"Options:\n"
			"  -p port        : tcp port number to listen on (default: 9090)\n"
			"  -w workers     : number of worker threads to start for accepting connections (default: 4)\n"
			"  -h             : display this help message\n", exe);
}

int
main(int argc, char *argv[]) {
	int opt;
	server *s;
	conf *cfg;

	/* defaults */
	cfg = calloc(1, sizeof(conf));
	cfg->ip = strdup("0.0.0.0");
	cfg->port = 9090;
	cfg->workers = 4;

	while((opt = getopt(argc,argv,"p:w:h")) != -1) {
		switch(opt) {
		case 'p':
			cfg->port = atoi(optarg);
			break;
		case 'w':
			cfg->workers = atoi(optarg);
			break;
		case 'h':
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	s = server_new(cfg);
	server_start(s);

	return EXIT_SUCCESS;
}
