#include "ResourceModel.h"
#include "ModuleFileSystem.h"
#include "Application.h"

#include "Dependencies/Assimp/include/mesh.h"
#include "Dependencies/Assimp/include/cimport.h"
#include "Dependencies/Assimp/include/scene.h"
#include "Dependencies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependencies/Assimp/libx86/assimp.lib")



ResourceModel::ResourceModel(const char* path)
{
    ImportModel(path);
    
}

void ResourceModel::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(texture_ID, checker_image);
    }
}

void ResourceModel::ImportModel(const char* path)
{
    LOG("Loading Model from file: %s", path);

    const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // Safecheck
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG("ERROR ASSIMP %s", aiGetErrorString());
        return;
    }
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void ResourceModel::processNode(aiNode* node, const aiScene* scene)
{

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

ResourceMesh ResourceModel::processMesh(aiMesh* mesh, const aiScene* scene)
{

    vector<Vertex> vertices;
    vector<uint> indices;
    vector<Texture> textures;

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {

        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
            
        // normals
        if (mesh->HasNormals())
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.texCoords = vec2(0.0f, 0.0f);
            LOG("Failed to load Texture Coordinates.");
        }
            

        vertices.push_back(vertex);

    }
    // indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {

        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    ResourceMesh tempMesh(vertices, indices, textures);
    return tempMesh;
}
void ResourceModel::SetCheckerImage()
{
    for (int i = 0; i < CHECKERS_WIDTH; i++) {
        for (int j = 0; j < CHECKERS_HEIGHT; j++) {
            int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
            checker_image[i][j][0] = (GLubyte)c;
            checker_image[i][j][1] = (GLubyte)c;
            checker_image[i][j][2] = (GLubyte)c;
            checker_image[i][j][3] = (GLubyte)255;
        }
    }
}
void ResourceModel::ApplyCheckerImage()
{
    SetCheckerImage();

    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &default_tex_ID);
    glBindTexture(GL_TEXTURE_2D, default_tex_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
        0, GL_RGBA, GL_UNSIGNED_BYTE, checker_image);

    glBindTexture(GL_TEXTURE_2D, 0);
}
void ResourceModel::SetupTexture(const char* path)
{
    string full_path = string(TEXTURES_FOLDER) + '/' + string(path);
   
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);

    //glClearColor(0.0, 0.0, 0.0, 0.0);
    

    ILuint devil_tex;
    ilGenImages(1, &devil_tex);
    ilBindImage(devil_tex);
    if (ilLoadImage(full_path.c_str()) == IL_FALSE)
    {
        //Check if the is any error
        ILenum error = ilGetError();
        LOG("ERROR: %s", iluErrorString(error));

    }
    texture_ID = ilutGLBindTexImage();
    LOG("Texture_ID: %d", texture_ID);
    ilDeleteImages(1, &devil_tex);

    
    SetCheckerImage();

    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
        0, GL_RGBA, GL_UNSIGNED_BYTE, checker_image);


    
}