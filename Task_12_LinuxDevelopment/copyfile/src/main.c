#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "copyfile.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s input_file ouput_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char* input_path = argv[1];
  const char* output_path = argv[2];

  bool result = copyfile(input_path, output_path);

  if (!result) {
    fprintf(stderr, "Copy was not succesful :(");
    exit(EXIT_FAILURE);
  }
  puts("Success!");

  return 0;
}

