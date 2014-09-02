# client-server

A implementation of a file locking server using udp sockets.


## The Client

The client's job is to send commands to the server. The commands are stored in
a script file which is passed in as a command line argument. The client can be
called as follows:

    client HOST ID PORT FILE

where:

- HOST is the hostname/ip address of server
- ID is the id of client
- PORT is the port number of server
- FILE is the name of the script of commands to send to server


## The Server

The server's job is to listen for client commands and executes the file
commands. The server can be started as follows:

    server PORT

where

- PORT is port number of server to listen on


## File Commands

The commands are read in by the client and sent to the server.

Open a file a given mode (read, write or readwrite).

    open FILE

Close a file.

    close FILE

Read the next n bytes from a file.

    read FILE N

Write a string to a file.

    write FILE STRING

Set the position of the file pointer to N bytes from the beginning of the file.

    lseek FILE N


## Build

You can build both the client and the server with as follows:

    make 

or separately:

    make client
    make server


# Tests

To run tests:

    make test
