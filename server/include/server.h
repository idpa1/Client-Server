/**
   \file server.h
   \brief Server header file.
   <B>
   \n
    COPYRIGHT :
   </B>

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
#include "ezxml.h"


/* The port client will be connecting to */
#define PORT 6423

/* how many pending connections queue will hold */
#define BACKLOG 5

/* Reception Buffer size in bytes */
#define	SERVERRECVBUFFSIZE 4096 // 4 KB

/* Server send Buffer size in bytes */
#define	SERVERSNDBUFFSIZE 4096 // 4 KB

/* XML Status file */
#define STSFILE "XMLsts.dat"



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

