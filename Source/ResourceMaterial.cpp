#include "ResourceMaterial.h"

ResourceMaterial::ResourceMaterial(uint _id, const char* _path) : Resource(ResourceType::Material)
{
	texture.id = _id;
	texture.path = _path;
}

ResourceMaterial::ResourceMaterial(Texture _texture) : Resource(ResourceType::Material)
{
	texture = _texture;
}

ResourceMaterial::ResourceMaterial() : Resource(ResourceType::Material)
{

}

ResourceMaterial::~ResourceMaterial()
{

}