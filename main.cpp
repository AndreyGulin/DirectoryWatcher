#include "DirectoryWatcherMainWindow.h"

#include <QApplication>


int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

   //make model and listener
  FilesInfoTableModel* filesInfoTableModel = new FilesInfoTableModel();

  DirectoryFilesWatcherListener* directoryFilesWatcherListener =
	  new DirectoryFilesWatcherListener;
  //setup listener for this model
  filesInfoTableModel->setDirectoryFilesWatcherListener(directoryFilesWatcherListener);

  DirectoryWatcherMainWindow w(filesInfoTableModel);
  w.show();

  return a.exec();
}
