/**
   \file client.c
   \brief Common source file used by clients' code 

   \par  Module owner:
   \li     Iker DE POY

   \par  Authors:
   \li     Iker DE POY

   \par  Id: $Id$
   \par  Date: $Date$
   \par  Revision: $Rev$
*/

#include "client.h"

/**
 * \brief Initialize a socket using:
 *     	  - IPv4 Internet protocols
 *     	  -	full-duplex byte streams
 *     	  - single protocol
 *
 * \param[in] server_addr  Pointer to the struct handling sockets connection
 * \param[in] IP_hostname  Server hostname or IP address
 *
 * \return returns an file descriptor of the socket.
 */
int init_socket(struct sockaddr_in *server_addr, char *ip_hostname){
	/* Listen connections on sockfd */
	int sockfd;

	/*Get server IP
	TODO: Translate hostname to IP address */

	/* Server IP address */
//	char ip[100];
//	hostname_to_ip(ip_hostname, ip);

	/* Initialize socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
	    perror("Client - Socket cannot be created");
	    exit(1);

	}

	printf("Client - Socket created OK ...\n");

	/* Initialise socket address/port structure */
	memset(server_addr, '0', sizeof(*server_addr));
	server_addr->sin_family = AF_INET; // IPv4 Internet protocols
	server_addr->sin_port = htons(PORT);
	server_addr->sin_addr.s_addr = inet_addr(ip_hostname);

	/* Return file descriptor */
	return sockfd;
}


/**
 * \brief Connect to an existing socket
 *
 * \param[in] sockfd       Sockets file descriptor
 * \param[in] server_addr  Pointer to the struct handling sockets connection
 *
 * \return returns void
 */
void connect_socket(int sockfd, struct sockaddr_in *server_addr){
	/* Now connect to the server */
	if (connect(sockfd, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0)
	{
		perror("Client - Cannot connect to the server");
		exit(1);
	}else{
		printf("Client - Connected to the server OK ...\n");
	}
}


/**
 * \brief Remove '\t' and '\n' from XML buffers
 *
 * \param[in] s  Pointer to the buffer
 *
 * \return returns pointer to the unformatted string
 */
char* unformat_buffer(char *s) {
	char *p = malloc(strlen(s) + 1);

	if(p) {
		char *p2 = p;
		while(*s != '\0') {
			if(*s != '\t' && *s != '\n') {
				*p2++ = *s++;
			} else {
				++s;
			}
		}
		*p2 = '\0';
	}
	return p;
}

/**
 * \brief Format XML buffer.
 *        Add '\t' and '\n' to the XML buffer
 *
 * \param[in] s  Pointer to the buffer
 *
 * \return returns pointer to the formatted string
 */
char* format_buffer(char *s) {
	char *p = calloc(SENDBUFFSIZE,sizeof(char));
	char *p2 = p;

	while(*s != '\0') {
		if((*s == '>') && (*(s+1) == '<')) {
			if (p2+3 > &p[SENDBUFFSIZE]){
				perror("Server ERROR - The destination buffer is not big enough. Cannot write it.");
			}
			*p2++ = *s++;
			*p2++='\n';

			if ((strncmp(s+2,"retrieve", 8) != 0) && (strncmp(s+2,"update", 6) != 0) && (strncmp(s+2,"status", 6) != 0)){
				*p2++='\t';
			}
		} else {
			if (p2+1 > &p[SENDBUFFSIZE]){
				perror("Server ERROR - The destination buffer is not big enough. Cannot write it.");
			}
			*p2++ = *s++;
		}
	}
	*p2 = '\0';

	return p;
}


/*
    Get ip from domain name
 */

//int hostname_to_ip(char *hostname , char *ip)
//{
//    int sockfd;
//    struct addrinfo hints, *servinfo, *p;
//    struct sockaddr_in *h;
//    int rv;
//
//    memset(&hints, 0, sizeof hints);
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_STREAM;
//
//    if ( (rv = getaddrinfo( hostname , "http" , &hints , &servinfo)) != 0)
//    {
//        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
//        return 1;
//    }
//
//    // loop through all the results and connect to the first we can
//    for(p = servinfo; p != NULL; p = p->ai_next)
//    {
//        h = (struct sockaddr_in *) p->ai_addr;
//        strcpy(ip , inet_ntoa( h->sin_addr ) );
//    }
//
//    /*Free mem */
//    freeaddrinfo(servinfo);
//    return 0;
//}
