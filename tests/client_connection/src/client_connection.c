/**
   \file client_sconnection.c
   \brief  Client testing connection with the server.
           The socket is closed after each message.
           This program will be specified an IP address in the command line
           and will connect to an already running server.

   \par  Module owner:
   \li      Iker DE POY

   \par  Authors:
   \li      Iker DE POY

   \par  Id: $Id$
   \par  Date: $Date$
   \par  Revision: $Rev$
*/

#include "client.h"

int main(int argc, char *argv[]) {

	/* Listen connections on sockfd */
	int sockfd;

	/* sockets connection struct */
	struct sockaddr_in server_addr;

	/* Test input arguments */
	if (argc < 2) {
		fprintf(stderr,"Usage: %s server_hostname\n", argv[0]);
	    exit(0);
	}

	/* Initialise a socket connection */
	sockfd = init_socket(&server_addr, argv[1]);

	/* Connect to a socket */
	connect_socket(sockfd, &server_addr);

	printf("TEST SUCCESS\n");

	/* Close connection */
	close(sockfd);

	return 0;
}
