#ifndef __ImporterShader__
#define __ImporterShader__

#include "Globals.h"

class ResourceShader;

namespace Importer
{
	namespace ShaderImporter
	{
		void Import(const char* path, ResourceShader* shader);

		int ImportVertex(const char* buffer, ResourceShader* shader);

		int ImportFragment(const char* buffer, ResourceShader* shader);

		uint64 Save(const ResourceShader* shader, char** buffer);

		void Load(ResourceShader* shader, const char* buffer);
	}
}


#endif //__ImporterShader__
