#include "Globals.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "ImporterMaterials.h"

#include "GameObject.h"

#include "ComponentTexture.h"

#include "ResourceMaterial.h"

#include "Dependencies/Assimp/include/material.h"
#include "Dependencies/Assimp/include/texture.h"


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

void Importer::MaterialsImporter::ImportMaterial(aiMaterial* material, GameObject* tempObject)
{

	ResourceMaterial* rMaterial = new ResourceMaterial();

	aiColor4D	color;
	aiString	texPath;
	std::string		texName;
	std::string		texExtension;

	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)										// Could also get specular and ambient occlusion colours.
	{
		rMaterial->SetColor(Color(color.r, color.g, color.b, color.a));
	}
	if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
	{
		App->fileSystem->SplitFilePath(texPath.C_Str(), nullptr, &texName, &texExtension);

		texName = "Assets/Textures/" + texName + "." + texExtension;

		rMaterial = Importer::TextureImporter::ImportTexture(texName.c_str());

	}
	ComponentTexture* tempCompTex = new ComponentTexture(tempObject, rMaterial);
    tempObject->AddComponent(tempCompTex);

	char* buffer = nullptr;
	Save(rMaterial, &buffer);
}


ResourceMaterial* Importer::TextureImporter::ImportTexture(const char* path)
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
    Texture tempTex;
    tempTex.id = tempid;
    tempTex.path = path;
    ResourceMaterial* tempMat = new ResourceMaterial(tempTex);

    return tempMat;
}

uint64 Importer::MaterialsImporter::Save(ResourceMaterial* rMaterial, char** buffer)
{
	uint size;
	char* cursor;
	uint bytes;

	size = sizeof(rMaterial->GetTexture()) + sizeof(rMaterial->GetColor().r) * 4;

	*buffer = new char[size];
	cursor = *buffer;

	uint64 textureID = rMaterial->GetId();
	bytes = sizeof(rMaterial->GetTexture());
	memcpy(cursor, &textureID, bytes);
	cursor += bytes;

	float color[4]{ rMaterial->GetColor().a, rMaterial->GetColor().g, rMaterial->GetColor().b, rMaterial->GetColor().a };
	bytes = sizeof(float) * 4;
	memcpy(cursor, color, bytes);
	cursor += bytes;

	std::string path = MATERIALS_PATH + std::to_string(rMaterial->GetUID()) + MATERIAL_EXTENSION;
	App->fileSystem->Save(path.c_str(), *buffer, size);

	return size;
}

void Importer::MaterialsImporter::Load(ResourceMaterial* rMaterial, char* buffer)
{
	char* cursor;
	uint64 textureID;
	uint bytes;

	cursor = buffer;

	bytes = sizeof(rMaterial->GetTexture());
	memcpy(&textureID, cursor, bytes);
	cursor += bytes;
	rMaterial->SetId(textureID);

	float color[4];
	bytes = sizeof(float) * 4;
	memcpy(color, cursor, bytes);
	cursor += bytes;

	Color _color = Color(color[0], color[1], color[2], color[3]);
	rMaterial->SetColor(_color);
}
