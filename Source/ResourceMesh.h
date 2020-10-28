#ifndef __ResourceMesh_H__
#define __ResourceMesh_H__

#include "glmath.h"
#include <OpenGL.h>
#include "Globals.h"
#include "Dependencies/MathGeoLib/include/MathBuildConfig.h"
#include "Dependencies/MathGeoLib/include/MathGeoLibFwd.h"
#include "Dependencies/MathGeoLib/include/MathGeoLib.h"

using namespace std;

struct Vertex {

    vec3 position;
	vec3 normal;
    vec2 texCoords;
};

struct Texture {
    uint id;
    string type;
    string path;
};

class ResourceMesh {
public:
    vector<Vertex>       vertices;
    vector<uint>         indices;
    vector<Texture>      textures;

    ResourceMesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures);
    ~ResourceMesh();

    void Draw(GLuint id, GLubyte checker_image[64][64][4]);
    void DrawNormals();
    void DrawTexCoords();

private:
	uint VAO = 0;											// Vertex Array
	uint VBO = 0;											// Vertex Buffer
	uint NBO = 0;											// Normal Buffer
	uint TBO = 0;											// Texture Buffer
	uint IBO = 0;
    uint EBO = 0;

    void setupMesh();

};
#endif //__ResourceMesh_H__