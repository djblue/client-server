#ifndef REQUEST_H
#define REQUEST_H

struct request {

  char client_ip[16]; /* Holds client IP address in dotted decimal */
  char m[24]; /* Name of machine on which client is running */
  int c; /* Client number */
  int r; /* Request number of client */
  int i; /* Incarnation number of client’s machine */
  char operation[80]; /* File operation client sends to server */

};

#endif
