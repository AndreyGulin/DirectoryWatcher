#ifndef IDIRECTORYFILESWATCHERLISTENER_H
#define IDIRECTORYFILESWATCHERLISTENER_H
#include <string>

/*!
 * \brief   Crossplatform listener interface  for listen directory watcher.
 * \details Interface for implementing and using file watcher listener.
 *			Listen notification for file added, changed, renamed and removed.
 * \author Gulin Andrey
 * \version 1.0
 */

class IDirectoryFilesWatcherListener {
 public:
	 virtual void fileAdded(const std::wstring fileName,
							const std::uint64_t fileSize,
							const std::string timeFileWrite) = 0;

  virtual void fileRemoved(const std::wstring fileName) = 0;

  virtual void fileChanged(const std::wstring fileName,
						   const std::uint64_t newFileSize,
						   const std::string timeFileWrite) = 0;

  virtual void fileRenamed(const std::wstring oldFileName,
						   const std::wstring newFileName,
						   const std::string timeFileWrite) = 0;

  virtual ~IDirectoryFilesWatcherListener() = 0;
};
#endif  // IDIRECTORYFILESWATCHERLISTENER_H


