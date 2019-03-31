#include <iostream>
#include "DirectoryFilesWatcher.h"

DirectoryFilesWatcher::DirectoryFilesWatcher(const Platform platform,
											 const QString dirPath,
											 IDirectoryFilesWatcherListener* dfwl):
											 _platform(platform),
											 _dirPath(dirPath),
											 _directoryFilesWatcherListener(dfwl) {
}

DirectoryFilesWatcher::~DirectoryFilesWatcher() {
  delete _directoryFilesWatcherInst;
}

void DirectoryFilesWatcher::process() {
	_directoryFilesWatcherInst = getDirectoryFilesWatcher(_platform, _dirPath);
	_directoryFilesWatcherInst->setListener(_directoryFilesWatcherListener);
	_directoryFilesWatcherInst->process();
	emit finished();
}

void DirectoryFilesWatcher::stop() {
	_directoryFilesWatcherInst->stop();
}

IDirectoryFilesWatcher* DirectoryFilesWatcher::getDirectoryFilesWatcher(
														const Platform platform,
														const QString dirPath) {
  switch (platform) {
	case Platform::WINDOWS: {
	  return new DirectoryFilesWatcherWin(dirPath.toStdWString());
	}
	case Platform::MACOSX:
	case Platform::LINUX:
	case Platform::UNKNOWN: {
	  return nullptr;
	}
  }
  return nullptr;
}
