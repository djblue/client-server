// client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// libraries for sockets
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

// application struct
#include "request.h"
#include "response.h"
#include "error.h"

int request_count = 0; // request count

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
    "  NAME - machine name of client\n"
    "  ID   - id of client\n"
    "  IP   - ip address of the server\n"
    "  PORT - port number of server\n"
    "  FILE - script of commands to send to server\n\n"
  , file);
}

#include <fcntl.h>

int set_spawn (int spawn) {

  int fd, result;

  struct flock fl = { F_WRLCK, SEEK_SET, 0, 0, 0 };
  fl.l_pid    = getpid();

  if ((fd = open("spawn", O_RDWR)) == -1) {
    error("opening spawn file");
  }

  if (fcntl(fd, F_SETLKW, &fl) == -1) {
    error("locking file");
  }

  result = write(fd, &spawn, 4);

  fl.l_type   = F_UNLCK; // tell it to unlock the region
  if (fcntl(fd, F_SETLK, &fl) == -1) {
    error("unlocking file");
  }

  close(fd);

  return result;
}

int get_spawn () {

  int fd, spawn = -1;

  struct flock fl = { F_RDLCK, SEEK_SET, 0, 0, 0 };
  fl.l_pid = getpid();

  if ((fd = open("spawn", O_RDWR)) == -1) {
    error("opening spawn file");
  }

  if (fcntl(fd, F_SETLKW, &fl) == -1) {
    error("locking spawn file");
  }

  read(fd, &spawn, 4);

  fl.l_type = F_UNLCK;
  if (fcntl(fd, F_SETLK, &fl) == -1) {
    error("unlocking file");
  }
  close(fd);

  return spawn;
}

int init_spawn () {
  if (access("spawn", F_OK ) == -1) {
    int val = 0;
    FILE *f = fopen("spawn", "w");
    fwrite(&val, 1, 4, f);
  }
}

#include <unistd.h>

int make_request (request *req) {

  struct sockaddr_in remaddr;     /* remote address */
  socklen_t addrlen = sizeof(remaddr); // length of addresses

  // create socket
  int s;

  // bind to socket
  if ((s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    error("cannot create socket");
    return -1;
  }

  // set socket timeout period
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 100000;
  if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
    error("cannot set timeout");
  }

  struct sockaddr_in server;
  memset((char *)&server, 0, sizeof(server));

  // server connection information
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(input.ip);
  server.sin_port = htons(input.port);

  // make request to server
  int count = 0, recvlen;
  response res;
  memset(&res, 0, sizeof(res));
  res.status = -200; // arbitrary number that means no successful response

  req->index = request_count++;

  do {
    res.status = 0;

    if (sendto(s, req, sizeof(*req), 0, (struct sockaddr *)&server, sizeof(server)) != sizeof(*req)) {
      error("sent a different number of bytes than expected");
      continue;
    } else {
      recvlen = recvfrom(s, &res, sizeof(res), 0, (struct sockaddr *)&remaddr, &addrlen);
      count += 1;

      printq(req);
      if (recvlen < 0) {
        printf("res: { "__red("timeout")" - no response }\n");
      } else {
        prints(&res);
      }

      if (res.status == -1) {
        recvlen = -1;
        req->index = request_count++;
      }
    }


  } while (recvlen < 0 && count < 100);

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

  char line[80]; // line from script file
  init_spawn();

  while (fscanf(fd, "%[^\n]\n", line) == 1) {

    //printf("EXECUTING(%d): %s\n", i, line);

    if (strcmp(line, "fail") == 0) {

      set_spawn(get_spawn() + 1);

    } else {

      request req;

      req.id = input.id;
      req.spawn = get_spawn();

      strcpy(req.ip, "");
      strcpy(req.name, input.machine_name);

      strcpy(req.operation, line);

      make_request(&req);
    }
  }

  fclose(fd); // clean up

  return 0;
}
