#ifndef RESPONSE_H 
#define RESPONSE_H

#include "colors.h"

typedef struct {

  // status code of the request (from discussion board)
  int status;
  char content[80];

} response;

void prints(response *res) {

  if (res->status >= 0) {
    printf("res: { "
      "status: "__green("%d")", "
      "content: "__blue("%s")
      " }\n"
      ,
      res->status,
      res->content
    );
  } else {
    printf("res: { "
      "status: "__red("%d")", "
      "content: "__yellow("%s")
      " }\n"
      ,
      res->status,
      res->content
    );
  }
  fflush(stdout);
}

#endif
