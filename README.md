# client-server

A implementation of a file locking server using udp sockets.


## The Client

The client's job is to send commands to the server. The commands are stored in
a script file which is passed in as a command line argument. The client can be
called as follows:

    client HOST ID IP PORT FILE

where:

- HOST is the host name of the client
- ID is the id of client
- IP is the ip address of the server
- PORT is the port number of server
- FILE is the name of the script of commands to send to server

NOTE: since the server has a 1:3 chance of failure, we have have a timeout with
a max of 100 retries.

## The Server

The server's job is to listen for client commands and executes the file
commands. The server can be started as follows:

    server PORT

where

- PORT is port number of server to listen on

## File Commands

The commands are read in by the client and sent to the server.

Open a file in a specific mode (read, write or readwrite).

    open FILE MODE

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

# Design and Analysis

Below are the main highlights of the implementation details and how they relate
to the assignment requirements.

## Structures

The two main structures are in `request.h` and `response.h`. They are the
structs that the server and client use to communicate to each other.

## Locking

The __client table__ and __lock table__ are implemented together using
`std::map`. The lock table is in `fs.h` because that is where are the file
operations are located. Each key in the lock table is in the following format:

    "name:file"

which maps to the client struct (located in client\_info.h) which is just a
subset of the request struct. The client struct contains all the information
about the client that owns the lock.

If an incoming client has a higher spawn (incarnation) number then the last
lock, we remove the old lock. This prevents ferment file locks.

## Error Simulation

When a server gets a request it generates a random number and between 0-2 which
maps to one of the following actions.

- __0__: execute file command
- __1__: execute but don't respond
- __2__: don't execute and don't respond

All paths have 1:3 chance of happening.

## Caching

Previously computed responses are stored in a cache map. The key for the cache
map is in the following format: "name:id:index". This way is we get the same
request, we can just return the previously computed response. This is
implemented in `server.c`.

## Spawn (incarnation)

To help simulate client machine failures, I use a spawn (incarnation) number.
The number is initially set to zero and in a file shared by all of the clients
on a single machine. If the fail command is read in from a script, we increment
the number in the file. This is useful because all clients read the number in
the file before sending it to the server. The filename of the number is  `spawn`
and is locked when read or written.

## Logging

Another main point was to make the output `grep-able` as all relevant is usually
printed on the same line. To get all of the information about client 1 while
running the tests, you can run something like:

  make test | grep 'id: 1'

The output format is all of the server logs, then all of the client logs
separated by dashes. Below is an example:

```
server logs
------------
client 1 logs
------------
client 2 logs
------------
```

## Conclusion

The project satisfies all of the requirements established in the assignment as
evidenced by the above.

A set of runs based on the sample input is provided as:

- logs/run1: a run of a single client and server both running on the same.
- logs/run2: a run of a single client and server each running on different
  machines.
- logs/run3: a run of at least two clients on the same machine and the server on
  a different machine.
