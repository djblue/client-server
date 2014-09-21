#ifndef FS_H
#define FS_H

#include <string.h>
#include <stdio.h>

#include <map>
#include <string>

using namespace std;

#include "client_info.h"

// struct to define which clients locked which files
typedef struct {
  int client; // who locked the file
  char name[80]; // file name m:file
  FILE *fd; // file descriptor
} lock;

// machine_name -> file_name -> lock
map<string, map<string, lock> > locks;

// check if a file is locked for a client
int is_locked (client *c, char *file) {
  return locks[string(c->name)].count(string(file));
}

int has_lock (client *c, char *file) {
  lock *l = &locks[string(c->name)].find(string(file))->second;
  return l->client == c->id;
}

// lock a file for a client
lock* lock_file (client *c, char *file) {

  lock *l = &locks[string(c->name)][string(file)];

  char name[80];
  // calculate name
  sprintf(name, "%s:%s", c->name, file);

  strncpy(l->name, name, 80);
  l->client = c->id; 

  return l;
}

lock* get_lock (client *c, char *file) {
  return &locks[string(c->name)][string(file)];
}

// unlock a file for for a client
int unlock_file (client *c, char *file) {
  locks[string(c->name)].erase(string(file));
}


// open/lock file for a machine if file is not already locked.
int file_open (client *c, char *file, char *mode) {

  // check if file is already locked by another client on the same machine
  if (is_locked(c, file) == 1) return -1;

  // open file for client in given mode

  lock *l = lock_file(c, file);

  if (strncmp("read", mode, 4) == 0) {
    l->fd = fopen(l->name, "r");
  } else if (strncmp("write", mode, 5) == 0) {
    l->fd = fopen(l->name, "w");
  } else if (strncmp("readwrite", mode, 9) == 0) {
    l->fd = fopen(l->name, "rw");
  } else {
    return -1;
  }
      
  return l->fd == NULL;
}

// close/unlock file for a machine, if there is a lock and they have the current
// lock
int file_close (client *c, char *file) {
  if (has_lock(c, file) != 1) return -1;
  lock *l = get_lock(c, file);
  int status = fclose(l->fd);
  unlock_file(c, file);
  return status;
}

// read next n bytes from a file into content buffer
int file_read (client *c, char *file, int n, char *content) {
  if (has_lock(c, file) != 1) return -1;
  lock *l = get_lock(c, file);
  return fread(content, 1, n, l->fd);
}

// write str to file
int file_write (client *c, char *file, char *str) {
  if (has_lock(c, file) != 1) return -1;
  lock *l = get_lock(c, file);
  //printf("%d\n", strlen(str));
  return fwrite(str, 1, strlen(str), l->fd);
}

// set the position of the file pointer to N bytes from the beginning of the
// file.
int file_lseek (client *c, char *file, int n) {
  if (has_lock(c, file) != 1) return -1;
  lock *l = get_lock(c, file);
  // SEEK_CUR - current position of file pointer
  return fseek(l->fd, n, SEEK_CUR);
}

#endif
