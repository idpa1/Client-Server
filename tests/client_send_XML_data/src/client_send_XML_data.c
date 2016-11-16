
/*
*DESCRIPTION: This program will be specified an IP address in the command line
*			  and will connect to an already running server.
*/

#include "client.h"

int main(int argc, char *argv[]) {

	/* Handle server hostname */
	struct hostent *server;

	/* Listen connections on sockfd */
	int sockfd;

	struct sockaddr_in server_addr;

	/* Test input arguments */
	if (argc < 2) {
		fprintf(stderr,"Usage: %s server_hostname\n", argv[0]);
	    exit(0);
	}
	/* Get server hostname */
	server = gethostbyname(argv[1]);
	if (server == NULL) {
	    fprintf(stderr,"Client ERROR - Socket cannot be created\n");
	    exit(0);
	}

	/* Creates a socket using:
	 *  IPv4 Internet protocols
	 *  full-duplex byte streams
	 *  single protocol	 */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
	    perror("Client - Socket cannot be created");
	    exit(1);

	}else{
	    printf("Client - Socket created OK ...\n");
	}

	/* Initialise socket address/port structure */
	memset(&server_addr, '0', sizeof(server_addr));
	server_addr.sin_family = AF_INET; // IPv4 Internet protocols
	server_addr.sin_port = htons(PORT);
	memcpy( &server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

	/* Now connect to the server */
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Client - Cannot connect to the server");
		exit(1);
	}else{
	    printf("Client - Connected to the server OK ...\n");
	}

	/*Send message to the server */
	close(sockfd);

	return 0;
}
