/**
   \file common.c
   \brief Common source file used by servers' code

   \par  Module owner:
   \li     Iker DE POY 

   \par  Authors:
   \li     Iker DE POY 

   \par  Id: $Id$
   \par  Date: $Date$
   \par  Revision: $Rev$
*/

#include "server.h"


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
	char *p = calloc(SERVERSNDBUFFSIZE,sizeof(char));

	char *p2 = p;
	while(*s != '\0') {
		if((*s == '>') && (*(s+1) == '<')) {
			if (p2+3 > &p[SERVERSNDBUFFSIZE]){
				perror("Server ERROR - The destination buffer is not big enough. Cannot write it.");
			}
			*p2++ = *s++;
			*p2++='\n';

			if ((strncmp(s+1,"retrieve", 8) != 0) && (strncmp(s+2,"retrieve", 8) != 0) && (strncmp(s+1,"update", 8) != 0) && (strncmp(s+2,"update", 6) != 0) && (strncmp(s+2,"status", 6) != 0) && (strncmp(s+1,"status", 6) != 0)){
				*p2++='\t';
			}
		} else {
			if (p2+1 > &p[SERVERSNDBUFFSIZE]){
				perror("Server ERROR - The destination buffer is not big enough. Cannot write it.");
			}
			*p2++ = *s++;
		}
	}
	*p2 = '\0';

	return p;
}

