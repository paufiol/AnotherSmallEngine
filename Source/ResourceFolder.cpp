#include "ResourceFolder.h"

ResourceFolder::ResourceFolder(const char* assetsFile, const char* libraryFile, const char* name) : Resource(ResourceType::Folder)
{
    this->name = name;
    this->libraryFile = libraryFile;
    this->assetsFile = assetsFile;

}

ResourceFolder::ResourceFolder() : Resource(ResourceType::Folder)
{
}

ResourceFolder::~ResourceFolder()
{
}
