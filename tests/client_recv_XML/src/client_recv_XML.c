/**
   \file client_recv_XML.c
   \brief  Client retrieving personsPassed and humidity from the server.
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

	/* Send buffer */
	char sendBuff[SENDBUFFSIZE];

	/* Number of bytes written to the file descriptor */
	int num_bytes;

	/* Formatted buffer */
	char* fmt_buff;

	/* Test input arguments */
	if (argc < 2) {
		fprintf(stderr,"Usage: %s server_hostname\n", argv[0]);
	    exit(0);
	}

	/* Initialise a socket connection */
	sockfd = init_socket(&server_addr, argv[1]);

	/* Connect to a socket */
	connect_socket(sockfd, &server_addr);

	/* Prepare message */

	/*Write root tag*/
	ezxml_t root = ezxml_new("retrieve");

	/* Insert child_tags into root_tag */
	ezxml_t first_child_tag = ezxml_add_child(root, "key", 0);
	ezxml_set_txt(first_child_tag,"personsPassed");

	ezxml_t second_child_tag = ezxml_add_child(root, "key", 0);
	ezxml_set_txt(second_child_tag,"humidity");

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

	fmt_buff = format_buffer(sendBuff);
	printf("Client - Message sent: \n%s\n",fmt_buff);
	free(fmt_buff);

	/* Get answer from the server */

	num_bytes = read(sockfd, sendBuff, SENDBUFFSIZE-1);
	if (num_bytes < 0) {
		perror("ERROR Client - Cannot read from socket");
	    exit(1);
	}

	/*  null terminate your buffer */
	sendBuff[num_bytes] = '\0';

	fmt_buff = format_buffer(sendBuff);
	printf("Client - Message received: \n%s\n",fmt_buff);
	free(fmt_buff);

	printf("TEST SUCCESS\n");

	/* Close server connection */
	close(sockfd);

	return 0;
}
