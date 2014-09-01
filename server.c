// stateful file locking client 

#include <stdio.h>
#include <stdlib.h>

// libraries for sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// application struct
#include "request.h"

// max buffer size
#define BUFSIZE 2048

void usage () {
  printf("Usage: server PORT\n");
}

// port - desired port number
// returns if server process was successful
int server (short port) {
  
  // setup a socket
  int s; // socket connection
  struct sockaddr_in myaddr;
  struct sockaddr_in remaddr;     /* remote address */
  socklen_t addrlen = sizeof(remaddr); // length of addresses
  int recvlen;                    /* # bytes received */
  unsigned char buf[BUFSIZE];     /* receive buffer */
  
  // SOCK_DGRAM - another name for packets , associated with connectionless
  // protocols
  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s < 0) {
    printf("ERROR: cannot create socket.\n");
    return s;
  }

  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(port);


  if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
    printf("ERROR: failed to bind to socket.\n");
    return -1;
  }

  printf("Server listening on port %d\n", port);
  fflush(stdout); // display message before entering loop

  // wait for connections
  while (1 == 1) {
    recvlen = recvfrom(s, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
    printf("received %d bytes\n", recvlen);
    if (recvlen > 0) {
      buf[recvlen] = 0;
      printf("received message: \"%s\"\n", buf);
    }

  }

  return 0;
}

int main (int args, char *argv[]) {

  // port number to start server on, taken in as first commandline argument.
  short port;

  // didn't provide port number
  if (args < 2) {
    printf("ERROR: Please supply port number.\n");
    usage(); // print usage information
    return -1;
  }

  port = atoi(argv[1]);

  // port specified was not a number
  if (port == 0) {
    printf("ERROR: Port specified not a number.\n");
    usage(); // print usage information
    return -2;
  }

  return server(port);
}
