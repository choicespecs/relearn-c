#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
int main(int argc, char *argv[]) {
    sem_t *semaphore;
  if ((semaphore = sem_open("/semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  // do something with sem_wait(&semaphore)
  // do something with sem_post(&semaphore)
  if (sem_close(semaphore) == -1) {
    perror("sem_close");
    exit(EXIT_FAILURE);
  }
  if (sem_unlink("/semaphore") == -1) {
    perror("sem_unlink");
    exit(EXIT_FAILURE);
  }
  puts("Done");
  exit(EXIT_SUCCESS);
}
// Output: Done
