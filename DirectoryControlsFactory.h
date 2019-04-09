#ifndef DIRECTORYCONTROLSFACTORY_H
#define DIRECTORYCONTROLSFACTORY_H

#include "IDirectoryFilesWatcher.h"
#include "IFileManager.h"
#include <QString>

class DirectoryControlsFactory
{
public:
	DirectoryControlsFactory();

	static IDirectoryFilesWatcher* getDirectoryFilesWatcher( const QString dirPath);
	static IFileManager* makeFileManager();
};

#endif // DIRECTORYCONTROLSFACTORY_H
