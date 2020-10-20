#ifndef __Meshes_H__
#define __Meshes_H__

#include "Shaders.h"

#include "Dependencies/MathGeoLib/include/MathBuildConfig.h"
#include "Dependencies/MathGeoLib/include/MathGeoLibFwd.h"
#include "Dependencies/MathGeoLib/include/MathGeoLib.h"

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

class Meshes {
public:
    vector<Vertex>       vertices;
    vector<uint>          indices;
    vector<Texture>      textures;
    uint VAO;

    Meshes(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    ~Meshes();
    void Draw(Shaders shader);

private:
    // render data 
    unsigned int VBO, EBO;
    void setupMesh();
};
#endif //__Meshes_H__