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
  printf("Usage: %s HOST ID PORT FILE\n", file);
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

  if (fd < 0) {
    printf("ERROR: unable to open file: %s", input.file);
    return -1;
  }

  char line[80];

  while (fscanf(fd, "%[^\n]\n", line) == 1) {
    printf("%s\n", line);
  }

  fclose(fd);

}
