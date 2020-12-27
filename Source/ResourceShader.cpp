#include "ResourceShader.h"

ResourceShader::ResourceShader(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID) : Resource(ResourceType::Shader)
{
	this->name = name;
	this->libraryFile = libraryFile;
	this->assetsFile = assetsFile;
	if (UID != 0) this->UID = UID;
}

ResourceShader::~ResourceShader()
{

}
