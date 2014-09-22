// stateful file locking server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// libraries for sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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
void dispatch (request *req, response *res, char *log) {

  char buffer[80];
  sprintf(buffer, "%s:%d:%d", req->name, req->id, req->index);

  if (cache.count(string(buffer)) > 0) {
    response *old = &cache[string(buffer)];
    res->status = old->status;
    strcpy(res->content, old->content);
    strcpy(log, "res from cache");
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
      strcpy(log, "cannot open file");
    } else {
      strcpy(res->content, "ok - file opened");
      strcpy(log, "ok - file opened");
    }

  } else if (strncmp("close", cmd, 5) == 0) {
    status = file_close(c, file);
    if (status == 0) {
      strcpy(res->content, "ok - file closed");
      strcpy(log, "ok - file closed");
    }

  } else if (strncmp("read", cmd, 4) == 0) {
    status = file_read(c, file, atoi(arg), res->content);

  } else if (strncmp("write", cmd, 5) == 0) {
    arg = strtok(arg, "\""); // get character from inside string
    status = file_write(c, file, arg);
    if (status == strlen(arg)) {
      strcpy(res->content, "ok - file write");
      strcpy(log, "ok - file write");
    } else {
      strcpy(res->content, "not ok");
      strcpy(log, "not ok");
    }

  } else if (strncmp("lseek", cmd, 5) == 0) {
    status = file_lseek(c, file, atoi(arg));
    if (status == 0) {
      strcpy(res->content, "ok - file seek");
      strcpy(log, "ok - file seek");
    }

  } else {
    status = 500;
    strcpy(res->content, "unknown file operation.");
    strcpy(log, "unknown file operation.");
  }

  if (status == -1) {
    strcpy(res->content, "file locked");
    strcpy(log, "file locked");
  }
  res->status = status;
  cache[string(buffer)] = *res;
}

void log (request *req, char *action, char *msg) {
  printf("{ ip: %s, name: %s, id: %d, index: %d, exec: %s, action: %s, log: "__purple("%s")" }\n",
    req->ip, req->name, req->id, req->index, req->operation, action, msg);
}

// port - desired port number
// returns if server process was successful
int server (short port) {
  
  // setup a socket
  int s; // socket connection
  int random; // random number to simulate errors
  char buff[256];
  struct sockaddr_in myaddr;
  struct sockaddr_in remaddr;     // remote address
  socklen_t addrlen = sizeof(remaddr); // length of addresses
  int recvlen;                    // # bytes received
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

  printf("server listening on port "__green("%d\n"), port);
  fflush(stdout); // display message before entering loop

  // wait for connections
  while (1 == 1) {

    recvlen = recvfrom(s, &req, sizeof(req), 0, (struct sockaddr *)&remaddr, &addrlen);

    strcpy(req.ip, inet_ntoa(remaddr.sin_addr));

    if (recvlen > 0) {

      response res;
      memset(&res, 0, sizeof(res));

      random = rand() % 3;

      // switch to simulate errors
      switch(random) {
        
        // execute file command and respond
        case 2:
          dispatch(&req, &res, buff);
          log(&req, __blue("respond"), buff);
          if (sendto(s, &res, sizeof(res), 0, (struct sockaddr *)&remaddr, sizeof(remaddr)) != sizeof(res)) {
            error("sent a different number of bytes than expected");
          }
          break;
      
        // execute but don't respond
        case 1:
          dispatch(&req, &res, buff);
          log(&req, __yellow("exec + drop"), buff);
          break;

        // don't execute and don't respond
        case 0:
          strcpy(buff, "");
          log(&req, __red("drop"), buff);
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
