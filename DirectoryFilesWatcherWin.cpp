#include "DirectoryFilesWatcherWin.h"
#include <iomanip>
#include <iostream>
#include <sstream>

DirectoryFilesWatcherWin::DirectoryFilesWatcherWin(
	const std::wstring& directoryPath)
	: _dirPath(directoryPath) {
  // Create stop event to stop the loop, and set state to normal (not signal state)
  _stopEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
}
DirectoryFilesWatcherWin::~DirectoryFilesWatcherWin() {
  ::CloseHandle(_stopEvent);
}

void DirectoryFilesWatcherWin::setListener(
	IDirectoryFilesWatcherListener* directoryFileWatcherListener) {
  _watcherListener = directoryFileWatcherListener;
}

void DirectoryFilesWatcherWin::process() {
  if (nullptr == _watcherListener) {
	return;
  }
  // Get handle for directory to watch
  _hDirectory = ::CreateFile(_dirPath.c_str(),
							 GENERIC_READ | FILE_LIST_DIRECTORY,
							 FILE_SHARE_READ | FILE_SHARE_WRITE |
								 FILE_SHARE_DELETE,
							 nullptr,
							 OPEN_EXISTING,
							 FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
							 nullptr);

  if (INVALID_HANDLE_VALUE == _hDirectory) {
	  std::cout << "CreateFile error " << GetLastError()
				<< std::endl;
	  return;
  }

  //notify the listener about files located in the directory
  readAndAddExistingFiles();

  bool stopFlag{false};
  OVERLAPPED overlap;
  overlap.OffsetHigh = 0;
  // Create event object for notify
  overlap.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

  do {
	DWORD dwBytesReturned = 0;
	const DWORD dwBuffLength = 1024 * 64;
	BYTE buffer[dwBuffLength] = {0};
	//read directory changes notifay, asynchronously
	bool result = ::ReadDirectoryChangesW(_hDirectory,
										  buffer,
										  dwBuffLength, FALSE,
										  FILE_NOTIFY_CHANGE_FILE_NAME |
											  FILE_NOTIFY_CHANGE_ATTRIBUTES |
											  FILE_NOTIFY_CHANGE_CREATION |
											  FILE_NOTIFY_CHANGE_SIZE,
										  &dwBytesReturned,
										  &overlap, nullptr);

	HANDLE events[] = {overlap.hEvent, _stopEvent};
	//wait events
	DWORD waitObject = WaitForMultipleObjects(2, events, false, INFINITE);
	switch (waitObject) {
	  case WAIT_OBJECT_0: {
		if (!result) {
		  continue;
		}
		//notify the listener about files changes, addition, deletion
		readNotifyBuffer(buffer);
		break;
	  }
	  case WAIT_OBJECT_0 + 1: {
		stopFlag = true;
		break;
	  }
	  default:
		std::cout << "WaitForSingleObject error" << GetLastError()
				  << std::endl;
		break;
	}
  } while (!stopFlag);
  ::CloseHandle(overlap.hEvent);
  ::CloseHandle(_hDirectory);
}

void DirectoryFilesWatcherWin::stop() { ::SetEvent(_stopEvent); }


void DirectoryFilesWatcherWin::readAndAddExistingFiles() {
  WIN32_FIND_DATAW wfd;
  HANDLE const hFind = ::FindFirstFileW((_dirPath + L"/*").c_str(), &wfd);
  if (INVALID_HANDLE_VALUE == hFind) {
	return;
  }
  do {
	if ((wfd.dwFileAttributes != FILE_ATTRIBUTE_REPARSE_POINT) &&
		!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

	  std::wstring fileName = wfd.cFileName;
	  uint64_t fileSize =
		  (wfd.nFileSizeHigh * (MAXDWORD + 1)) + wfd.nFileSizeLow;
	  //Notify listener about file located in the directory
	  _watcherListener->fileAdded(fileName, fileSize, fileTimeToString(wfd.ftLastWriteTime));
	}
  } while (0 != FindNextFileW(hFind, &wfd));
  ::FindClose(hFind);
}

void DirectoryFilesWatcherWin::readNotifyBuffer(BYTE* notifyBuffer) {
  DWORD dwNextEntryOffset = 0;
  PFILE_NOTIFY_INFORMATION pNotify = nullptr;

  std::wstring oldFileName;
  do {
	pNotify = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(notifyBuffer +
														 dwNextEntryOffset);
	std::wstring fileName(pNotify->FileName,
						  pNotify->FileNameLength / sizeof(WCHAR));

	switch (pNotify->Action) {
	  case FILE_ACTION_ADDED:
		fileAdded(fileName);
		break;
	  case FILE_ACTION_REMOVED:
		fileRemoved(fileName);
		break;
	  case FILE_ACTION_MODIFIED:
		fileChanged(fileName);
		break;
	  case FILE_ACTION_RENAMED_OLD_NAME:
		oldFileName = fileName;
		break;
	  case FILE_ACTION_RENAMED_NEW_NAME:
		fileRenamed(oldFileName, fileName);
		break;
	}

	dwNextEntryOffset += pNotify->NextEntryOffset;

  } while (pNotify->NextEntryOffset);
}
bool DirectoryFilesWatcherWin::getFileInfo(const std::wstring& fileName,
										   WIN32_FIND_DATAW& wfd) {
	HANDLE const hFind = ::FindFirstFileW((_dirPath + L"/" + fileName).c_str(), &wfd);
	if (INVALID_HANDLE_VALUE == hFind) {
		return false;
	}
	::CloseHandle(hFind);
	return true;
}

void DirectoryFilesWatcherWin::fileAdded(const std::wstring& fileName) {
	WIN32_FIND_DATAW wfd;
	if(! getFileInfo(fileName, wfd)){
		return;
	}
  uint64_t fileSize = fileSizeFromHighPartAndLow(wfd.nFileSizeHigh,
												 wfd.nFileSizeLow);
  _watcherListener->fileAdded(fileName, fileSize,
							  fileTimeToString(wfd.ftLastWriteTime));
}
void DirectoryFilesWatcherWin::fileRemoved(const std::wstring& fileName) {
  _watcherListener->fileRemoved(fileName);
}
void DirectoryFilesWatcherWin::fileChanged(const std::wstring& fileName) {
	WIN32_FIND_DATAW wfd;
	if(! getFileInfo(fileName, wfd)){
		return;
	}
  uint64_t fileSize = fileSizeFromHighPartAndLow(wfd.nFileSizeHigh,
												   wfd.nFileSizeLow);
  _watcherListener->fileChanged(fileName,fileSize,
								  fileTimeToString(wfd.ftLastWriteTime));
}
void DirectoryFilesWatcherWin::fileRenamed(const std::wstring& oldFileName,
										   const std::wstring& newFileName) {
	WIN32_FIND_DATAW wfd;
	if(! getFileInfo(newFileName, wfd)){
		return;
	}
	_watcherListener->fileRenamed(oldFileName, newFileName,
								  fileTimeToString(wfd.ftLastWriteTime));
}

std::string DirectoryFilesWatcherWin::fileTimeToString(FILETIME ftime) {
  SYSTEMTIME localTime;
  FILETIME localFT;
  ::FileTimeToLocalFileTime(&ftime, &localFT);
  ::FileTimeToSystemTime(&localFT, &localTime);

  std::ostringstream stm;
  const auto w2 = std::setw(2);
  stm << std::setfill('0') << std::setw(4) << localTime.wYear << '-' << w2
	  << localTime.wMonth << '-' << w2 << localTime.wDay << ' ' << w2
	  << localTime.wYear << ' ' << w2 << localTime.wHour << ':' << w2
	  << localTime.wMinute << ':' << w2  << localTime.wSecond;

  return stm.str();
}

uint64_t DirectoryFilesWatcherWin::fileSizeFromHighPartAndLow(DWORD nFileSizeHigh,
															  DWORD nFileSizeLow)
{
	return (nFileSizeHigh * (MAXDWORD + 1)) + nFileSizeLow;
}
