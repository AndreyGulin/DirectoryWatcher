#ifndef IFILEMANAGER_H
#define IFILEMANAGER_H
#include <string>

/*!
 * \brief   Crossplatform file manager interface.
 * \author Gulin Andrey
 * \version 1.0
 *
 * \todo add other command for manage file
 */

class IFileManager
{
public:
virtual ~IFileManager() = 0;

virtual bool renameFile(const std::wstring oldFileName,
						const std::wstring newFileName,
						const std::wstring directoryPath) = 0;
};

#endif // IFILEMANAGER_H
