/*
*DESCRIPTION: This program will be specified an IP address in the command line
*			  and will connect to an already running server.
*/

#include "client.h"
#include "ezxml.h"

int main(int argc, char *argv[]) {

	/* Handle server hostname */
	struct hostent *server;

	/* Listen connections on sockfd */
	int sockfd;

	struct sockaddr_in server_addr;

	/* Send buffer */
	char sendBuff[SENDBUFFSIZE];
	/* Number of bytes written to the file descriptor */
	int num_bytes;

	/* Test input arguments */
	if (argc < 2) {
		fprintf(stderr,"Usage: %s server_hostname\n", argv[0]);
	    exit(0);
	}
	/* Get server hostname */
	server = gethostbyname(argv[1]);
	if (server == NULL) {
	    fprintf(stderr,"ERROR Client - Socket cannot be created\n");
	    exit(0);
	}

	/* Creates a socket using:
	 *  IPv4 Internet protocols
	 *  full-duplex byte streams
	 *  single protocol	 */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
	    perror("ERROR Client - Socket cannot be created");
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
		perror("ERROR Client - Cannot connect to the server");
		exit(1);
	}else{
	    printf("Client - Connected to the server OK ...\n");
	}

	/* Prepare message */

	/*Write root tag*/
	ezxml_t root = ezxml_new("update");

	/* Insert child_tags into root_tag */
	ezxml_t first_child_tag = ezxml_add_child(root, "personsPassed", 0);
	ezxml_set_txt(first_child_tag,"12");

	ezxml_t second_child_tag = ezxml_add_child(root, "lightCondition", 0);
	ezxml_set_txt(second_child_tag,"bright");

	ezxml_t third_child_tag = ezxml_add_child(root, "humidity", 0);
	ezxml_set_txt(third_child_tag,"52");

	char *xml = ezxml_toxml(root);

	/* Copy to the write buffer*/
	if ((int)strlen(xml) >= SENDBUFFSIZE){
		perror("ERROR Client - XML Message is bigger than the send buffer");
		exit(1);
	}else{
		memcpy(sendBuff,xml,strlen(xml)+1);
	}
	/* Free xml*/
	free(xml);
	/* Cleanup*/
	ezxml_free(root);

	/*Send message to the server */
	num_bytes = write(sockfd, sendBuff, strlen(sendBuff));
	if (num_bytes < 0) {
		perror("ERROR Client - Cannot writing to socket");
		exit(1);
	}

	printf("Message size: %lu\n",strlen(sendBuff));
	printf("Message sent: %s\n", sendBuff);

	close(sockfd);

	return 0;
}
