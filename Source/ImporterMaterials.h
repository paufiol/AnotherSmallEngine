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
		void ImportTexture(ResourceTexture* rMaterial = nullptr, const char* buffer = nullptr, uint size = 0);
		uint64 Save(const ResourceTexture* rMaterial = nullptr, char** buffer = nullptr);
		void Load(ResourceTexture* rMaterial = nullptr, char* buffer = nullptr, uint size = 0);
	}
}
#endif //__ImporterMaterials__