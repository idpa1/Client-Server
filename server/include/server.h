#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

// The port client will be connecting to
#define PORT 6423

// how many pending connections queue will hold
#define BACKLOG 5

// Reception Buffer size in bytes
#define	RECVBUFFSIZE 4096 // 4 KB
