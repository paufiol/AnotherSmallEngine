#ifndef __ImporterMaterials__
#define __ImporterMaterials__

#include "Globals.h"

struct aiMaterial;
class ResourceMaterial;
class GameObject;

namespace Importer
{
	namespace MaterialsImporter
	{
		
		void ImportMaterial(aiMaterial* material, GameObject* tempObject);
		uint64 Save(ResourceMaterial* rMaterial, char** buffer);
		void Load(ResourceMaterial* rMaterial, char* buffer);

	}

	namespace TextureImporter
	{
		ResourceMaterial* ImportTexture(const char* file);
		void InitDevil();
	}
}
#endif //__ImporterMaterials__