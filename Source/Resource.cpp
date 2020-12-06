#include "Resource.h"


Resource::Resource(ResourceType type) : type(type), UID(randomNum.GenerateRandomInt())
{

}

Resource::~Resource()
{

}

const uint32 Resource::GetUID() const
{
	return UID;
}
