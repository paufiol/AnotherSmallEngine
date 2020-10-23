#ifndef __Meshes_H__
#define __Meshes_H__

#include "glmath.h"
#include <OpenGL.h>
#include "Globals.h"
#include "Dependencies/MathGeoLib/include/MathBuildConfig.h"
#include "Dependencies/MathGeoLib/include/MathGeoLibFwd.h"
#include "Dependencies/MathGeoLib/include/MathGeoLib.h"

using namespace std;

struct Vertex {

    vec3 Position;
	vec3 Normal;
    vec2 TexCoords;
};

struct Texture {
    uint id;
    string type;
    string path;
};

class Meshes {
public:
    vector<Vertex>       vertices;
    vector<uint>          indices;
    vector<Texture>      textures;

    Meshes(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures);
    ~Meshes();
    void Draw();

private:
    // render data 
	uint VAO = 0;											// Vertex Array
	uint VBO = 0;											// Vertex Buffer
	uint NBO = 0;											// Normal Buffer
	uint TBO = 0;											// Texture Buffer
	uint IBO = 0;
    uint EBO = 0;
    void setupMesh();

};
#endif //__Meshes_H__