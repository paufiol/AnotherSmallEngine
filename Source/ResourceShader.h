#ifndef __ResourceShader_H__
#define __ResourceShader_H__

#include "Resource.h"

class ResourceShader : public Resource
{
public:

	ResourceShader();
	~ResourceShader();



public:

	uint32 shaderProgram = 0;

};
#endif //__ResourceShader_H__
