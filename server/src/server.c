#include "server.h"
#include "ezxml.h"

int main(int argc, char *argv[]) {

	/* Reception buffer */
	char recvBuff[RECVBUFFSIZE];
	/* Number of bytes read from the file descriptor */
	int num_bytes;
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
		perror("ERROR Server - Socket cannot be created");
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
		perror("ERROR Server - Socket cannot be binded");
		exit(1);
	}else{
	    printf("Server - Socket binded OK ...\n");
	}

	/* Now start listening for the clients */
	if (listen(sockfd, BACKLOG) < 0)
	{
		perror("ERROR Server - Cannot listen for the clients");
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
			perror("ERROR Server - Cannot accept a connection");
			exit(1);
		}else{
			printf("Server - Received request from Client %s:%d\n",inet_ntoa(client_addr.sin_addr),PORT);
		}

		/* If connection is established then start communicating.
		 * Iterate in case input stream is bigger than the local buffer */
		while(1)
		{
			num_bytes = read(clientfd, recvBuff, RECVBUFFSIZE-1);
			if (num_bytes < 0) {
				perror("ERROR Server - Cannot read from socket");
			    exit(1);
			}
			/* End loop when transfer ends */
		    if (num_bytes == 0)
		    	break;

		    /*  null terminate your buffer */
		    recvBuff[num_bytes] = '\0';

		    printf("Received Message: %s\n", recvBuff);

		    /* TODO: if the data exceeds the buffer's capacity
		    * reallocate the buffer with a larger size
		    *  or fail the operation*/
		}


		/* Close client connection */
		close(clientfd);
	}

	/* Clean up (should never get here) */
	close(sockfd);

	return 0;
}
