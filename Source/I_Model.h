#ifndef __I_Model_H__
#define __I_Model_H__



#include "Globals.h"
#include "Meshes.h"
#include "Dependencies/Assimp/Assimp/include/Importer.hpp"
#include "Dependencies/Assimp/Assimp/include/scene.h"
#include "Dependencies/Assimp/Assimp/include/postprocess.h"
#pragma comment (lib, "Dependencies/Assimp/Assimp/libx86/assimp.lib")
using namespace std;

uint TextureFromFile(const char* path, const string& directory, bool gamma = false);
class I_Model
{
public:
    vector<Texture>  textures_loaded;
    vector<Meshes>   meshes;
    string directory;
    bool gammaCorrection;

    I_Model(string const& path, bool gamma = false);
    void Draw(Shaders& shader);
    void loadModel(string const& path);  // loads a model  from file and stores the meshes in the meshes vector.

private:
   
    void processNode(aiNode* node, const aiScene* scene); // processes each mesh located at the node 
    Meshes processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName); //checks all textures of a given type and loads them

   // uint TextureFromFile(const char* path, const string& directory, bool gamma = false);
};

#endif //__I_Model_H__
