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

        //Why no entrar
        tempMesh->CreateAABB();

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

uint64 Importer::MeshImporter::Save(const ResourceMesh* mesh, char** buffer)
{
    char* cursor; // Used to point where to write the next chunk of information
    uint bytes;   // The amount of bytes each parameter use
    uint fullSize;// The amount of bytes it takes to save the mesh

    fullSize = sizeof(mesh->size) + sizeof(uint)
        * mesh->size[ResourceMesh::index] + sizeof(uint)
        * mesh->size[ResourceMesh::vertex] + sizeof(float) * 3
        * mesh->size[ResourceMesh::normal] + sizeof(float) * 3
        * mesh->size[ResourceMesh::texture] + sizeof(float) * 2;

    *buffer = new char[fullSize];
    cursor = *buffer;

    // Store ranges
    bytes = sizeof(mesh->size);
    memcpy(cursor, mesh->size, bytes);
    cursor += bytes;

    // Store indices
    bytes = sizeof(uint) * mesh->size[ResourceMesh::index];
    memcpy(cursor, mesh->indices, bytes);
    cursor += bytes;

    // Store vertices
    bytes = sizeof(uint) * mesh->size[ResourceMesh::vertex];
    memcpy(cursor, mesh->vertices, bytes);
    cursor += bytes;

    // Store normals
    bytes = sizeof(uint) * mesh->size[ResourceMesh::normal];
    memcpy(cursor, mesh->normals, bytes);
    cursor += bytes;

    // Store Texture Coordinates
    bytes = sizeof(uint) * mesh->size[ResourceMesh::texture];
    memcpy(cursor, mesh->texCoords, bytes);
    cursor += bytes;

    return fullSize;
}

void Importer::MeshImporter::Load(ResourceMesh* mesh, char* buffer)
{
    char* cursor = buffer;
    uint bytes;

    bytes = sizeof(mesh->size);
    memcpy(mesh->size, cursor, bytes);
    cursor += bytes;

    // Load indices
    bytes = sizeof(uint) * mesh->size[ResourceMesh::index];
    mesh->indices = new uint[mesh->size[ResourceMesh::index]];
    memcpy(mesh->indices, cursor, bytes);
    cursor += bytes;

    // Load vertices
    bytes = sizeof(float) * mesh->size[ResourceMesh::vertex];
    mesh->vertices = new float[mesh->size[ResourceMesh::vertex]];
    memcpy(mesh->vertices, cursor, bytes);
    cursor += bytes;

    // Load normals
    bytes = sizeof(float) * mesh->size[ResourceMesh::normal];
    mesh->normals = new float[mesh->size[ResourceMesh::normal]];
    memcpy(mesh->normals, cursor, bytes);
    cursor += bytes;

    // Load Texture Coordinates
    bytes = sizeof(float) * mesh->size[ResourceMesh::texture];
    mesh->texCoords = new float[mesh->size[ResourceMesh::texture]];
    memcpy(mesh->texCoords, cursor, bytes);
    cursor += bytes;
}