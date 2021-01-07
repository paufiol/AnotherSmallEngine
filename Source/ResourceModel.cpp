#include "ResourceModel.h"

ResourceModel::ResourceModel() : Resource(ResourceType::Model)
{
}

ResourceModel::ResourceModel(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID) : Resource(ResourceType::Model)
{
	this->name = name;
	this->libraryFile = libraryFile;
	this->assetsFile = assetsFile;
	if (UID != 0) this->UID = UID;
}

ResourceModel::~ResourceModel()
{
}
