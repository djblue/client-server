#ifndef REQUEST_H
#define REQUEST_H

#include "client_info.h"

// The names of the fields differ from the struct specific in the assignment,
// but the positions of the elements are the same, so this struct should still
// work for other teams.

typedef struct {

  // Holds client IP address in dotted decimal
  char ip[16]; // client ip

  client c;

  // File operation client sends to server
  char operation[80];

} request;

void printq(request *req) {
  printf("req: {"
    "\"ip\": \"%s\", "
    "\"name\": \"%s\", "
    "\"id\": %d, "
    "\"index\": %d, "
    "\"spawn\": %d, "
    "\"operation\": \"%s\""
    "}\n"
    ,
    req->ip,
    req->c.name,
    req->c.id,
    req->c.index,
    req->c.spawn,
    req->operation
  );
  fflush(stdout);
}

#endif
