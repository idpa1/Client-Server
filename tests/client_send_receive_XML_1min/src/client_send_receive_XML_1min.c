/**
   \file client_send_recv_XML_1min.c
   \brief  Client test sending and receiving 60 different messages every second.
   	   	   The socket remains open during 60 seconds.
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

	/* PersonsPassed and humidity values */
	int pp = 100, humid = 10;

	/* Number of messages to send */
	int nb_msg = 60;

	/* Send buffer */
	char sendBuff[SENDBUFFSIZE];

	/* Number of bytes written to the file descriptor */
	int num_bytes;
	/* Ezxml messages */
	ezxml_t root, child_tag;
	char snum[5];
	/* XML string message */
	char *xml;

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
	while(nb_msg > 0){
		/*Write root tag*/
		root = ezxml_new("update");
		/* Convert integer into string */
		sprintf(snum,"%d",pp++);

		/* Insert child_tags into root_tag */
		child_tag = ezxml_add_child(root, "personsPassed", 0);
		ezxml_set_txt(child_tag,snum);

		/* Convert integer into string */
		sprintf(snum,"%d",humid++);

		child_tag = ezxml_add_child(root, "humidity", 0);
		ezxml_set_txt(child_tag,snum);

		xml = ezxml_toxml(root);

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

		printf("Client - Message sent: %s\n", sendBuff);

		/* Retrieve message from the server */
		root = ezxml_new("retrieve");
		child_tag = ezxml_add_child(root,"humidity", 0);

		xml = ezxml_toxml(root);

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
		printf("Client - Message sent: %s\n", sendBuff);
		/* Get answer from the server */

		num_bytes = read(sockfd, sendBuff, SENDBUFFSIZE-1);
		if (num_bytes < 0) {
			perror("ERROR Client - Cannot read from socket");
		    exit(1);
		}

		/*null terminate your buffer*/
		sendBuff[num_bytes] = '\0';
		printf("Client - Received Message: %s\n", sendBuff);

		/* Check the results */
		/* creates an ezxml structure */
		root = ezxml_parse_str(sendBuff, strlen(sendBuff));
		if (root == NULL){
			perror("ERROR Server - XML cannot be parsed");
			exit(1);
		}

		child_tag = ezxml_get(root,"humidity", -1);

		if (atoi(child_tag->txt) != humid-1){
			perror("Client ERROR - The value received if wrong.");
			exit(1);
		}else{
			printf("Client - The value received (%d) is OK... \n",atoi(child_tag->txt));
		}
		/* Cleanup*/
		ezxml_free(root);

		nb_msg--;
		sleep(1);
	}

	/* Close server connection */
	close(sockfd);
	printf("TEST SUCCESS\n");

	return 0;
}