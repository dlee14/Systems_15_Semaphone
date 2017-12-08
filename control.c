#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define KEY 111

void create_sem();
void view();
void remove_sem();

int main(int argc, char const *argv[]) {
  if (argv[1] != NULL) {
    if (!strcmp(argv[1], "-c")) {
      create_sem();
    }
    else if (!strcmp(argv[1], "-v")) {
      view();
    }
    else if (!strcmp(argv[1], "-r")) {
      remove_sem();
    }
  } else {
    printf("Error: You don't know what you're doing. Or you made a mistake. I'm sorry, that was really mean.\n\n");
  }
  return 0;
}

void create_sem() {
  int sid;
  sid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0664);
  if (sid > 0) {
    semctl(sid, 0, SETVAL, 0);
    printf("Creating new semaphore:\n");
    printf("\t[id]: %d\n", sid);
    printf("Creating a new file\n");
    int f = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  } else {
    sid = semget(KEY, 1, IPC_CREAT);
    printf("Error: Semaphore already exists:\n");
    printf("\t[id]: %d\n", sid);
  }
}

void view() {
  FILE *file = fopen("file.txt", "rw");
  char str[1000];
  if (file) {
    fscanf(file, "%[^\n]", str);
  }
  printf("Story: \n%s\n", str);
  fclose(file);
}

void remove_sem() {
  int sid, val;
  sid = semget(KEY, 1, 0664);
  if (sid != -1) {
    printf("Deleting semaphore\n");
    FILE *file = fopen("file.txt", "rw");
    char str[1000];
    if (file) {
      fscanf(file, "%[^\n]", str);
    }
    printf("Story: \n%s\n", str);
    fclose(file);
    remove("file.txt");
    semctl(sid, 0, IPC_RMID);
  } else {
    printf("Error: No semaphore found\n\n");
  }
}
