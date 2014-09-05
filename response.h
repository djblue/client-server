#ifndef RESPONSE_H 
#define RESPONSE_H

typedef struct {

  // status code of the request (from discussion board)
  int status;

} response;

void prints(response *res) {
  printf("res: {"
    "\"status\": %d"
    "}\n"
    ,
    res->status
  );
  fflush(stdout);
}

#endif
