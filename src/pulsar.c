/*
 * pulsar.c
 *
 *  Created on: Apr 3, 2012
 *      Author: abhinavsingh
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "server.h"
#include "conf.h"

int
main(int argc, char *argv[]) {
	server *s;

	s = server_new();
	server_start(s);

	return EXIT_SUCCESS;
}
