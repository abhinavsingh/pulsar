/*
 * pulsar.c
 *
 *  Created on: Apr 3, 2012
 *      Author: abhinavsingh
 *
 *		Memory Leak Check:
 *		-------------------
 *      valgrind --leak-check=full --show-reachable=yes ./pulsar
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
			"  -v verbosity   : 0 <= verbosity <= 4 (default: 4)\n"
			"  -l logfile     : file to log into (default: stderr)\n"
			"  -h             : display this help message\n", exe);
}

int
main(int argc, char *argv[]) {
	int opt;

	server *s;
	conf *cfg;
	logger *log;

	/* defaults */
	log = log_new("log/pulsar.log", PULSAR_DEBUG);
	cfg = conf_new(strdup("0.0.0.0"), 9090, 4);

	/* read input options */
	while((opt = getopt(argc,argv,"p:w:v:l:h")) != -1) {
		switch(opt) {
		case 'p':
			cfg->port = atoi(optarg);
			break;
		case 'w':
			cfg->workers = atoi(optarg);
			break;
		case 'v':
			log->verbosity = atoi(optarg);
			break;
		case 'l':
			log->logfile = optarg;
			break;
		case 'h':
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		case '?':
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	s = server_new(cfg, log);
	server_start(s);

	return EXIT_SUCCESS;
}
