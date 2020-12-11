#include "Globals.h"

struct aiScene;
struct aiNode;
class GameObject;
class ResourceScene;
class ResourceMesh;
class ResourceMaterial;
namespace Importer
{
	namespace ModelImporter
	{
		const aiScene* ImportAssimpScene(const char* buffer, uint size);
		void ImportScene(const aiScene* aiScene, ResourceScene* scene);
		void IterateNodes( const aiScene* scene, aiNode* node, ResourceScene* resourceScene, uint32 ID);

		uint32 Save(const ResourceScene* scene, char** buffer);
		void Load(ResourceScene* resourceScene, char* buffer);

	}

	namespace SceneImporter
	{
		uint32 Save(const ResourceScene* resourceScene, char** buffer);
		void Load(ResourceScene* resourceScene, char* buffer);
	}
}