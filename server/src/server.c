#include "server.h"

int main(int argc, char *argv[]) {

	/* Listen connections on sockfd */
	int sockfd;
	/* File descriptor of the accepted socket of the client */
	int clientfd;

	socklen_t  addrlen;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	/* Creates a socket using:
	 *  IPv4 Internet protocols
	 *  full-duplex byte streams
	 *  single protocol	 */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
	    perror("Server - Socket cannot be created");
	    exit(1);

	}else{
	    printf("Server - Socket created OK ...\n");
	}

	/* Initialise socket address/port structure */
	memset(&server_addr, '0', sizeof(server_addr));
	server_addr.sin_family = AF_INET; // IPv4 Internet protocols
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP

	/* Assign a port number to the socket */
	if ( bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
	{
		perror("Server - Socket cannot be binded");
		exit(1);
	}else{
	    printf("Server - Socket binded OK ...\n");
	}

	/* Now start listening for the clients */
	if (listen(sockfd, BACKLOG) < 0)
	{
		perror("Server - Error listening for the clients");
		exit(1);
	}

	/* Accept connections from the clients */
	while(1)
	{
		addrlen=sizeof(client_addr);
		/* Accept a connection (creating a data pipe) */
		clientfd = accept(sockfd, (struct sockaddr *)&client_addr,&addrlen);

		if (clientfd < 0)
		{
			perror("Server - Error accepting a connection");
			exit(1);
		}else{
			printf("Server - Received request from Client %s:%d\n",inet_ntoa(client_addr.sin_addr),PORT);
		}

		close(clientfd);
	}

	/* Clean up (should never get here) */
	close(sockfd);

	return 0;
}
