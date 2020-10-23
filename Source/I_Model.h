#ifndef __I_Model_H__
#define __I_Model_H__

#include "Meshes.h"

struct	aiScene;
struct	aiNode;
struct	aiMesh;
struct	aiMaterial;
enum	aiTextureType;
using namespace std;


class I_Model
{
public:
    vector<Texture>  textures_loaded;
    vector<Meshes>   meshes;
    string directory;
    bool gammaCorrection;

    I_Model();
    void Draw();
    void loadModel(const char* path);  // loads a model  from file and stores the meshes in the meshes vector.

private:
   
    void processNode(aiNode* node, const aiScene* scene); // processes each mesh located at the node 
    Meshes processMesh(aiMesh* mesh, const aiScene* scene);

};

#endif //__I_Model_H__
