// client

#include <stdio.h>
#include <stdlib.h>

// libraries for sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// application struct
#include "request.h"

#include <string.h> /* for memcpy */

void usage (char* file) {
  printf(
    "Usage: %s HOST ID PORT FILE\n\n"
    "  HOST - hostname of server\n"
    "  ID   - id of client\n"
    "  PORT - port number of server\n"
    "  FILE - script of commands to send to server\n\n"
  , file);
}

struct Input {
  char host[255];
  int id;
  int port;
  char file[1024];
};

int main (int argc, char *argv[]) {

  if (argc < 5) {
    usage(argv[0]);
    return -1;
  }

  struct Input input;

  strcpy(input.host, argv[1]);
  input.id = atoi(argv[2]);
  input.port = atoi(argv[3]);
  strcpy(input.file, argv[4]);

  FILE *fd = fopen(input.file, "r");

  if (fd == NULL) {
    printf("ERROR: unable to open file: %s\n", input.file);
    return -1;
  }

  int i = 0; // request count
  int spawn = 0;
  char line[80]; // line from script file

  while (fscanf(fd, "%[^\n]\n", line) == 1) {

    printf("Executing(%d): %s\n", i, line);

    if (strcmp(line, "fail") == 0) {

      // on failure, increment incarnation number
      spawn += 1;

    } else {

      struct request r;

      r.id = input.id;
      r.index = i;
      r.spawn = spawn;

      strcpy(r.ip, "0.0.0.0");
      strcpy(r.name, "host");
      strcpy(r.operation, line);

      i += 1; // increment request count
    }
  }

  fclose(fd); // clean up

  return 0;
}
