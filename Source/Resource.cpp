#include "Resource.h"


Resource::Resource()
{
}

Resource::Resource(ResourceType type) : type(type)
{
	UID = randomNum.GenerateRandomInt();
}

Resource::Resource(ResourceType type, const char* assetsFile, const char* name) : 
	type(type),
	name(name),
	assetsFile(assetsFile),
	UID(randomNum.GenerateRandomInt())
{
}

Resource::~Resource()
{

}


