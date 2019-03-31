#include <iostream>
#include <QSize>
#include "FilesInfoTableModel.h"

FilesInfoTableModel::FilesInfoTableModel(QObject *parent)
	: QAbstractTableModel(parent) {}


QVariant FilesInfoTableModel::headerData(int section,
										 Qt::Orientation orientation,
										 int role) const {

  if ((role != Qt::DisplayRole) || (orientation != Qt::Horizontal)) {
	return QVariant();
  }
  switch (section) {
	case COLUMN_FILE_NAME:
	  return tr("File");
	case COLUMN_FILE_SIZE:
	  return tr("Size");
	case COLUMN_FILE_TIME:
	  return tr("Create time");
  }

  return QVariant();
}

int FilesInfoTableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return _filesInfo.size();
}

int FilesInfoTableModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return COLUMN_COUNT;
}

QVariant FilesInfoTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || _filesInfo.count() <= index.row() ||
	  (role != Qt::DisplayRole)) {
	return QVariant();
  }
  switch (index.column()) {
	case COLUMN_FILE_NAME:
	  return _filesInfo[_fileNameIndexes.at(index.row())].fileName;
	case COLUMN_FILE_SIZE:
	  return _filesInfo[_fileNameIndexes.at(index.row())].fileSize;
	case COLUMN_FILE_TIME:
	  return _filesInfo[_fileNameIndexes.at(index.row())].timeFileCreateStr;
  }
  return QVariant();
}

void FilesInfoTableModel::setDirectoryFilesWatcherListener(
	DirectoryFilesWatcherListener *directoryFilesWatcherListener) {
	_directoryFilesWatcherListener = directoryFilesWatcherListener;
	_directoryFilesWatcherListener->setParent(this);
	// Conected signals from listener with slots from model
	connect(_directoryFilesWatcherListener, &DirectoryFilesWatcherListener::fileAddedSignal,
			this, &FilesInfoTableModel::fileAdded, Qt::QueuedConnection);
	connect(_directoryFilesWatcherListener, &DirectoryFilesWatcherListener::fileRemovedSignal,
			this, &FilesInfoTableModel::fileRemoved, Qt::QueuedConnection);
	connect(_directoryFilesWatcherListener, &DirectoryFilesWatcherListener::fileRenamedSignal,
			this, &FilesInfoTableModel::fileRenamed, Qt::QueuedConnection);
	connect(_directoryFilesWatcherListener, &DirectoryFilesWatcherListener::fileChangedSignal,
			this, &FilesInfoTableModel::fileChanged, Qt::QueuedConnection);
}

DirectoryFilesWatcherListener *FilesInfoTableModel::getDirectoryFilesWatcherListener()
{
	return _directoryFilesWatcherListener;
}

void FilesInfoTableModel::clearData()
{
	if(0 == _fileNameIndexes.length()) {
		return;
	}
	beginResetModel();
	_fileNameIndexes.clear();
	_filesInfo.clear();
	endResetModel();
	emit dataCountChanged(_fileNameIndexes.length());
}

void FilesInfoTableModel::fileAdded(const QString fileName,
									const quint64 fileSize,
									const QString timeFileCreate) {

  FileInfo fileInfo{fileName, timeFileCreate, fileSize};
  _filesInfo[fileName] = fileInfo;
  int row = _fileNameIndexes.count();

  beginInsertRows( QModelIndex(), row, row );
 _fileNameIndexes.push_back(fileName);
  endInsertRows();
  emit dataCountChanged(_fileNameIndexes.length());
}
void FilesInfoTableModel::fileRemoved(const QString fileName) {
  if(!_filesInfo.contains(fileName)){
	return;
  }
  int row = _fileNameIndexes.indexOf(fileName);

  beginRemoveRows(QModelIndex(), row, row );
  _filesInfo.remove(fileName);
  _fileNameIndexes.removeAt(row);
  endRemoveRows();
  emit dataCountChanged(_fileNameIndexes.length());
}
void FilesInfoTableModel::fileChanged(const QString fileName,
									  const quint64 newFileSize,
									  const QString timeFileWrite) {
  if(!_filesInfo.contains(fileName)){
	return;
  }
  _filesInfo[fileName].fileSize = newFileSize;
  _filesInfo[fileName].timeFileCreateStr = timeFileWrite;

}
void FilesInfoTableModel::fileRenamed(const QString oldFileName,
									  const QString newFileName,
									  const QString timeFileWrite) {
  if(!_filesInfo.contains(oldFileName)){
	return;
  }
  //Only file size dont changes
  fileAdded(newFileName, _filesInfo[oldFileName].fileSize, timeFileWrite);
  fileRemoved(oldFileName);
}


