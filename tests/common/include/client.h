#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

// The port client will be connecting to
#define PORT 6423

// Send buffer size in bytes
#define	SENDBUFFSIZE 1024 // 1 KB