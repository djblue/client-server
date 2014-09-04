// client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// libraries for sockets
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

// application struct
#include "request.h"
#include "error.h"


// global for storing user input
struct Input {
  char machine_name[255];
  int id;
  char ip[15];
  int port;
  char file[1024];
} input;

void usage (char* file) {
  printf(
    "Usage: %s NAME ID IP PORT FILE\n\n"
    "  NAME - name of client\n"
    "  ID   - id of client\n"
    "  IP   - ip address of the server\n"
    "  PORT - port number of server\n"
    "  FILE - script of commands to send to server\n\n"
  , file);
}

int make_request (request *r) {

  // create socket
  int s;
  if ((s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    error("cannot create socket");
    return -1;
  }

  struct sockaddr_in server;
  memset((char *)&server, 0, sizeof(server));

  // server connection information
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(input.ip);
  server.sin_port = htons(input.port);

  // make request to server
  if (sendto(s, r, sizeof(*r), 0, (struct sockaddr *)&server, sizeof(server)) != sizeof(*r)) {
    error("sent a different number of bytes than expected");
    return -1;
  }

  return 0;
}

int main (int argc, char *argv[]) {

  if (argc < 6) {
    usage(argv[0]);
    return -1;
  }

  strcpy(input.machine_name, argv[1]);
  input.id = atoi(argv[2]);
  strcpy(input.ip, argv[3]);
  input.port = atoi(argv[4]);
  strcpy(input.file, argv[5]);

  FILE *fd = fopen(input.file, "r");

  if (fd == NULL) {
    error("unable to open file: %s", input.file);
    return -1;
  }

  int i = 0; // request count
  int spawn = 0;
  char line[80]; // line from script file

  while (fscanf(fd, "%[^\n]\n", line) == 1) {

    printf("EXECUTING(%d): %s\n", i, line);

    if (strcmp(line, "fail") == 0) {

      // on failure, increment incarnation number
      spawn += 1;

    } else {

      struct request r;

      r.id = input.id;
      r.index = i;
      r.spawn = spawn;

      strcpy(r.ip, "");
      strcpy(r.name, input.machine_name);

      strcpy(r.operation, line);

      make_request(&r);

      i += 1; // increment request count
    }
  }

  fclose(fd); // clean up

  return 0;
}
