#include "I_Model.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "Dependencies/Assimp/include/mesh.h"
#include "Dependencies/Assimp/include/cimport.h"
#include "Dependencies/Assimp/include/scene.h"
#include "Dependencies/Assimp/include/postprocess.h"

#pragma comment (lib, "Dependencies/Assimp/libx86/assimp.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

I_Model::I_Model()
{

}

void I_Model::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw();
        LOG("DRAWING MESHES");
    }
}

void I_Model::loadModel(const char* path)
{
    LOG("Loading Model from file: %s", path);

    // read file via ASSIMP
    //Assimp::Importer importer;
    //const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
    // Safecheck
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        LOG("ERROR ASSIMP %s", aiGetErrorString());
        return;
    }
 

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void I_Model::processNode(aiNode* node, const aiScene* scene)
{
	
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
		LOG("MESH PROCESSED");
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
        
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
		LOG("NODE PROCESSED");
		processNode(node->mChildren[i], scene);
    }

}

Meshes I_Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

    // walk through each of the mesh's vertices
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
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
            vertex.TexCoords = vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
		
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{

		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	Meshes tempMesh (vertices, indices, textures);
    return tempMesh;
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
vector<Texture> I_Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    //for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    //{
    //    aiString str;
    //    mat->GetTexture(type, i, &str);
    //    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
    //    bool skip = false;
    //    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    //    {
    //        if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
    //        {
    //            textures.push_back(textures_loaded[j]);
    //            skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
    //            break;
    //        }
    //    }
    //    if (!skip)
    //    {   // if texture hasn't been loaded already, load it
    //        Texture texture;
    //        texture.id = TextureFromFile(str.C_Str(), directory);
    //        texture.type = typeName;
    //        texture.path = str.C_Str();
    //        textures.push_back(texture);
    //        textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
    //    }
    //}
    return textures;
}


uint TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    uint textureID;
 /*   glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }*/

    return textureID;
}


