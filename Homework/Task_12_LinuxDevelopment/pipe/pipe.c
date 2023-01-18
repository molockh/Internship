#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFSIZE 4096

int main(void) {
  int pipefds[2], pid;
  char buffer[BUFFSIZE];

  pipe(pipefds);
  pid = fork();

  if (pid == -1) {
    fprintf(stderr, "Fork error!");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    const char* str = "Hi daddy, I'm your child!";

    close(pipefds[0]);
    write(pipefds[1], str, strlen(str) + 1);
    close(pipefds[1]);
  } else {
    close(pipefds[1]);

    read(pipefds[0], buffer, BUFFSIZE);
    strcat(buffer, " Hi!\n");

    close(pipefds[0]);

    int output_file = open("result.txt", O_WRONLY | O_CREAT);
    write(output_file, buffer, strlen(buffer));

    close(output_file);
  }

  return EXIT_SUCCESS;
}
