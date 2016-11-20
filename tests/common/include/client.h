/**
   \file client.h
   \brief Header file used by clients' code

   \par  Module owner:
   \li     Iker DE POY

   \par  Authors:
   \li     Iker DE POY

   \par  Id: $Id$
   \par  Date: $Date$
   \par  Revision: $Rev$
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include "ezxml.h"

// The port client will be connecting to
#define PORT 6423

// Send buffer size in bytes
#define	SENDBUFFSIZE 1024 // 1 KB


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
int init_socket(struct sockaddr_in *server_addr, char *IP_hostname);


/**
 * \brief Connect to an existing socket
 *
 * \param[in] sockfd       Sockets file descriptor
 * \param[in] server_addr  Pointer to the struct handling sockets connection
 *
 * \return returns void.
 */
void connect_socket(int sockfd, struct sockaddr_in *server_addr);


/**
 * \brief Format XML buffer.
 *        Add '\t' and '\n' to the XML buffer
 *
 * \param[in] s  Pointer to the buffer
 *
 * \return returns pointer to the formatted string
 */
char* format_buffer(char *s);


/**
 * \brief Remove '\t' and '\n' from XML buffers
 *
 * \param[in] s  Pointer to the buffer
 *
 * \return returns pointer to the unformatted string
 */
char* unformat_buffer(char *s);
