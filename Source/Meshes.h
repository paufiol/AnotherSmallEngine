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
	uint VAO;											// Vertex Array
	uint VBO;											// Vertex Buffer
	uint NBO;											// Normal Buffer
	uint TBO;											// Texture Buffer
	uint IBO;
    void setupMesh();

};
#endif //__Meshes_H__