#ifndef DIRECTORYFILESWATCHERWIN_H
#define DIRECTORYFILESWATCHERWIN_H

#include <windows.h>

#include "IDirectoryFilesWatcher.h"

class DirectoryFilesWatcherWin : public IDirectoryFilesWatcher {
 public:
  explicit DirectoryFilesWatcherWin(const std::wstring& directoryPath);

  ~DirectoryFilesWatcherWin() override;

  virtual void setListener(
	  IDirectoryFilesWatcherListener* directoryFileWatcherListener) override;

  virtual void process() override;

  virtual void stop() override;

 private:
  void fileAdded(const std::wstring& fileName);
  void fileRemoved(const std::wstring& fileName);
  void fileChanged(const std::wstring& fileName);
  void fileRenamed(const std::wstring& oldFileName,
				   const std::wstring& newFileName);

  void readAndAddExistingFiles();
  void readNotifyBuffer(BYTE* notifyBuffer);
  bool getFileInfo(const std::wstring& fileName, WIN32_FIND_DATAW& wfd);

  std::string fileTimeToString( FILETIME ftime );
  uint64_t fileSizeFromHighPartAndLow(DWORD nFileSizeHigh,
									  DWORD nFileSizeLow);

 private:

  std::wstring                    _dirPath;
  HANDLE                          _hDirectory;
  HANDLE                          _stopEvent;

  IDirectoryFilesWatcherListener* _watcherListener;
};
#endif  // DIRECTORYFILESWATCHERWIN_H
