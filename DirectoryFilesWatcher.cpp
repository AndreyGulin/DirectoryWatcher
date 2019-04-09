#include "DirectoryFilesWatcher.h"
#include "DirectoryControlsFactory.h"

DirectoryFilesWatcher::DirectoryFilesWatcher(const QString dirPath,
											 IDirectoryFilesWatcherListener* dfwl):
											 _dirPath(dirPath),
											 _directoryFilesWatcherListener(dfwl) {
}

DirectoryFilesWatcher::~DirectoryFilesWatcher() {
  delete _directoryFilesWatcherInst;
}

void DirectoryFilesWatcher::process() {
	_directoryFilesWatcherInst =
				DirectoryControlsFactory::getDirectoryFilesWatcher(_dirPath);
	_directoryFilesWatcherInst->setListener(_directoryFilesWatcherListener);
	_directoryFilesWatcherInst->process();
	emit finished();
}

void DirectoryFilesWatcher::stop() {
	_directoryFilesWatcherInst->stop();
}

