#include <stdbool.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>

bool copyfile(const char *input_file_path, const char *output_file_path) {
  int file_input = open(input_file_path, O_RDONLY);

  struct stat stat_buf;
  fstat(file_input, &stat_buf);

  int file_output = open(output_file_path, O_WRONLY | O_CREAT, stat_buf.st_mode);

  ssize_t size = sendfile(file_output, file_input, 0, stat_buf.st_size);

  close(file_input);
  close(file_output);

  if (size != -1) {
    return true;
  }
  return false;
}

