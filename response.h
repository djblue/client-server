#ifndef RESPONSE_H 
#define RESPONSE_H

typedef struct {

  // status code of the request (from discussion board)
  int status;
  char content[80];

} response;

void prints(response *res) {
  printf("res: {"
    "\"status\": %d, "
    "\"content\": \"%s\""
    "}\n"
    ,
    res->status,
    res->content
  );
  fflush(stdout);
}

#endif
