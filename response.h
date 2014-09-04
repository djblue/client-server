#ifndef RESPONSE_H 
#define RESPONSE_H

struct response {

  // status code of the request (from discussion board)
  int status;

};

void print(response *res) {
  printf("res: {"
    "\"status\": %d"
    "}\n"
    ,
    res->status
  );
  fflush(stdout);
}

#endif
