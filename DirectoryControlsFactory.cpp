#include "DirectoryControlsFactory.h"

#ifdef Q_OS_WIN
	//TODO change pro file
	#include "DirectoryFilesWatcherWin.h"
	#include "FileManagerWin.h"
	typedef DirectoryFilesWatcherWin DirectoryFilesWatcherPlatformImpl;
	typedef FileManagerWin FileManagerPlatformImpl;

	//TODO comment
	//If platform didnt supported, then compile time error
	static bool isPlatformVersionIncluded {true};
#else
	// #include platform version of directory controls
	//typedef DirectoryFilesWatcherWin DirectoryFilesWatcherPlatformImpl;
	//typedef FileManagerWin FileManagerPlatformImpl;
#endif

DirectoryControlsFactory::DirectoryControlsFactory() { }

IDirectoryFilesWatcher* DirectoryControlsFactory::getDirectoryFilesWatcher(
														const QString dirPath) {
	if(!isPlatformVersionIncluded) {
		return nullptr;
	}
	return new DirectoryFilesWatcherPlatformImpl(dirPath.toStdWString());

}


IFileManager* DirectoryControlsFactory::makeFileManager() {
	if(!isPlatformVersionIncluded) {
		return nullptr;
	}
	return new FileManagerPlatformImpl();
}
