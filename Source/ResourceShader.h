#ifndef __ResourceShader_H__
#define __ResourceShader_H__

#include "Globals.h"
#include "Resource.h"
#include "OpenGL.h"

class ResourceShader : public Resource
{
public:

	ResourceShader(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID);
	ResourceShader();
	~ResourceShader();

	void SetUniformMatrix4(std::string name, GLfloat* value);
	void SetUniform4f(std::string name, GLfloat* value);
	void SetUniform1f(std::string name, GLfloat value);
	void SetUniform1i(std::string name, GLint value);


public:

	uint32 shaderProgramID = 0;
	uint32 vertexID = 0;
	uint32 fragmentID = 0;

};
#endif //__ResourceShader_H__
