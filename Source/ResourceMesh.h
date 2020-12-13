#ifndef __ResourceMesh_H__
#define __ResourceMesh_H__

#include "Globals.h"
#include "Dependencies/MathGeoLib/include/Geometry/AABB.h"
#include "Resource.h"
using namespace std;


class ResourceMesh : public Resource
{
public:

	uint* indices = nullptr;
	float* vertices = nullptr;
	float* normals = nullptr;
	float* texCoords = nullptr;

	enum  Buffers
	{
		index,
		vertex,
		normal,
		texture,
		NONE
	};
	uint ID[NONE];
	uint size[NONE];

	AABB aabb;
public:

	ResourceMesh(); 
    ResourceMesh(const char* assetsFile, const char* libraryFile, const char* name, uint32 UID);
    ~ResourceMesh();

    void DrawNormals();
    void DrawTexCoords();
	void SetUpBuffers(ResourceMesh* mesh);
	void CreateAABB();

};
#endif //__ResourceMesh_H__