#ifndef FILEMANAGERWIN_H
#define FILEMANAGERWIN_H

#include "IFileManager.h"

class FileManagerWin : public IFileManager
{
public:
FileManagerWin();

// IFileManager interface
public:
~FileManagerWin() override;

bool renameFile(const std::wstring oldFileName,
				const std::wstring newFileName,
				const std::wstring directoryPath) override;
};

#endif // FILEMANAGERWIN_H
