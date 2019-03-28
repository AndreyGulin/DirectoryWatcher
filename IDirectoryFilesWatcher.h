#ifndef IDIRECTORYFILESWATCHER_H
#define IDIRECTORYFILESWATCHER_H
#include "IDirectoryFilesWatcherListener.h"

class IDirectoryFilesWatcher {
 public:
  virtual void setListener(IDirectoryFilesWatcherListener &DFWL) = 0;

  virtual void start() = 0;

  virtual void stop() = 0;

  virtual ~IDirectoryFilesWatcher() = 0;
};
#endif  // IDIRECTORYFILESWATCHER_H
