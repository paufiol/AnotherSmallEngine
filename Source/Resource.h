#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"
#include "Random.h"
enum class ResourceType
{
	Mesh,
	Material,
	Texture,
	None
};

class Resource
{
private:
	ResourceType type;
	uint64 UID;
	Random randomNum;
public:

	Resource(ResourceType type);
	~Resource();

	const uint32 GetUID() const;

};
#endif //__RESOURCE_H__
