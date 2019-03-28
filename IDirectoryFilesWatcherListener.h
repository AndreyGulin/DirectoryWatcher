#ifndef IDIRECTORYFILESWATCHERLISTENER_H
#define IDIRECTORYFILESWATCHERLISTENER_H
#include <string>

class IDirectoryFilesWatcherListener {
 public:
  virtual void fileAdded(const std::wstring fileName, const uint64_t fileSize,
						 const std::wstring timeFileCreate) = 0;

  virtual void fileRemoved(const std::wstring fileName) = 0;

  virtual void fileChanged(const std::wstring fileName,
						   const uint64_t newFileSize) = 0;

  virtual void fileRenamed(const std::wstring oldFileName,
						   const std::wstring newFileName) = 0;

  virtual ~IDirectoryFilesWatcherListener() = 0;
};
#endif  // IDIRECTORYFILESWATCHERLISTENER_H
