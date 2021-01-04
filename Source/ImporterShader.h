#ifndef __ImporterShader__
#define __ImporterShader__

#include "Globals.h"

class ResourceShader;

namespace Importer
{
	namespace ShaderImporter
	{
		void Import(const char* path, ResourceShader* shader);

		int ImportVertex(std::string shaderFile, ResourceShader* shader);

		int ImportFragment(std::string shaderFile, ResourceShader* shader);

		void GetShaderUniforms(ResourceShader* shader);

		void SetShaderUniforms(ResourceShader* shader);

		uint64 Save(const ResourceShader* shader, char** buffer);

		void Load(ResourceShader* shader, const char* buffer, uint size = 0);

		void Recompile(ResourceShader* shader);
	}
}


#endif //__ImporterShader__
