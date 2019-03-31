#include "DirectoryFilesWatcherListener.h"
#include <iostream>
#include <QDateTime>

DirectoryFilesWatcherListener::DirectoryFilesWatcherListener() {}

DirectoryFilesWatcherListener::~DirectoryFilesWatcherListener()
{

}

void DirectoryFilesWatcherListener::fileAdded(const std::wstring fileName,
											  const std::uint64_t fileSize,
											  const std::string timeFileWrite) {
  QString qfileName = QString::fromStdWString(fileName);
  QString qtimeFileWrite = QString::fromStdString(timeFileWrite);
  std::cout << "DirectoryFilesWatcherListener::fileAdded time "<< timeFileWrite ;
  std::wcout  << L" file:" << fileName << L"\n";
  emit fileAddedSignal(qfileName, fileSize, qtimeFileWrite);
}

void DirectoryFilesWatcherListener::fileRemoved(const std::wstring fileName) {
  QString qfileName = QString::fromStdWString(fileName);
  emit fileRemovedSignal(qfileName);
}

void DirectoryFilesWatcherListener::fileChanged(const std::wstring fileName,
												const std::uint64_t newFileSize,
												const std::string timeFileWrite) {
  QString qfileName = QString::fromStdWString(fileName);
  QString qtimeFileWrite = QString::fromStdString(timeFileWrite);
  emit fileChangedSignal(qfileName, newFileSize, qtimeFileWrite);
}

void DirectoryFilesWatcherListener::fileRenamed(const std::wstring oldFileName,
												const std::wstring newFileName,
												const std::string timeFileWrite) {
  QString qoldFileName = QString::fromStdWString(oldFileName);
  QString qnewFileName = QString::fromStdWString(newFileName);
  QString qtimeFileWrite = QString::fromStdString(timeFileWrite);
  emit fileRenamedSignal(qoldFileName, qnewFileName, qtimeFileWrite);
}
