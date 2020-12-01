#ifndef __ImporterMesh__
#define __ImporterMesh__

#include "Globals.h"
#include <string>
#include <vector>

using namespace std;

struct aiScene;
struct aiNode;
class ResourceMesh;

namespace Importer
{
	namespace MeshImporter
	{
		vector<ResourceMesh*> LoadMeshes(const aiScene* scene, const aiNode* node);

		uint64 Save(const ResourceMesh* mesh, char** buffer);

		void Load(ResourceMesh* mesh, char* buffer);
	}
}


#endif //__ImporterMesh__