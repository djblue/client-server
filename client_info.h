#ifndef CLIENT_INFO
#define CLIENT_INFO

// struct to define clients

typedef struct {

  // Name of machine on which client is running
  char name[24];  // m

  // Client id number
  int id;         // c

  // Request number of client (each request should get incremented by 1)
  int index;      // r

  // Incarnation number of client’s machine, for simulating a crash on the
  // clients end
  int spawn;      // i

} client;

#endif
