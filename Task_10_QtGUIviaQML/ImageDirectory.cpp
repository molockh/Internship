#include "ImageDirectory.h"

#include <QDir>
#include <QStringList>
#include <iostream>

QString ImageDirectory::directory_path() const {
  return directory_path_;
}

QStringList ImageDirectory::image_list() const {
  return image_list_;
}

void ImageDirectory::setDirectoryPath(const QString& path) {
  QString trimmed = path.mid(7);
  directory_path_ = path;

  image_list_.clear();
  QDir directory(trimmed);
  QStringList images = directory.entryList(QStringList() << "*.png" << "*.jpg" << "jpeg",QDir::Files);
  for (const QString& filename : images) {
      image_list_.append(path + "/" + filename);
      std::cout << "Appending " << (path + "/" + filename).toStdString() << std::endl;
  }


  emit imageListChanged();
}
