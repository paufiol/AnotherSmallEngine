#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

enum class ResourceType
{
	Mesh,
	Material,
	Texture,
	None
};

class Resource
{
public:

	Resource();
	~Resource();

};
#endif //__RESOURCE_H__
