#include <iostream>

class FileGuard {
 public:
  FileGuard(FILE* file) : file_(file) {}
  ~FileGuard() {
    if (file_ != nullptr) {
      fclose(file_);
    }
  }
  FileGuard(FileGuard&& file_guard) {
    file_ = file_guard.file_;
    file_guard.file_ = nullptr;
  }
  FileGuard& operator=(FileGuard&& file_guard) {
    if (file_ != nullptr) {
      fclose(file_);
    }
    file_ = file_guard.file_;
    file_guard.file_ = nullptr;

    return *this;
  }

  FileGuard(const FileGuard& file_guard) = delete;
  FileGuard& operator=(const FileGuard& file_guard) = delete;

 private:
  FILE* file_ = nullptr;
};

int main() {
  FileGuard file = fopen("Makefile", "w");

  return 0;
}
