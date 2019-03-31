#ifndef FILESINFOTABLEMODEL_H
#define FILESINFOTABLEMODEL_H

#include "DirectoryFilesWatcherListener.h"
#include <QAbstractTableModel>

class FilesInfoTableModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit FilesInfoTableModel(QObject *parent = nullptr);


public:
  QVariant headerData(int section, Qt::Orientation orientation,
					  int role = Qt::DisplayRole) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
				int role = Qt::DisplayRole) const override;

  void setDirectoryFilesWatcherListener( DirectoryFilesWatcherListener *dFWL);
  DirectoryFilesWatcherListener* getDirectoryFilesWatcherListener();

  void clearData();

 public slots:
  void fileAdded(const QString fileName, const quint64 fileSize,
				 const QString timeFileWrite);
  void fileRemoved(const QString fileName);
  void fileChanged(const QString fileName, const quint64 newFileSize,
				   const QString timeFileWrite);
  void fileRenamed(const QString oldFileName, const QString newFileName,
				   const QString timeFileWrite);

signals:
	// When files count changed, files added or removed
  void dataCountChanged(int newCount);

public:
  enum Column {
	  COLUMN_FILE_NAME = 0,
	  COLUMN_FILE_SIZE,
	  COLUMN_FILE_TIME,
	  COLUMN_COUNT
  };

private:
  struct FileInfo {
	QString fileName;
	QString timeFileCreateStr;
	std::uint64_t fileSize;
  };

  typedef QString FileName;
  typedef QHash<FileName, FileInfo> FilesInfo;
  typedef QVector<FileName> FileNameIndexes;

  FilesInfo                      _filesInfo;
  FileNameIndexes                _fileNameIndexes;
  DirectoryFilesWatcherListener* _directoryFilesWatcherListener;
};
#endif  // FILESINFOTABLEMODEL_H
