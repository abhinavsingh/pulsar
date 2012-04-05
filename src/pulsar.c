/*
 * pulsar.c
 *
 *  Created on: Apr 3, 2012
 *      Author: abhinavsingh
 */

#include <stdlib.h>

#include "server.h"

int
main(int argc, char *argv[]) {
	server *s;

	s = server_new();
	server_start(s);

	return EXIT_SUCCESS;
}
