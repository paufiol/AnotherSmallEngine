#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID) : Resource(ResourceType::Texture)
{
	this->name = name;
	this->libraryFile = libraryFile;
	this->assetsFile = assetsFile;
	this->UID = UID;
}

ResourceTexture::ResourceTexture(uint ID, std::string path) : Resource(ResourceType::Texture), id(ID), path(path)
{
}

ResourceTexture::ResourceTexture() : Resource(ResourceType::Texture)
{
	id = 0;
	path = "";
}

ResourceTexture::~ResourceTexture()
{
}
