#ifndef __ImporterMesh__
#define __ImporterMesh__

#include "Globals.h"

using namespace std;

struct aiScene;
struct aiNode;
struct aiMesh;
class ResourceMesh;

namespace Importer
{
	namespace MeshImporter
	{
		void LoadMeshes(ResourceMesh* mesh, const aiMesh* aiMesh);

		uint64 Save(const ResourceMesh* mesh, char** buffer);

		void Load(ResourceMesh* mesh, const char* buffer);
	}
}


#endif //__ImporterMesh__