#include "ResourceShader.h"

ResourceShader::ResourceShader(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID) : Resource(ResourceType::Shader)
{
	this->name = name;
	this->libraryFile = libraryFile;
	this->assetsFile = assetsFile;
	if (UID != 0) this->UID = UID;
}

ResourceShader::ResourceShader() : Resource(ResourceType::Shader)
{
}

ResourceShader::~ResourceShader()
{

}

void ResourceShader::SetUniformMatrix4(std::string name, GLfloat* value)
{
	int uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniformMatrix4fv(uinformLoc, 1, GL_FALSE, value);
}

void ResourceShader::SetUniform4f(std::string name, GLfloat* value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform4fv(uinformLoc, 1, value);
}

void ResourceShader::SetUniform1f(std::string name, GLfloat value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform1f(uinformLoc, value);
}

void ResourceShader::SetUniform1i(std::string name, GLint value)
{
	uint uinformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform1i(uinformLoc, value);
}
