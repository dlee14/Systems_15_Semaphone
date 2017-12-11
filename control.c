#include "header.h"

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
  sid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0664);
  if (sid > 0) {
    printf("Creating new semaphore...\n");
    printf("\t[id]: %d\n", sid);
    union semun su;
    su.val = 1;
    su.val = semctl(sid, 0, SETVAL, su);

    sid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0644);
    printf("Creating a new file...\n");
    int f = open("story.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  } else {
    sid = semget(SEM_KEY, 1, IPC_CREAT);
    printf("Error: Semaphore already exists:\n");
    printf("\t[id]: %d\n", sid);
  }
}

char * get_story() {
  int fd;
  struct stat story;
  stat("story.txt", &story);
  fd = open("story.txt", O_RDONLY);
  if (fd) {
    int size = story.st_size;
    char * s = malloc(size);
    read(fd, s, size);
    return s;
  } else {
    printf("\nError: Story does not exist\n\n");
    exit(1);
  }
}

void view() {
  char * story = get_story();
  printf("\nSTORY:\n\n%s\n", story);
  free(story);
}

void remove_sem() {
  int sid, val;
  sid = semget(SEM_KEY, 1, 0664);
  if (sid != -1) {
    printf("Deleting semaphore...\n");
    view();
    remove("story.txt");
    semctl(sid, 0, IPC_RMID);
  } else {
    printf("Error: Semaphore does not exist\n\n");
  }
}
