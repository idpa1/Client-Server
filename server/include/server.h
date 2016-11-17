#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "ezxml.h"


// The port client will be connecting to
#define PORT 6423

// how many pending connections queue will hold
#define BACKLOG 5

// Reception Buffer size in bytes
#define	SERVERRECVBUFFSIZE 4096 // 4 KB

// Server send Buffer size in bytes
#define	SERVERSNDBUFFSIZE 4096 // 4 KB

// XML Status file
#define STSFILE "XMLsts.dat"

