#ifndef REQUEST_H
#define REQUEST_H

// The names of the fields differ from the struct specific in the assignment,
// but the positions of the elements are the same, so this struct should still
// work for other teams.

typedef struct {

  // Holds client IP address in dotted decimal
  char ip[16]; // client ip

  // Name of machine on which client is running
  char name[24];  // m

  // Client id number
  int id;         // c

  // Request number of client (each request should get incremented by 1)
  int index;      // r

  // Incarnation number of client’s machine, for simulating a crash on the
  // clients end
  int spawn;      // i

  // File operation client sends to server
  char operation[80];

} request;

void printq(request *req) {
  printf("req: ("
    "ip: %s, "
    "name: %s, "
    "id: %d, "
    "index: %d, "
    "spawn: %d, "
    "operation: %s"
    ")\n"
    ,
    req->ip,
    req->name,
    req->id,
    req->index,
    req->spawn,
    req->operation
  );
  fflush(stdout);
}

#endif
