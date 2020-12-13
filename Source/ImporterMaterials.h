#ifndef __ImporterMaterials__
#define __ImporterMaterials__

#include "Globals.h"

struct aiMaterial;
class ResourceMaterial;
class ResourceTexture;

namespace Importer
{
	namespace MaterialsImporter
	{
		
		void ImportMaterial(aiMaterial* material, ResourceMaterial* resourceMaterial);
		uint64 Save(ResourceMaterial* rMaterial, char** buffer);
		void Load(ResourceMaterial* rMaterial, const char* buffer);

	}

	namespace TextureImporter
	{
		void InitDevil();
		void ImportTexture(ResourceTexture* rMaterial, const char* buffer, uint size);
		uint64 Save(const ResourceTexture* rMaterial, char** buffer);
		void Load(ResourceTexture* rMaterial, char* buffer, uint size);
	}
}
#endif //__ImporterMaterials__