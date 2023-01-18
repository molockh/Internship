#include "file_writer.h"

#include <fstream>
#include <iostream>
#include <utility>

FileWriter::FileWriter(const fs::path& write_path) : output_file_(write_path) {
  if (!output_file_)
    throw std::runtime_error("Error while opening " +
        static_cast<std::string>(write_path));
}

FileWriter::~FileWriter() {
  if (output_file_.is_open()) output_file_.close();
}

TxtFileWriter::TxtFileWriter(const fs::path& write_path)
    : FileWriter(write_path) {}

void TxtFileWriter::WriteRow(const std::string& path,
                             const std::string& last_write_time,
                             const std::string& size,
                             const std::string& permissions,
                             int path_column_width) {
  output_file_ << std::setw(path_column_width) << std::left << path
               << std::setw(28) << std::left << last_write_time << std::setw(12)
               << std::left << size << std::setw(12) << std::left << permissions
               << std::endl;
}

void TxtFileWriter::Write(const std::vector<File>& files) {
  if (files.empty()) return;

  auto longest_path_it = max_element(std::begin(files), std::end(files),
                                     [](const File& lhs, const File& rhs) {
                                       return lhs.path.size() < rhs.path.size();
                                     });
  int path_column_width = static_cast<int>(longest_path_it->path.size()) + 5;

  WriteRow("Path", "Last Write Time", "Size", "Permissions", path_column_width);

  for (const auto& file : files) {
    WriteRow(file.path, file.last_write_time, std::to_string(file.size),
             file.permissions, path_column_width);
  }
}

CsvFileWriter::CsvFileWriter(const fs::path& write_path)
    : FileWriter(write_path) {}

void CsvFileWriter::WriteRow(const std::string& path,
                             const std::string& last_write_time,
                             const std::string& size,
                             const std::string& permissions) {
  output_file_ << path << delimiter_ << last_write_time << delimiter_ << size
               << delimiter_ << permissions << std::endl;
}

void CsvFileWriter::Write(const std::vector<File>& files) {
  if (files.empty()) return;

  WriteRow("Path", "Last Write Time", "Size", "Permissions");

  for (const auto& file : files) {
    std::string size;
    if (file.is_directory)
      size = "-||-";
    else
      size = std::to_string(file.size) + " bytes";

    WriteRow(file.path, file.last_write_time, size, file.permissions);
  }

  output_file_.close();
}

JsonFileWriter::JsonFileWriter(const fs::path& write_path)
    : FileWriter(write_path) {}

void JsonFileWriter::WriteFileDict(const std::string& path,
                                   const std::string& last_write_time,
                                   const std::string& size,
                                   const std::string& permissions) {

  output_file_ << space_ << space_ << open_dict_bracket_ << std::endl
               << space_ << space_ << space_ << R"("Path": ")" << path
               << "\",\n"
               << space_ << space_ << space_ << R"("LastWriteTime": ")"
               << last_write_time << "\",\n"
               << space_ << space_ << space_ << R"("Size": ")" << size
               << "\",\n"
               << space_ << space_ << space_ << R"("Permissions": ")"
               << permissions << "\"" << close_dict_bracket_;
}

void JsonFileWriter::Write(const std::vector<File>& files) {
  if (files.empty()) return;

  output_file_ << open_dict_bracket_ << std::endl;
  output_file_ << space_ << R"("Files": )" << open_list_bracket_ << std::endl;

  for (const auto& file : files) {
    std::string size;
    if (file.is_directory)
      size = "-||-";
    else
      size = std::to_string(file.size) + " bytes";

    WriteFileDict(file.path, file.last_write_time, size, file.permissions);

    if (&file != &files.back()) output_file_ << ",";
    output_file_ << std::endl;
  }
  output_file_ << space_ << close_list_bracket_ << std::endl;

  output_file_ << close_dict_bracket_;
}

[[nodiscard]] std::unique_ptr<FileWriter> MakeWriter(FileWriterType type,
                                                     const fs::path& write_path) {
  switch (type) {
    case FileWriterType::kTxt:return std::make_unique<TxtFileWriter>(write_path);
    case FileWriterType::kCsv:return std::make_unique<CsvFileWriter>(write_path);
    case FileWriterType::kJson:
      return std::make_unique<JsonFileWriter>(write_path);
  }
  throw std::runtime_error("Unknown FileWriterType");
}

void WriteToFile(FileWriter& file_writer, const std::vector<File>& files) {
  file_writer.Write(files);
}