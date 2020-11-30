#include "Application.h"
#include "ImporterMesh.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleScene.h"
#include "ResourceMesh.h"

#include "Dependencies/Assimp/include/mesh.h"
#include "Dependencies/Assimp/include/cimport.h"
#include "Dependencies/Assimp/include/scene.h"
#include "Dependencies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependencies/Assimp/libx86/assimp.lib")

vector<ResourceMesh*> Importer::MeshImporter::LoadMeshes(const aiScene* scene, const aiNode* node)
{
    vector<ResourceMesh*> meshes;
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        ResourceMesh* tempMesh = new ResourceMesh();

        tempMesh->size[ResourceMesh::vertex] = scene->mMeshes[node->mMeshes[i]]->mNumVertices;
        tempMesh->vertices = new float[tempMesh->size[ResourceMesh::vertex] * 3];
        memcpy(tempMesh->vertices, scene->mMeshes[node->mMeshes[i]]->mVertices, sizeof(float) * tempMesh->size[ResourceMesh::vertex] * 3);
        LOG("New mesh with %d vertices", tempMesh->size[ResourceMesh::vertex]);

        if (scene->mMeshes[node->mMeshes[i]]->HasFaces())
        {
            tempMesh->size[ResourceMesh::index] = scene->mMeshes[node->mMeshes[i]]->mNumFaces * 3;
            tempMesh->indices = new uint[tempMesh->size[ResourceMesh::index]];
            for (uint f = 0; f < scene->mMeshes[node->mMeshes[i]]->mNumFaces; ++f)
            {
                if (scene->mMeshes[node->mMeshes[i]]->mFaces[f].mNumIndices != 3)
                {
                    LOG("Mesh face with less or more than 3 indices!");
                }
                else
                {
                    memcpy(&tempMesh->indices[f * 3], scene->mMeshes[node->mMeshes[i]]->mFaces[f].mIndices, 3 * sizeof(uint));
                }
            }

            LOG("With %d indices", tempMesh->size[ResourceMesh::index]);
        }

        if (scene->mMeshes[node->mMeshes[i]]->HasNormals())
        {
            tempMesh->size[ResourceMesh::normal] = scene->mMeshes[node->mMeshes[i]]->mNumVertices;
            tempMesh->normals = new float[tempMesh->size[ResourceMesh::normal] * 3];
            memcpy(tempMesh->normals, scene->mMeshes[node->mMeshes[i]]->mNormals, sizeof(float) * tempMesh->size[ResourceMesh::normal] * 3);
        }

        if (scene->mMeshes[node->mMeshes[i]]->HasTextureCoords(0))
        {
            tempMesh->size[ResourceMesh::texture] = scene->mMeshes[node->mMeshes[i]]->mNumVertices;
            tempMesh->texCoords = new float[scene->mMeshes[node->mMeshes[i]]->mNumVertices * 2];

            for (int j = 0; j < tempMesh->size[ResourceMesh::texture]; j++)
            {
                tempMesh->texCoords[j * 2] = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].x;
                tempMesh->texCoords[j * 2 + 1] = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].y;
            }
        }
        tempMesh->SetUpBuffers(tempMesh);
        meshes.push_back(tempMesh);
    }
    return meshes;
}