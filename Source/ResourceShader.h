#ifndef __ResourceShader_H__
#define __ResourceShader_H__

#include "Resource.h"

class ResourceShader : public Resource
{
public:

	ResourceShader(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID);
	~ResourceShader();

public:

	uint32 shaderProgramID = 0;
	uint32 vertexID = 0;
	uint32 fragmentID = 0;

};
#endif //__ResourceShader_H__
