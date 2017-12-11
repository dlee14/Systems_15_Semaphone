#include "header.h"

int open_file();
void get_line(int, int*);
void write_line(int, char *);

int main() {
  int fd, sem_id, shm_id;

  if ((sem_id = semget(SEM_KEY, 1, 0)) == -1 ) {
    printf("\nError: Story does not exist\n\n");
    exit(1);
  }

  printf("Waiting for the semaphone...\n");
  struct sembuf sbuf;
  sbuf.sem_num = 0;
  sbuf.sem_flg = SEM_UNDO;
  sbuf.sem_op = -1; // down down down
  semop(sem_id, &sbuf, 1);
  printf("Done waiting!\n\n");

  shm_id = shmget(SHM_KEY, sizeof(int), 0);
  int * size = shmat(shm_id, 0, 0);
  fd = open("story.txt", O_RDWR | O_APPEND);

  printf("Last input:\n");
  get_line(fd, size);

  char input[1024];
  printf("Next input?:\n");
  fgets(input, sizeof(input), stdin);
  write_line(fd, input);

  *size = strlen(input);
  shmdt(size);

  sbuf.sem_op = 1; //up up up
  semop(sem_id, &sbuf, 1);

  return 0;
}

void get_line(int fd, int * size) {
  char * line = (char *)calloc(1, *size + 1);
  lseek(fd, -1 * *size, SEEK_END);
  read(fd, line, *size);
  printf("\n%s\n", line);
  free(line);
}

int open_file() {
  int fd = open("story.txt", O_RDWR | O_APPEND);
  if (!fd) {
    exit(1);
  }
  return fd;
}

void write_line(int fd, char * line) {
  lseek(fd, 0, SEEK_END);
  write(fd, line, strlen(line));
}
