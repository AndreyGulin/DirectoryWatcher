#ifndef DIRECTORYWATCHERMAINWINDOW_H
#define DIRECTORYWATCHERMAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <memory>

//TODO rename to  DirectoryFilesWatchWorker ?
#include "DirectoryFilesWatcher.h"
#include "FilesInfoTableModel.h"
#include "IFileManager.h"

class DirectoryWatcherMainWindow : public QMainWindow {
  Q_OBJECT

public:
	DirectoryWatcherMainWindow(FilesInfoTableModel* filesInfoTableModel,
							   QWidget* parent = nullptr);
  ~DirectoryWatcherMainWindow();

// QWidget interface
protected:
	  void closeEvent(QCloseEvent *event);

private:
	void constructFileInfoTableUI(QVBoxLayout *layaut);
	void constructDirectoryPathInputUI(QVBoxLayout *layaut);
	void constructCommandWatcherUI(QVBoxLayout *layaut);
	void constructFileInfoContextMenuUI();

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
	 enum ColumnDefaultWidth {
		 COLUMN_FILE_NAME_WIDTH = 530,
		 COLUMN_FILE_SIZE_WIDTH = 80,
		 COLUMN_FILE_TIME_WIDTH = 140
	 };

  QLabel*                _labelDirPath          {nullptr};
  QLabel*                _filesCounter          {nullptr};

  QPushButton*           _startWatcherButton    {nullptr};
  QPushButton*           _stopWatcherButton     {nullptr};
  QPushButton*           _dirChoseButton        {nullptr};

  QTableView*            _filesInfoView         {nullptr};
  QMenu*                 _filesInfoContextMenu  {nullptr};

  DirectoryFilesWatcher* _directoryFilesWatcher {nullptr};
  FilesInfoTableModel*   _filesInfoModel        {nullptr};

  std::unique_ptr<IFileManager> _fileManager;
};

#endif  // DIRECTORYWATCHERMAINWINDOW_H
