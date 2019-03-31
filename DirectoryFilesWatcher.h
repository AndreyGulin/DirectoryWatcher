#ifndef DIRECTORYFILESWATCHER_H
#define DIRECTORYFILESWATCHER_H

#include <QObject>
#include "DirectoryFilesWatcherWin.h"


enum class Platform: int {
	LINUX,
	MACOSX,
	WINDOWS,
	UNKNOWN
};

class DirectoryFilesWatcher : public QObject {
Q_OBJECT
public:
	DirectoryFilesWatcher(const Platform platform, const QString dirPath,
						  IDirectoryFilesWatcherListener *dfwl);
	~DirectoryFilesWatcher() override;

	void process() ;
private:
IDirectoryFilesWatcher* getDirectoryFilesWatcher(const Platform platform,
													 const QString dirPath);
public slots:
	void stop();

signals:
	void finished();
private:
  const Platform                  _platform;
  const QString                   _dirPath;
  IDirectoryFilesWatcher*         _directoryFilesWatcherInst;
  IDirectoryFilesWatcherListener* _directoryFilesWatcherListener;
};

#endif  // DIRECTORYFILESWATCHER_H
