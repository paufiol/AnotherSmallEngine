#ifndef __ResoruceTexture_H__
#define __ResoruceTexture_H__

#include "Globals.h"
#include "Color.h"
#include "Resource.h"

class ResourceTexture : public Resource
{
public:

	ResourceTexture(const char* assetsFile, const char* libraryFile, const char* name);
	ResourceTexture();
	~ResourceTexture();

public:

	uint id = 0;
	std::string path;

};
#endif //__ResoruceTexture_H__
