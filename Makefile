# variables
CC=gcc

all: client server

# build server component
server: request.h response.h error.h fs.h server.c;  $(CC) server.c -o server

# build client component
client: request.h response.h error.h client.c; $(CC) client.c -o client

# run tests
test: ; ls test/*.sh | xargs -d'\n' -I{} sh {}

# clean up directory
clean: ; rm -rf server client

.PHONY: test clean
