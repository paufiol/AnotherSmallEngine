#include "Globals.h"
#include <string.h>

struct aiScene;
struct aiNode;
class GameObject;

namespace Importer
{
	namespace SceneImporter
	{
		void ImportScene(const char* scenePath);
		void IterateNodes(const char* scenePath, const aiScene* scene, aiNode* node, GameObject* parent);

		

	}
}