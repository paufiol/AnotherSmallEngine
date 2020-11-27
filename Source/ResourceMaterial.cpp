#include "ResourceMaterial.h"

ResourceMaterial::ResourceMaterial(uint _id, const char* _path)
{
	texture.id = _id;
	texture.path = _path;
}

ResourceMaterial::ResourceMaterial(Texture _texture)
{
	texture = _texture;
}

ResourceMaterial::ResourceMaterial()
{

}

ResourceMaterial::~ResourceMaterial()
{

}