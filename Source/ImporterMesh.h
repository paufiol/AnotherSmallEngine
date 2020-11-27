#ifndef __ImporterMesh__
#define __ImporterMesh__

#include "Globals.h"
#include <string.h>
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


	}
}


#endif //__ImporterMesh__