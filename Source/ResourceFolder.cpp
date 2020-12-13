#include "ResourceFolder.h"

ResourceFolder::ResourceFolder(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID) : Resource(ResourceType::Folder)
{
    this->name = name;
    this->libraryFile = libraryFile;
    this->assetsFile = assetsFile;
    if (UID != 0) this->UID = UID;
}

ResourceFolder::ResourceFolder() : Resource(ResourceType::Folder)
{
}

ResourceFolder::~ResourceFolder()
{
}
