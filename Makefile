# variables
CC=g++


all: client server

# build server component
server: server.c;  $(CC) server.c -o server

# build client component
client: request.h error.h client.c; $(CC) client.c -o client

# run tests
test: ; ls test/*.sh | xargs -d'\n' -I{} sh {}

# clean up directory
clean: ; rm -rf server client

.PHONY: test clean
