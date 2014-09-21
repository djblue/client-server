// stateful file locking client 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// libraries for sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include <map>

// application struct
#include "fs.h"
#include "client_info.h"
#include "request.h"
#include "response.h"
#include "error.h"

void usage (char* file) {
  printf(
    "Usage: %s PORT\n\n"
    "  PORT - port number of server\n\n"
  , file);
}

using namespace std;

// machine -> client id -> client
map<string, map<int, client> > clients;

map<string,response> cache;

// parse operation command and dispatch file operations
void dispatch (request *req, response *res) {

  char buffer[80];
  sprintf(buffer, "%s:%d:%d", req->name, req->id, req->index);

  if (cache.count(string(buffer)) > 0) {
    response *old = &cache[string(buffer)];
    res->status = old->status;
    strcpy(res->content, old->content);
    return;
  }

  int status;

  client *c = (client*) &req->name;
  
  // parse operation
  char *cmd = strtok(req->operation, " ");
  char *file = strtok(NULL, " ");
  char *arg = strtok(NULL, " ");


  if (strncmp("open", cmd, 4) == 0) {
    status = file_open(c, file, arg);
    if (status == 1) {
      strcpy(res->content, "cannot open file");
    } else {
      strcpy(res->content, "ok - file opened");
    }

  } else if (strncmp("close", cmd, 5) == 0) {
    status = file_close(c, file);
    if (status == 0) {
      strcpy(res->content, "ok - file closed");
    }

  } else if (strncmp("read", cmd, 4) == 0) {
    status = file_read(c, file, atoi(arg), res->content);

  } else if (strncmp("write", cmd, 5) == 0) {
    arg = strtok(arg, "\""); // get character from inside string
    status = file_write(c, file, arg);
    if (status == strlen(arg)) {
      strcpy(res->content, "ok - file write");
    } else {
      strcpy(res->content, "not ok");
    }

  } else if (strncmp("lseek", cmd, 5) == 0) {
    status = file_lseek(c, file, atoi(arg));
    if (status == 0) {
      strcpy(res->content, "ok - file seek");
    }

  } else {
    status = 500;
    strcpy(res->content, "unknown file operation.");
  }

  if (status == -1) {
    strcpy(res->content, "file locked");
  }
  res->status = status;
  cache[string(buffer)] = *res;
}

// port - desired port number
// returns if server process was successful
int server (short port) {
  
  // setup a socket
  int s; // socket connection
  int random; // random number to simulate errors
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

      response res;
      memset(&res, 0, sizeof(res));

      random = rand() % 3;
      //printf("%d\n", random);

      // switch to simulate errors
      switch(random) {
        
        // execute file command and respond
        case 2:  //printf("execute file command and respond\n");
          dispatch(&req, &res);
          if (sendto(s, &res, sizeof(res), 0, (struct sockaddr *)&remaddr, sizeof(remaddr)) != sizeof(res)) {
            error("sent a different number of bytes than expected");
          }
          break;
      
        // execute but don't respond
        case 1: //printf("execute but don't respond\n");
          dispatch(&req, &res);
          break;

        // don't execute and don't respond
        case 0: //printf("don't execute and don't respond\n");
          break;
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

  srand(time(NULL));

  return server(port);
}
