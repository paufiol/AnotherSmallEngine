#ifndef __RESOURCEFOLDER_H__
#define __RESOURCEFOLDER_H__

#include "Resource.h"

class ResourceFolder : public Resource
{
public:
	ResourceFolder(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID);
	ResourceFolder(); 
	~ResourceFolder();

};

#endif //__RESOURCEFOLDER_H__