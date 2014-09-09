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
  int locked; // is the file locked
  int client; // who locked the file
  char file[24];
} lock;

// machine_name -> file_name -> lock
map<string, map<string, lock> > locks;

// check if a file is locked for a client
int is_locked (client *c, char *file) {
  return locks[string(c->name)].count(string(file));
}

// lock a file for a client
int lock_file (client *c, char *file) {
}

// unlock a file for for a client
int unlock_file (client *c, char *file) {
}


// open/lock file for a machine if file is not already locked.
int file_open (client *c, char *file, char *mode) {

  // check if file is already locked by another client on the same machine
  if (is_locked(c, file) == 1) {
    return -1;

  // open file for client in given mode
  } else {
  }

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
