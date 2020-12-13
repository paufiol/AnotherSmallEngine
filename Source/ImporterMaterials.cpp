#include "Globals.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "ImporterMaterials.h"

#include "GameObject.h"

#include "ComponentTexture.h"

#include "ModuleResource.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"

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



void Importer::MaterialsImporter::ImportMaterial(aiMaterial* aiMaterial, ResourceMaterial* resourceMaterial)
{

	

	aiColor4D	color;
	aiString	texPath;
	aiString	matName;
	std::string		texName;
	std::string		texExtension;

	if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)										// Could also get specular and ambient occlusion colours.
	{
		resourceMaterial->SetColor(Color(color.r, color.g, color.b, color.a));
		aiMaterial->Get(AI_MATKEY_NAME, matName);
		resourceMaterial->name = matName.C_Str();
	}
	if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
	{
		App->fileSystem->SplitFilePath(texPath.C_Str(), nullptr, &texName, &texExtension);

		texName = "Assets/Textures/" + texName + "." + texExtension;

		char* buffer = nullptr;
		uint64 size =  App->fileSystem->Load(texName.c_str(), &buffer);



		//This shouldn't be here (Works for now)
		ResourceTexture* tempTex = new ResourceTexture();

		Importer::TextureImporter::Load(tempTex, buffer, size);

		resourceMaterial->SetTexture(tempTex);
		resourceMaterial->SetPath(texName);
	}
	else
	{
		ResourceTexture* tempTex = new ResourceTexture();
		resourceMaterial->SetTexture(tempTex);
	}


}

uint64 Importer::MaterialsImporter::Save(ResourceMaterial* rMaterial, char** buffer)
{
	uint size;
	char* cursor;
	uint bytes;

	size = sizeof(unsigned long long) + sizeof(float) * 4;

	*buffer = new char[size];
	cursor = *buffer;

	uint64 textureID = rMaterial->GetId();
	bytes = sizeof(unsigned long long);
	memcpy(cursor, &textureID, bytes);
	cursor += bytes;

	float color[4]{ rMaterial->GetColor().r, rMaterial->GetColor().g, rMaterial->GetColor().b, rMaterial->GetColor().a };
	bytes = sizeof(float) * 4;
	memcpy(cursor, color, bytes);
	cursor += bytes;

	/*std::string path = MATERIALS_PATH + std::to_string(rMaterial->GetUID()) + MATERIAL_EXTENSION;
	App->fileSystem->Save(path.c_str(), *buffer, size);*/

	return size;
}

void Importer::MaterialsImporter::Load(ResourceMaterial* rMaterial, const char* buffer)
{
	uint64 textureID = 0;
	uint bytes;

	const char* cursor = buffer;
	 
	bytes = sizeof(unsigned long long);
	memcpy(&textureID, cursor, bytes);
	cursor += bytes;
	//rMaterial->SetId(textureID);

	float color[4];
	bytes = sizeof(float) * 4;
	memcpy(color, cursor, bytes);
	cursor += bytes;

	Color _color = Color(color[0], color[1], color[2], color[3]);
	rMaterial->SetColor(_color);
	//rMaterial->SetPath(rMaterial->GetAssetsFile());

	ResourceTexture* texture = new ResourceTexture(textureID, rMaterial->GetAssetsFile());
	rMaterial->SetTexture(texture);
}

void Importer::TextureImporter::ImportTexture(ResourceTexture* rMaterial, const char* buffer, uint size)
{

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
	{
		LOG("Succesfully imported texture");
	}
	else
	{
		LOG("ERROR: Texture could not be loaded");
	}
}

uint64 Importer::TextureImporter::Save(const ResourceTexture* rMaterial, char** buffer)
{
	ilEnable(IL_FILE_OVERWRITE);

	ILuint size;
	ILubyte* ILbuffer;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0) {
		ILbuffer = new ILubyte[size];
		if (ilSaveL(IL_DDS, ILbuffer, size) > 0)
		{
			*buffer = (char*)ILbuffer;
		}
	}



	return size;
}

void Importer::TextureImporter::Load(ResourceTexture* rTexture, char* buffer, uint size)
{
	ILuint Il_Tex;

	ilGenImages(1, &Il_Tex);
	ilBindImage(Il_Tex);

	ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size);
	rTexture->id = (ilutGLBindTexImage());
	ilDeleteImages(1, &Il_Tex);

}

