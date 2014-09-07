#ifndef FS_H
#define FS_H

#include <string.h>
#include "client_info.h"

#define MAX_CLIENTS 1024

// clients own files in filesystem

// struct to define a table of clients
typedef struct {
  client list[MAX_CLIENTS];
  int length;
} clients;

// get client by name
client *get_client (clients *self, char *name) {
  int i;
  for (i = 0; i < self->length; i++) {
    if (strcmp(self->list[i].name, name) == 0) {
      return  &self->list[i];
    }
  }

  return NULL;
}

// add a new client
int add_client (clients *self, client *c) {

  int i = self->length;

  if (i > MAX_CLIENTS) return 0;

  strcpy(self->list[i].name, c->name);
  self->list[i].id     = c->id;
  self->list[i].index  = c->index;
  self->list[i].spawn  = c->spawn;

  self->length += 1; // move to next spot

  return 1;
}

// struct to define which clients locked which files
typedef struct {
  int locked; // is the file locked
  int client; // who locked the file
  char file[24];
} lock;

// check if a file is locked for a client
int is_locked (client *c, char *file) {
}

// lock a file for a client
int lock_file (client *c, char *file) {
}

// unlock a file for for a client
int unlock_file (client *c, char *file) {
}


// open/lock file for a machine if file is not already locked.
int file_open (client *c, char *file, char *mode) {
  return 0;
}

// close/unlock file for a machine, if there is a lock and they have the current
// lock
int file_close (client *c, char *file) {
  return 0;
}

// read next n bytes from a file into content buffer
int file_read (client *c, char *file, int n, char *content) {
  return 0;
}

// write str to file
int file_write (client *c, char *file, char *str) {
  return 0;
}

// set the position of the file pointer to N bytes from the beginning of the
// file.
int file_lseek (client *c, char *file, int n) {
  return 0;
}

#endif
