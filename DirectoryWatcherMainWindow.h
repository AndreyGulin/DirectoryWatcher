#ifndef DIRECTORYWATCHERMAINWINDOW_H
#define DIRECTORYWATCHERMAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <memory>

#include "DirectoryFilesWatcher.h"
#include "FilesInfoTableModel.h"
#include "IFileManager.h"

class DirectoryWatcherMainWindow : public QMainWindow {
  Q_OBJECT

public:
	DirectoryWatcherMainWindow(FilesInfoTableModel* filesInfoTableModel,
							   const Platform platform,
							   QWidget* parent = nullptr);
  ~DirectoryWatcherMainWindow();

// QWidget interface
protected:
	  void closeEvent(QCloseEvent *event);

private:
	void constructFileInfoTableUI(QVBoxLayout *layaut);
	void constructDirectoryPathInputUI(QVBoxLayout *layaut);
	void constructCommandWatcherUI(QVBoxLayout *layaut);
	IFileManager* makeFileManager();
public slots:
	void dirChoiceDialog();
	void watcherStoped();
	void tableRowCountChanged(int rowCount);
private slots:
	void startWatcher();
	void renameFile();
	void slotCustomMenuRequested(QPoint pos);
signals:
	void stopWatcher();

 private:
  const Platform         _platform;
  QLabel*                _labelDirPath          {nullptr};
  QLabel*                _filesCounter          {nullptr};

  QPushButton*           _startWatcherButton    {nullptr};
  QPushButton*           _stopWatcherButton     {nullptr};
  QPushButton*           _dirChoseButton        {nullptr};

  QTableView*            _filesInfoView         {nullptr};

  DirectoryFilesWatcher* _directoryFilesWatcher {nullptr};
  FilesInfoTableModel*   _filesInfoModel        {nullptr};

  std::unique_ptr<IFileManager> _fileManager;
};

#endif  // DIRECTORYWATCHERMAINWINDOW_H
