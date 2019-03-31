#include "FileManagerWin.h"
#include <iostream>
#include <windows.h>

FileManagerWin::FileManagerWin()
{

}

FileManagerWin::~FileManagerWin() { }

bool FileManagerWin::renameFile(const std::wstring oldFileName,
								const std::wstring newFileName,
								const std::wstring directoryPath)
{
	if(::MoveFile((directoryPath + L"/" + oldFileName).c_str(),
				   (directoryPath + L"/" + newFileName).c_str())) {
		return true;
	}
	std::cout << "Error rename file: " << GetLastError() << std::endl;
	return false;
}
