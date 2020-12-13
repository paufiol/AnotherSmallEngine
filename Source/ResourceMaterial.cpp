#include "ResourceMaterial.h"


ResourceMaterial::ResourceMaterial(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID) : Resource(ResourceType::Material)
{
	this->name = name;
	this->libraryFile = libraryFile;
	this->assetsFile = assetsFile;
	if (UID != 0) this->UID = UID;

}

ResourceMaterial::ResourceMaterial(uint _id, const char* _path) : Resource(ResourceType::Material)
{
	texture->id = _id;
	texture->path = _path;
}

ResourceMaterial::ResourceMaterial(ResourceTexture* _texture) : Resource(ResourceType::Material)
{
	texture = _texture;
}

ResourceMaterial::ResourceMaterial() : Resource(ResourceType::Material)
{
	/*texture->id = 0;
	texture->path = "";*/
}

ResourceMaterial::~ResourceMaterial()
{

}