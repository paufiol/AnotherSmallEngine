#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(const char* assetsFile, const char* libraryFile, const char* name) : Resource(ResourceType::Texture)
{
	this->name = name;
	this->libraryFile = libraryFile;
	this->assetsFile = assetsFile;
}

ResourceTexture::ResourceTexture() : Resource(ResourceType::Texture)
{
}

ResourceTexture::~ResourceTexture()
{
}
