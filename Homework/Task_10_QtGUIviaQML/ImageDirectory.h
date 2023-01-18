#ifndef IMAGEDIRECTORY_H
#define IMAGEDIRECTORY_H

#include <QObject>
#include <QDate>
#include <QVector>
#include <QString>
#include <QStringList>

class ImageDirectory : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString directory_path READ directory_path WRITE setDirectoryPath NOTIFY directoryPathChanged)
  Q_PROPERTY(QStringList image_list READ image_list NOTIFY imageListChanged)

public:
  QString directory_path() const;
  void setDirectoryPath(const QString& path);

  QStringList image_list() const;

 signals:
  void directoryPathChanged();
  void imageListChanged();

private:
  QString directory_path_;
  QStringList image_list_;
};

#endif // IMAGEDIRECTORY_H
