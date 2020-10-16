#ifndef __Meshes_H__
#define __Meshes_H__

#include "OpenGL.h"
#include "Globals.h"

#include "Dependencies/MathGeoLib/include/MathBuildConfig.h"
#include "Dependencies/MathGeoLib/include/MathGeoLibFwd.h"
#include "Dependencies/MathGeoLib/include/MathGeoLib.h"


#include <string>
#include <vector>
using namespace std;

struct Vertex {
    float3 Position;
    float3 Normal;
    float2 TexCoords;
    float3 Tangent;
    float3 Bitangent;
};

struct Texture {
    uint id;
    string type;
    string path;
};

class Mesh {
public:
    vector<Vertex>       vertices;
    vector<uint>           indices;
    vector<Texture>      textures;
    uint VAO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
};
#endif //__Meshes_H__