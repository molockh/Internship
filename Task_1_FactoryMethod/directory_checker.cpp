#include "directory_checker.h"

#include <chrono>
#include <ctime>
#include <sstream>
#include <utility>

DirectoryChecker::DirectoryChecker(fs::path directory_path)
    : directory_path_(std::move(directory_path)) {
  if (!fs::is_directory(directory_path_))
    throw std::runtime_error("Incorrect input directory");
}

std::vector<File> DirectoryChecker::GetFiles() const {
  std::vector<File> files;

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(directory_path_)) {
    File file = {
        entry.path().relative_path(),        // path
        GetFileLastWriteTime(entry.path()),  // last_write_time
        GetFilePermissions(entry.path()),    // permissions
        GetFileSize(entry.path()),           // size
        fs::is_directory(entry.path())       // is_directory
    };
    files.push_back(file);
  }

  return files;
}

size_t DirectoryChecker::GetFileSize(const fs::path& path) {
  if (is_directory(path)) return 0;
  return fs::file_size(path);
}

std::string DirectoryChecker::GetFileLastWriteTime(const fs::path& path) {
  auto ftime = fs::last_write_time(path);
  std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
  std::string last_write_time = std::asctime(std::localtime(&cftime));
  last_write_time.pop_back();

  return last_write_time;
}

std::string DirectoryChecker::GetFilePermissions(const fs::path& path) {
  std::stringstream ss;

  fs::perms p = status(path).permissions();
  ss << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
     << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
     << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
     << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
     << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
     << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
     << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
     << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
     << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-");

  return ss.str();
}
