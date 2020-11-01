#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImporter.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleSceneIntro.h"

#include "Dependencies/Assimp/include/mesh.h"
#include "Dependencies/Assimp/include/cimport.h"
#include "Dependencies/Assimp/include/scene.h"
#include "Dependencies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependencies/Assimp/libx86/assimp.lib")

#include "Dependencies/Devil/Include/ilut.h"
#include "Dependencies/Devil/Include/ilu.h"
#include "Dependencies/Devil/Include/il.h"

#pragma comment (lib, "Dependencies/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Dependencies/Devil/libx86/ILU.lib")
#pragma comment (lib, "Dependencies/Devil/libx86/ILUT.lib")

void Importer::TextureImporter::InitDevil()
{
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
}

void Importer::MeshImporter::Import(const char* file)
{
    const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);

    if (scene != nullptr && scene->HasMeshes())
    {
        string tempName = "Game Object ";
        string stringSize = to_string(App->scene_intro->game_objects.size());
        tempName += stringSize;
        GameObject* tempGameObj = new GameObject(tempName);

        // Use scene->mNumMeshes to iterate on scene->mMeshes array
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            Mesh* tempMesh = new Mesh();
            std::string tmpString = "";
            tmpString.append("New_Obj ");
            
            ComponentMesh* tempComponentMesh = new ComponentMesh(tempGameObj);

            tempMesh->size[Mesh::vertex] = scene->mMeshes[i]->mNumVertices;
            tempMesh->vertices = new float[tempMesh->size[Mesh::vertex] * 3];
            memcpy(tempMesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * tempMesh->size[Mesh::vertex] * 3);
            LOG("New mesh with %d vertices", tempMesh->size[Mesh::vertex]);

            if (scene->mMeshes[i]->HasFaces())
            {
                tempMesh->size[Mesh::index] = scene->mMeshes[i]->mNumFaces * 3;
                tempMesh->indices = new uint[tempMesh->size[Mesh::index]];
                for (uint f = 0; f < scene->mMeshes[i]->mNumFaces; ++f)
                {
                    if (scene->mMeshes[i]->mFaces[f].mNumIndices != 3)
                    {
                        LOG("WARNING, geometery face with != 3 indices!");
                    }
                    else
                    {
                        memcpy(&tempMesh->indices[f * 3], scene->mMeshes[i]->mFaces[f].mIndices, 3 * sizeof(uint));
                    }
                }
                LOG("With %d indices", tempMesh->size[Mesh::index]);
            }

            if (scene->mMeshes[i]->HasNormals())
            {
                tempMesh->size[Mesh::normal] = scene->mMeshes[i]->mNumVertices;
                tempMesh->normals = new float[tempMesh->size[Mesh::normal] * 3];
                memcpy(tempMesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * tempMesh->size[Mesh::normal] * 3);
            }

            if (scene->mMeshes[i]->HasTextureCoords(0))
            {
                tempMesh->size[Mesh::texture] = scene->mMeshes[i]->mNumVertices;
                tempMesh->texCoords = new float[scene->mMeshes[i]->mNumVertices * 2];

                for (int j = 0; j < tempMesh->size[Mesh::texture]; j++)
                {
                    tempMesh->texCoords[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
                    tempMesh->texCoords[j * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
                }
            }
            tempComponentMesh->SetMesh(tempMesh);
            tempComponentMesh->SetPath(file);

            tempGameObj->AddComponent(tempComponentMesh);

            

            App->renderer3D->SetUpBuffers(tempMesh);
            meshes.push_back(tempMesh);
        }
        
        App->scene_intro->AddGameObject(tempGameObj);
        aiReleaseImport(scene);
    }
    else
    {
        LOG("Error loading scene %s", file);
    }
}

uint Importer::TextureImporter::Import(const char* path)
{  
    ILuint Il_Tex;
    uint tempid;
    ilGenImages(1, &Il_Tex);
    ilBindImage(Il_Tex);
    ilLoadImage(path);
    tempid = ilutGLBindTexImage();
    ilDeleteImages(1, &Il_Tex);
    if (Il_Tex != NULL)
    {
        LOG("Successfuly loaded %s texture", path);
    }
    else {
        LOG("Error loading the texture!");
    }

    return tempid;
}

