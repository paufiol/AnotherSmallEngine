#ifndef __ResourceModel_H__
#define __ResourceModel_H__

#include "ResourceMesh.h"

struct	aiScene;
struct	aiNode;
struct	aiMesh;
struct	aiMaterial;
enum	aiTextureType;
using namespace std;


class ResourceModel
{
public:

    ResourceModel(const char* path);
    void Draw();

private:
    void ImportModel(const char* path);  // loads a model  from file and stores the meshes in the meshes vector.
    void processNode(aiNode* node, const aiScene* scene); // processes each mesh located at the node 
    ResourceMesh processMesh(aiMesh* mesh, const aiScene* scene);

public:

    vector<ResourceMesh>   meshes;

};

#endif //__ResourceModel_H__