// text color decoration macros
#define __blue(str)   "\033[0;34m"str"\033[0;0m"
#define __yellow(str) "\033[0;33m"str"\033[0;0m"
#define __green(str)  "\033[0;32m"str"\033[0;0m"
#define __red(str)    "\033[0;31m"str"\033[0;0m"
#define __purple(str) "\033[0;35m"str"\033[0;0m"

// error reporting macro
// NOTE: maximum error message length is 255
#define error(msg, ...) { \
  char buffer[255]; \
  sprintf(buffer, msg, ##__VA_ARGS__); \
  fprintf(stderr, __red("ERROR:")" %s in %s on line %d\n", buffer, __FILE__, __LINE__); \
}
