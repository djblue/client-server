// stateful file locking client 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// libraries for sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// application struct
#include "fs.h"
#include "request.h"
#include "response.h"
#include "error.h"

void usage (char* file) {
  printf(
    "Usage: %s PORT\n\n"
    "  PORT - port number of server\n\n"
  , file);
}

// parse operation command and dispatch file operations
void dispatch (request *req, response *res) {

  client *c = &req->c;
  // parse operation
  char *cmd = strtok(req->operation, " ");
  char *file = strtok(NULL, " ");
  char *arg = strtok(NULL, " ");

  if (strncmp("open", cmd, 4) == 0) {
    res->status = file_open(c, file, arg);
  } else if (strncmp("close", cmd, 5) == 0) {
    res->status = file_close(c, file);
  } else if (strncmp("read", cmd, 4) == 0) {
    res->status = file_read(c, file, atoi(arg), res->content);
  } else if (strncmp("write", cmd, 5) == 0) {
    res->status = file_write(c, file, arg);
  } else if (strncmp("lseek", cmd, 5) == 0) {
    res->status = file_lseek(c, file, atoi(arg));
  } else {
    res->status = -1;
    strcpy(res->content, "unknown file operation.");
  }
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
  request req;
  
  // SOCK_DGRAM - another name for packets , associated with connectionless
  // protocols
  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s < 0) {
    error("cannot create socket");
    return s;
  }

  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(port);


  if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
    error("failed to bind to socket");
    return -1;
  }

  printf("Server listening on port %d\n", port);
  fflush(stdout); // display message before entering loop

  // wait for connections
  while (1 == 1) {
    recvlen = recvfrom(s, &req, sizeof(req), 0, (struct sockaddr *)&remaddr, &addrlen);
    if (recvlen > 0) {
      printq(&req);
      response res;
      dispatch(&req, &res);
      if (sendto(s, &res, sizeof(res), 0, (struct sockaddr *)&remaddr, sizeof(remaddr)) != sizeof(res)) {
        error("sent a different number of bytes than expected");
      }
    }
  }

  return 0;
}

int main (int args, char *argv[]) {

  // port number to start server on, taken in as first commandline argument.
  short port;

  // didn't provide port number
  if (args < 2) {
    usage(argv[0]); // print usage information
    return -1;
  }

  port = atoi(argv[1]);

  // port specified was not a number
  if (port == 0) {
    usage(argv[0]); // print usage information
    return -2;
  }

  return server(port);
}
