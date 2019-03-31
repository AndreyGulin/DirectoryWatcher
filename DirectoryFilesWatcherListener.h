#ifndef DIRECTORYFILESWATCHERLISTENER_H
#define DIRECTORYFILESWATCHERLISTENER_H

#include <QObject>
#include "IDirectoryFilesWatcherListener.h"

class DirectoryFilesWatcherListener : public QObject,
									  public IDirectoryFilesWatcherListener {
  Q_OBJECT
 public:
  DirectoryFilesWatcherListener();
  ~DirectoryFilesWatcherListener() override;

  virtual void fileAdded(const std::wstring fileName,
						 const std::uint64_t fileSize,
						 const std::string timeFileWrite) override;

  virtual void fileRemoved(const std::wstring fileName) override;

  virtual void fileChanged(const std::wstring fileName,
						   const std::uint64_t newFileSize,
						   const std::string timeFileWrite) override;

  virtual void fileRenamed(const std::wstring oldFileName,
						   const std::wstring newFileName,
						   const std::string timeFileWrite) override;

 signals:
  void fileAddedSignal(const QString fileName,
					   const quint64 newFileSize,
					   const QString timeFileWrite);

  void fileRemovedSignal(const QString fileName);

  void fileChangedSignal(const QString fileName,
						 const quint64 newFileSize,
						 const QString timeFileWrite);

  void fileRenamedSignal(const QString oldFileName,
						 const QString newFileName,
						 const QString timeFileWrite);
};

#endif  // DIRECTORYFILESWATCHERLISTENER_H
