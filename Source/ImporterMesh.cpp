#include "Application.h"
#include "ImporterMesh.h"
#include "ModuleFileSystem.h"

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

void Importer::MeshImporter::LoadMeshes(ResourceMesh* mesh, const aiMesh* aiMesh)
{
    mesh->size[ResourceMesh::vertex] = aiMesh->mNumVertices;
    mesh->vertices = new float[mesh->size[ResourceMesh::vertex] * 3];
    memcpy(mesh->vertices, aiMesh->mVertices, sizeof(float) * mesh->size[ResourceMesh::vertex] * 3);
    LOG("New mesh with %d vertices", mesh->size[ResourceMesh::vertex]);

    mesh->CreateAABB();

    if (aiMesh->HasFaces())
    {
        mesh->size[ResourceMesh::index] = aiMesh->mNumFaces * 3;
        mesh->indices = new uint[mesh->size[ResourceMesh::index]];
        for (uint f = 0; f < aiMesh->mNumFaces; ++f)
        {
            if (aiMesh->mFaces[f].mNumIndices != 3)
            {
                LOG("Mesh face with less or more than 3 indices!");
            }
            else
            {
                memcpy(&mesh->indices[f * 3], aiMesh->mFaces[f].mIndices, 3 * sizeof(uint));
            }
        }

        LOG("With %d indices", mesh->size[ResourceMesh::index]);
    }

    if (aiMesh->HasNormals())
    {
        mesh->size[ResourceMesh::normal] = aiMesh->mNumVertices;
        mesh->normals = new float[mesh->size[ResourceMesh::normal] * 3];
        memcpy(mesh->normals, aiMesh->mNormals, sizeof(float) * mesh->size[ResourceMesh::normal] * 3);
    }

    if (aiMesh->HasTextureCoords(0))
    {
        mesh->size[ResourceMesh::texture] = aiMesh->mNumVertices;
        mesh->texCoords = new float[aiMesh->mNumVertices * 2];

        for (int j = 0; j < mesh->size[ResourceMesh::texture]; j++)
        {
            mesh->texCoords[j * 2] = aiMesh->mTextureCoords[0][j].x;
            mesh->texCoords[j * 2 + 1] = aiMesh->mTextureCoords[0][j].y;
        }
    }
    
    
    
    mesh->SetUpBuffers(mesh); // This have to go out
    //meshes.push_back(tempMesh);

    /*char* buffer = nullptr;
    uint64 size = Importer::MeshImporter::Save(tempMesh, &buffer);*/
}

uint64 Importer::MeshImporter::Save(const ResourceMesh* mesh, char** buffer)
{
    //char* cursor; // Used to point where to write the next chunk of information
    //uint bytes;   // The amount of bytes each parameter use
    //uint fullSize;// The amount of bytes it takes to save the mesh

    uint ranges[4] = { 
        mesh->size[ResourceMesh::index], 
        mesh->size[ResourceMesh::vertex], 
        mesh->size[ResourceMesh::normal],
        mesh->size[ResourceMesh::texture] 
    };

    uint fullSize = sizeof(ranges)
        + mesh->size[ResourceMesh::index] * sizeof(uint)
        + mesh->size[ResourceMesh::vertex] * sizeof(float) * 3
        + mesh->size[ResourceMesh::normal] * sizeof(float) * 3
        + mesh->size[ResourceMesh::texture] * sizeof(float) * 2;


    *buffer = new char[fullSize];
    char* cursor = *buffer;


    uint bytes = sizeof(ranges);
    memcpy(cursor, ranges, bytes);
    cursor += bytes;

    bytes = sizeof(uint) * mesh->size[ResourceMesh::index];
    memcpy(cursor, mesh->indices, bytes);
    cursor += bytes;

    bytes = sizeof(float) * mesh->size[ResourceMesh::vertex] * 3;
    memcpy(cursor, mesh->vertices, bytes);
    cursor += bytes;

    if (mesh->size[ResourceMesh::normal] > 0)
    {
        bytes = sizeof(float) * mesh->size[ResourceMesh::normal] * 3;
        memcpy(cursor, mesh->normals, bytes);
        cursor += bytes;
    }


    if (mesh->size[ResourceMesh::texture] > 0)
    {
        bytes = sizeof(float) * mesh->size[ResourceMesh::texture] * 2;
        memcpy(cursor, mesh->texCoords, bytes);
        cursor += bytes;
    }


    //string path = MESHES_PATH + std::to_string(mesh->GetUID()) + MESH_EXTENSION;
    //App->fileSystem->Save(path.c_str(), *buffer, fullSize);

    return fullSize;
}

void Importer::MeshImporter::Load(ResourceMesh* mesh, const char* buffer)
{
    const char*  cursor = buffer;
    uint bytes;

    uint ranges[4];
    bytes = sizeof(ranges);
    memcpy(ranges, cursor, bytes);
    cursor += bytes;

    mesh->size[ResourceMesh::index] = ranges[0];
    mesh->size[ResourceMesh::vertex] = ranges[1];
    mesh->size[ResourceMesh::normal] = ranges[2];
    mesh->size[ResourceMesh::texture] = ranges[3];

    // Load indices
    bytes = sizeof(uint) * mesh->size[ResourceMesh::index];
    mesh->indices = new uint[mesh->size[ResourceMesh::index]];
    memcpy(mesh->indices, cursor, bytes);
    cursor += bytes;

    // Load vertices
    bytes = sizeof(float) * mesh->size[ResourceMesh::vertex] * 3;
    mesh->vertices = new float[mesh->size[ResourceMesh::vertex] * 3];
    memcpy(mesh->vertices, cursor, bytes);
    cursor += bytes;

    // Load normals
    if (mesh->size[ResourceMesh::normal] > 0)
    {
        bytes = sizeof(float) * mesh->size[ResourceMesh::normal] * 3;
        mesh->normals = new float[mesh->size[ResourceMesh::normal] * 3];
        memcpy(mesh->normals, cursor, bytes);
        cursor += bytes;
    }

    // Load Texture Coordinates
    if (mesh->size[ResourceMesh::texture] > 0)
    {
        bytes = sizeof(float) * mesh->size[ResourceMesh::texture] * 2;
        mesh->texCoords = new float[mesh->size[ResourceMesh::texture] * 2];
        memcpy(mesh->texCoords, cursor, bytes);
        cursor += bytes;
    }

    mesh->SetUpBuffers(mesh);
    mesh->CreateAABB();
   
}