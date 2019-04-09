#ifndef DIRECTORYFILESWATCHER_H
#define DIRECTORYFILESWATCHER_H

#include <QObject>
#include "IDirectoryFilesWatcher.h"
#include "IDirectoryFilesWatcherListener.h"

//TODO rename to worker
class DirectoryFilesWatcher : public QObject {
Q_OBJECT
public:
	DirectoryFilesWatcher(const QString dirPath,
						  IDirectoryFilesWatcherListener *dfwl);
	~DirectoryFilesWatcher() override;

	void process() ;

public slots:
	void stop();

signals:
	void finished();
private:
  const QString                   _dirPath;
  IDirectoryFilesWatcher*         _directoryFilesWatcherInst;
  IDirectoryFilesWatcherListener* _directoryFilesWatcherListener;
};

#endif  // DIRECTORYFILESWATCHER_H
