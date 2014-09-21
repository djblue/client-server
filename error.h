#ifndef ERROR_H
#define ERROR_H

#include "colors.h"

// error reporting macro
// NOTE: maximum error message length is 255
#define error(msg, ...) { \
  char buffer[255]; \
  sprintf(buffer, msg, ##__VA_ARGS__); \
  fprintf(stderr, __red("ERROR:")" %s in %s on line %d\n", buffer, __FILE__, __LINE__); \
  fflush(stderr); \
}

#endif
