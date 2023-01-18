#ifndef TASK_1_FACTORYMETHOD__DIRECTORY_CHECKER_H_
#define TASK_1_FACTORYMETHOD__DIRECTORY_CHECKER_H_

#include <string>
#include <vector>
#include <filesystem>

#include "file.h"

namespace fs = std::filesystem;

class DirectoryChecker {
 public:
  explicit DirectoryChecker(fs::path directory_path);
  [[nodiscard]] std::vector<File> GetFiles() const;

 private:
  fs::path directory_path_;

  static size_t GetFileSize(const fs::path& path);
  static std::string GetFileLastWriteTime(const fs::path& path);
  static std::string GetFilePermissions(const fs::path& path);
};

#endif //TASK_1_FACTORYMETHOD__DIRECTORY_CHECKER_H_
