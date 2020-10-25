#ifndef __ResourceModel_H__
#define __ResourceModel_H__

#include "ResourceMesh.h"

#pragma comment( lib, "Dependencies/Devil/libx86/DevIL.lib" )
#include "Dependencies/Devil\include\ilu.h"
#pragma comment( lib, "Dependencies/Devil/libx86/ILU.lib" )
#include "Dependencies/Devil\include\ilut.h"
#pragma comment( lib, "Dependencies/Devil/libx86/ILUT.lib" )

struct	aiScene;
struct	aiNode;
struct	aiMesh;
struct	aiMaterial;
enum	aiTextureType;
using namespace std;

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64
class ResourceModel 
{
public:

    ResourceModel(const char* path);
    void Draw();

    void SetupTexture(const char* path);
    void SetCheckerImage();
private:
    void ImportModel(const char* path);  // loads a model  from file and stores the meshes in the meshes vector.
    void processNode(aiNode* node, const aiScene* scene); // processes each mesh located at the node 
    ResourceMesh processMesh(aiMesh* mesh, const aiScene* scene);



public:

    vector<ResourceMesh>   meshes;
    vector<Texture>        textures_loaded;

    GLuint                 texture_ID = 0;
    GLubyte                checker_image[64][64][4];

};

#endif //__ResourceModel_H__