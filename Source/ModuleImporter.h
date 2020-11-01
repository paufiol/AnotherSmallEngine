#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include <vector>
#include "OpenGL.h"

using namespace std;

struct Mesh
{
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

};

struct Texture
{
	GLuint id;
	const char* path;
};
namespace Importer
{
	namespace MeshImporter
	{
		void Import(const char* file);

		vector<Mesh*> meshes;
	}

	namespace TextureImporter
	{
		uint Import(const char* file);
		void InitDevil();
		//GLuint Gl_Tex;
		Texture texture; 
	}
}




#endif //__ModuleImporter_H__
