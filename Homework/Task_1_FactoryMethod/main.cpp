#include <iostream>

#include "directory_checker.h"
#include "file_writer.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr
        << "Program should have 1 argument - path to the directory.\n"
        << "Example: " << argv[0] << " /Users/user/Documents"
        << std::endl;
    return EXIT_FAILURE;
  }

  try {
    std::string directory_path = argv[1];

    DirectoryChecker dir_check(directory_path);
    auto files = dir_check.GetFiles();

    std::unique_ptr<FileWriter>
        txt_file_writer = MakeWriter(FileWriterType::kTxt, "result.txt");
    std::unique_ptr<FileWriter>
        csv_file_writer = MakeWriter(FileWriterType::kCsv, "result.csv");
    std::unique_ptr<FileWriter>
        json_file_writer = MakeWriter(FileWriterType::kJson, "result.json");

    WriteToFile(*txt_file_writer, files);
    WriteToFile(*csv_file_writer, files);
    WriteToFile(*json_file_writer, files);
  }
  catch (const std::exception& exception) {
    std::cerr << exception.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (...) {
    std::cerr << "Unknown error" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
