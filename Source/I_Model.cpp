#include "I_Model.h"

#include "Dependencies/Assimp/include/mesh.h"
#include "Dependencies/Assimp/include/cimport.h"
#include "Dependencies/Assimp/include/scene.h"
#include "Dependencies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependencies/Assimp/libx86/assimp.lib")

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

I_Model::I_Model()
{

}

void I_Model::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw();
    }
}

void I_Model::loadModel(const char* path)
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

void I_Model::processNode(aiNode* node, const aiScene* scene)
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

Meshes I_Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
   
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		
        // normals
        if (mesh->HasNormals())
        {
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) 
        {
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
            vertex.TexCoords = vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
		
    }
    // indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{

		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
   
	Meshes tempMesh (vertices, indices, textures);
    return tempMesh;
}






