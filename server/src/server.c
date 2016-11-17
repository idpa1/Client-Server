#include "server.h"


void update_values(FILE *fp, ezxml_t *src_msg, ezxml_t *xml_sts_str){

	/* Pointer to the ezxml source child struct */
	ezxml_t *xmlchild;
	/* ezxml status child structs */
	ezxml_t stsxmlchild, new_child_tag;


	/* Parse all child nodes from received XML */
	if ((*src_msg)->child != NULL){
		xmlchild = &(*src_msg)->child;

		/* Update value in the status XML structure */
		/* First child */
		stsxmlchild = ezxml_get(*xml_sts_str, (*xmlchild)->name, -1);
		if (stsxmlchild == NULL){
			new_child_tag = ezxml_add_child(stsxmlchild, (*xmlchild)->name, 0);
			ezxml_set_txt(new_child_tag,(*xmlchild)->txt);
			printf("Server - Cannot find %s in status file. Added OK ...\n",(*xmlchild)->name);
		}else{
			ezxml_set_txt(stsxmlchild,(*xmlchild)->txt);
			printf("Server - %s field updated with value %s   OK...\n",(*xmlchild)->name,(*xmlchild)->txt);
		}

		/* The rest of the children */
		while ((*xmlchild)->ordered != NULL){
			xmlchild = &(*xmlchild)->ordered;
			/* Update status file */
			stsxmlchild = ezxml_get(*xml_sts_str, (*xmlchild)->name, -1);
			if (stsxmlchild == NULL){
				new_child_tag = ezxml_add_child(stsxmlchild, (*xmlchild)->name, 0);
				ezxml_set_txt(new_child_tag,(*xmlchild)->txt);
				printf("Server - Cannot find %s in status file. Added OK ...\n",(*xmlchild)->name);
			}else{
				printf("Server - %s field updated with value %s\n",(*xmlchild)->name,(*xmlchild)->txt);
				ezxml_set_txt(stsxmlchild,(*xmlchild)->txt);
			}
		}
	}
	printf("Server - Status file updated OK ...\n");
}


void parseXML(char* buff){

	/* Status file pointer */
	FILE * file;

	/* ezxml_parse_str modify entry buffer. Let's have a copy */
	char * cpybuff = (char*)malloc(sizeof(char)*strlen(buff)+1);
	memcpy(cpybuff, buff, strlen(buff)+1);

	/* creates an ezxml structure */
	ezxml_t xml_msg = ezxml_parse_str(buff, strlen(buff));
	if (xml_msg == NULL){
		perror("ERROR Server - XML cannot be parsed");
		exit(1);
	}

	/* Update command */
	if (strcmp(xml_msg->name, "update") == 0){
		/* Update command */
		/* Check if the status file exists */
		if (access( STSFILE, F_OK ) == -1 ){

			/* File doesn't exist.
			 * Therefore we create a file and copy the XML file */
			/* Appends to a file.The file is created if it does not exist. */
			file = fopen(STSFILE, "w");
			if(file == NULL) {
				perror("ERROR server - Can't open config file\n");
				exit(1);
			}

			/* write whole buffer to the file */
			fprintf(file,"%s",cpybuff);

			printf("Server - New status file created OK ...\n");

		}else{

			/* Opens a file to update both reading and writing. */
			file = fopen(STSFILE, "r+");
			if(file == NULL) {
				perror("ERROR server - Can't open status file\n");
				exit(1);
			}

			/* creates an ezxml structure from the status file */
			ezxml_t xml_sts_str = ezxml_parse_fp(file);
			if (xml_sts_str == NULL){
				fprintf(stderr,"ERROR Server - XML status file cannot be parsed");
				exit(1);
			}

			/* Update the status values with the received XML*/
			update_values(file, &xml_msg, &xml_sts_str);

			/* write values to the status file */
			fclose(file);
			file = fopen(STSFILE, "w");
			if(file == NULL) {
				perror("ERROR server - Can't open status file\n");
			    exit(1);
			}
			char *updatedxml = ezxml_toxml(xml_sts_str);
			fprintf(file,"%s",updatedxml);

			/* Free memory */
			free(updatedxml);
			ezxml_free(xml_sts_str);
		}

		/* Close file */
		fclose(file);

	}//else(strcmp(xml_msg->name, "retrieve") == 0){
		/* Retrieve message */

//	}

	/* Free xml_msg */
	ezxml_free(xml_msg);

	/* Free buffer */
	free (cpybuff);
}

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

		    /* TODO: if the data exceeds the buffer's capacity
		    * reallocate the buffer with a larger size
		    *  or fail the operation*/
		}


	    printf("Received Message: %s\n", recvBuff);

	    /* Parse XML request */
	    parseXML(recvBuff);
	    /* Close client connection */
	    close(clientfd);
	}

	/* Clean up (should never get here) */
	close(sockfd);

	return 0;
}
