# DirectoryWatcher/ru

Кроссплатформенные интерфейсы (без использования Qt)
 - IDirectoryFilesWatcher : Основной интерфейс наблюдателя.
 - IDirectoryFilesWatcherListener : Интерфейс получателя сообщений от наблюдателя.
 - IFileManager : Интерфейс управления файлами (по задаче только переименование)


Обертки над кросплатформенными классами, для реализации логики взаимодействия на Qt
- DirectoryFilesWatcher
- DirectoryFilesWatcherListener
- DirectoryWatcher

Платформо-зависимые реализации (Windows OS)
- FileManagerWin
- DirectoryFilesWatcherWin

Реализация уровня Ui на Qt
- DirectoryWatcherMainWindow

Реализация модели данных ("бизнес" уровня) средствами Qt 
- FilesInfoTableModel

