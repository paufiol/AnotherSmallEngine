#ifndef __ResourceMesh_H__
#define __ResourceMesh_H__

#include "Globals.h"
#include "Dependencies/MathGeoLib/include/Geometry/AABB.h"

using namespace std;


class ResourceMesh 
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
    ~ResourceMesh();

    void DrawNormals();
    void DrawTexCoords();
	void SetUpBuffers(ResourceMesh* mesh);
	void CreateAABB();

};
#endif //__ResourceMesh_H__