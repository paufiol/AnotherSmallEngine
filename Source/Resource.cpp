#include "Resource.h"


Resource::Resource()
{
	UID = randomNum.GenerateRandomInt();
}

Resource::Resource(ResourceType type) : type(type)
{
	UID = randomNum.GenerateRandomInt();
}

Resource::Resource(ResourceType type, const char* assetsFile, const char* name, uint32 ID) : 
	type(type),
	name(name),
	assetsFile(assetsFile),
	UID(ID)
{
	UID == 0 ? randomNum.GenerateRandomInt() : ID;
}

Resource::~Resource()
{

}


