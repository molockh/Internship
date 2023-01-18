#ifndef TASK_1_FACTORYMETHOD__FILE_H_
#define TASK_1_FACTORYMETHOD__FILE_H_

#include <string>

struct File {
  std::string path;
  std::string last_write_time;
  std::string permissions;
  size_t size = 0;
  bool is_directory = false;
};

#endif //TASK_1_FACTORYMETHOD__FILE_H_
