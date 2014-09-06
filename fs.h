#ifndef FS_H
#define FS_H

#include <string.h>

#define MAX_CLIENTS 1024

// clients own files in filesystem

// struct to define clients
typedef struct {
  char name[24];  // m
  int id;         // c
  int index;      // r
  int spawn;      // i
} client;

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
  char filename[24];
} lock;

int is_locked (client *c, char *filename) {
}

int lock_file (client *c, char *filename) {
}

int unlock_file (client *c, char *filename) {
}


int open () {
}

int close () {
}

int read () {
}

int write () {
}

int lseek () {
}

#endif
