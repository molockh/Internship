#ifndef TASK_1_FACTORYMETHOD__FILE_WRITER_H_
#define TASK_1_FACTORYMETHOD__FILE_WRITER_H_

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

#include "file.h"

namespace fs = std::filesystem;

class FileWriter {
 public:
  explicit FileWriter(const fs::path& write_path);
  virtual void Write(const std::vector<File>& files) = 0;
  virtual ~FileWriter();

 protected:
  std::ofstream output_file_;
};

class TxtFileWriter : public FileWriter {
 public:
  explicit TxtFileWriter(const fs::path& write_path);

  void Write(const std::vector<File>& files) override;

 private:
  void WriteRow(const std::string& path,
                const std::string& last_write_time,
                const std::string& size,
                const std::string& permissions,
                int path_column_width);
};

class CsvFileWriter : public FileWriter {
 public:
  explicit CsvFileWriter(const fs::path& write_path);

  void Write(const std::vector<File>& files) override;

 private:
  void WriteRow(const std::string& path,
                const std::string& last_write_time,
                const std::string& size,
                const std::string& permissions);

  const char delimiter_ = ',';
};

class JsonFileWriter : public FileWriter {
 public:
  explicit JsonFileWriter(const fs::path& write_path);

  void Write(const std::vector<File>& files) override;

 private:
  void WriteFileDict(const std::string& path,
                     const std::string& last_write_time,
                     const std::string& size,
                     const std::string& permissions);

  const char open_list_bracket_ = '[';
  const char close_list_bracket_ = ']';

  const char open_dict_bracket_ = '{';
  const char close_dict_bracket_ = '}';

  const std::string space_ = "  ";
};

enum class FileWriterType {
  kTxt,
  kCsv,
  kJson
};

[[nodiscard]] std::unique_ptr<FileWriter> MakeWriter(FileWriterType type,
                                                     const fs::path& write_path);

void WriteToFile(FileWriter& file_writer, const std::vector<File>& files);

#endif //TASK_1_FACTORYMETHOD__FILE_WRITER_H_
