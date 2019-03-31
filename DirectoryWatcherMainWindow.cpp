#include "DirectoryWatcherMainWindow.h"
#include "FileManagerWin.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include <QHBoxLayout>
#include <QThread>

DirectoryWatcherMainWindow::DirectoryWatcherMainWindow(
	FilesInfoTableModel* filesInfoTableModel, const Platform platform,
	QWidget* parent)
	: QMainWindow(parent),
	  _platform(platform),
	  _filesInfoModel(filesInfoTableModel),
	  _fileManager(makeFileManager()){

  _filesInfoModel->setParent(this);

  QVBoxLayout* vLayout = new QVBoxLayout;

  constructFileInfoTableUI(vLayout);
  constructDirectoryPathInputUI(vLayout);
  constructCommandWatcherUI(vLayout);

  QWidget* centralWidget = new QWidget;
  centralWidget->setLayout(vLayout);

  this->setCentralWidget(centralWidget);
  this->setMinimumSize(800, 500);
}

DirectoryWatcherMainWindow::~DirectoryWatcherMainWindow() {}

void DirectoryWatcherMainWindow::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event)
	emit stopWatcher();
}

void DirectoryWatcherMainWindow::dirChoiceDialog() {
  QString directoryPath = QFileDialog::getExistingDirectory(
	  this, tr("Directory Dialog"), ".",
	  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  _labelDirPath->setText(directoryPath);

  if(directoryPath.isEmpty()) {
	//If directory didnt re'choiced.
	_startWatcherButton->setEnabled(false);
  }
  else {
	_startWatcherButton->setEnabled(true);
  }
}

void DirectoryWatcherMainWindow::watcherStoped() {
  _stopWatcherButton->setEnabled(false);
  _dirChoseButton->setEnabled(true);
  _startWatcherButton->setEnabled(true);
  _filesInfoModel->clearData();

  if (nullptr != _directoryFilesWatcher) {
	assert(true);
  }
}

void DirectoryWatcherMainWindow::tableRowCountChanged(int rowCount)
{
	_filesCounter->setText(tr("File count: %1").arg(rowCount));
}

void DirectoryWatcherMainWindow::startWatcher() {

  _startWatcherButton->setEnabled(false);
  _dirChoseButton->setEnabled(false);

  _directoryFilesWatcher = new DirectoryFilesWatcher(
	  _platform, _labelDirPath->text(),
	  _filesInfoModel->getDirectoryFilesWatcherListener());
  // stop watche button clicked, signal to watcher
  connect(this, &DirectoryWatcherMainWindow::stopWatcher,
		  _directoryFilesWatcher, &DirectoryFilesWatcher::stop, Qt::DirectConnection);
  //watcher finished, signal to main UI(change UI after that)
  connect(_directoryFilesWatcher, &DirectoryFilesWatcher::finished, this,
		  &DirectoryWatcherMainWindow::watcherStoped);


  QThread* thread = new QThread;
  _directoryFilesWatcher->moveToThread(thread);
  //after thread started, signal watcher to run process
  connect(thread, &QThread::started,
		  _directoryFilesWatcher,  &DirectoryFilesWatcher::process);
  //after watcher finished, signal thread to quit
  connect(_directoryFilesWatcher, &DirectoryFilesWatcher::finished,
											thread, &QThread::quit);
  //after watcher finished, delete it
  connect(_directoryFilesWatcher, &DirectoryFilesWatcher::finished,
		  _directoryFilesWatcher, &DirectoryFilesWatcher::deleteLater);
  //after thread finished, delete it
  connect(thread, &QThread::finished,
		  _directoryFilesWatcher, &QThread::deleteLater);

  thread->start();

  _stopWatcherButton->setEnabled(true);
}

void DirectoryWatcherMainWindow::renameFile()
{
	using Column = FilesInfoTableModel::Column;

	int row = _filesInfoView->selectionModel()->currentIndex().row();
	if(row < 0) {
		QMessageBox::warning(nullptr, QObject::tr("Warning"),
							 QObject::tr("No file selected. Select it first!"));
		return;
	}
	bool ok;
	QString oldFileName =
		_filesInfoModel->index(row,Column::COLUMN_FILE_NAME).data().toString();
	QString newFileName = QInputDialog::getText(this,
										 tr("File rename"),
										 QString::fromUtf8("New file name:"),
										 QLineEdit::Normal,
										 oldFileName, &ok,
										 Qt::MSWindowsFixedSizeDialogHint);
	if(!ok) {
		return;
	}
	bool resultRenameFile = _fileManager->renameFile(oldFileName.toStdWString(),
							 newFileName.toStdWString(),
							 _labelDirPath->text().toStdWString());
	if(!resultRenameFile) {
		QMessageBox::warning(nullptr, QObject::tr("Warning"),
							 QObject::tr("Rename file failed!"));
		return;
	}
}

void DirectoryWatcherMainWindow::constructFileInfoTableUI(
	QVBoxLayout* vlayaut) {
  _filesCounter = new QLabel(this);
  vlayaut->addWidget(_filesCounter);
  _filesInfoView = new QTableView;
  _filesInfoView->setModel(_filesInfoModel);
  _filesInfoView->resizeColumnsToContents();
  _filesInfoView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  _filesInfoView->setSelectionBehavior(QAbstractItemView::SelectRows);

  //construct context menu for view table
  _filesInfoView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_filesInfoView, &QTableView::customContextMenuRequested,
		  this, &DirectoryWatcherMainWindow::slotCustomMenuRequested);

  //When the information about the files in the model changes,
  //  need change the width of the column.
  connect(_filesInfoModel, &FilesInfoTableModel::infoChanged,
		  _filesInfoView, &QTableView::resizeColumnsToContents );
  //Files count changed
  connect(_filesInfoModel, &FilesInfoTableModel::dataCountChanged,
		  this, &DirectoryWatcherMainWindow::tableRowCountChanged);
  //Setting the stretch
  //  for view table to use the entire free height of the parent widget.
  const int stretch = 1;
  vlayaut->addWidget(_filesInfoView, stretch);
}

void DirectoryWatcherMainWindow::constructDirectoryPathInputUI(
	QVBoxLayout* vlayaut) {
  _dirChoseButton = new QPushButton(tr("Directory"));
  connect(_dirChoseButton, &QPushButton::clicked, this,
		  &DirectoryWatcherMainWindow::dirChoiceDialog);

  QSpacerItem* spacer =
	  new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
  _labelDirPath = new QLabel;

  QHBoxLayout* hLayout = new QHBoxLayout;
  hLayout->addWidget(_dirChoseButton);
  hLayout->addSpacerItem(spacer);
  hLayout->addWidget(_labelDirPath);

  vlayaut->addLayout(hLayout);
}

void DirectoryWatcherMainWindow::constructCommandWatcherUI(
	QVBoxLayout* vlayaut) {
  _startWatcherButton = new QPushButton(tr("Start watch"));
  _startWatcherButton->setEnabled(false);

  //Start watcher
  connect(_startWatcherButton, &QPushButton::clicked, this,
		  &DirectoryWatcherMainWindow::startWatcher);
  _stopWatcherButton = new QPushButton(tr("Stop watch"));
  _stopWatcherButton->setEnabled(false);

  //Stop watcher
  connect(_stopWatcherButton, &QPushButton::clicked, this,
		  &DirectoryWatcherMainWindow::stopWatcher);

  QHBoxLayout* hLayout = new QHBoxLayout;
  hLayout->addWidget(_startWatcherButton);
  hLayout->addWidget(_stopWatcherButton);

  vlayaut->addLayout(hLayout);
}

void DirectoryWatcherMainWindow::slotCustomMenuRequested(QPoint pos)
{
	QMenu * menu = new QMenu(this);
	QAction * renameFileAction = new QAction(tr("Rename file"), this);

	connect(renameFileAction, &QAction::triggered,
			this, &DirectoryWatcherMainWindow::renameFile);
	menu->addAction(renameFileAction);
	menu->popup(_filesInfoView->viewport()->mapToGlobal(pos));
}

IFileManager* DirectoryWatcherMainWindow::makeFileManager() {
	switch (_platform) {
	case Platform::WINDOWS: {
		return new FileManagerWin();
	}
	case Platform::MACOSX:
	case Platform::LINUX:
	case Platform::UNKNOWN: {
		return nullptr;
	}
	}
	return nullptr;
}

