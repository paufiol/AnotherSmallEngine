#include "Globals.h"

struct aiScene;
struct aiNode;
class GameObject;

namespace Importer
{
	namespace SceneImporter
	{
		void ImportScene(const char* path);
		void IterateNodes(aiScene* scene, aiNode* node, GameObject* parent);

		

	}
}