#ifndef IDIRECTORYFILESWATCHER_H
#define IDIRECTORYFILESWATCHER_H
#include "IDirectoryFilesWatcherListener.h"

/*!
 * \brief   Crossplatform directory watcher interface.
 * \details Interface for implementing file watchers in the specified directory.
 * \author Gulin Andrey
 * \version 1.0
 */

class IDirectoryFilesWatcher {
 public:
  /// Set listener object, for listen watcher notification
  /// \param[in] dfmt listener object
  virtual void setListener(IDirectoryFilesWatcherListener *dfml) = 0;

  /// Main watcher loop
  virtual void process() = 0;
  /// Stop watching
  virtual void stop() = 0;

  virtual ~IDirectoryFilesWatcher() = 0;
};
#endif  // IDIRECTORYFILESWATCHER_H
