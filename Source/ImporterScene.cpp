#include "ImporterScene.h"
#include "ImporterMesh.h"
#include "ImporterTexture.h"

#include "Application.h"
#include "GameObject.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"

#include "ResourceMesh.h"
#include "ResourceMaterial.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"

#include "Dependencies/Assimp/include/cimport.h"
#include "Dependencies/Assimp/include/scene.h"
#include "Dependencies/Assimp/include/postprocess.h"
#pragma comment (lib, "Dependencies/Assimp/libx86/assimp.lib")

//#include "Dependencies/MathGeoLib/include/MathGeoLib.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/Quat.h"

void Importer::SceneImporter::ImportScene(const char* scenePath)
{
	LOG("Importing Scene: %s", scenePath);
	char* buffer = nullptr;
	uint size = App->fileSystem->Load(scenePath, &buffer);

	if (size > 0)
	{
		const aiScene* aiScene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

		Importer::SceneImporter::IterateNodes(scenePath, aiScene, aiScene->mRootNode, App->scene_intro->root_object);
	}

}

void Importer::SceneImporter::IterateNodes(const char* scenePath, const aiScene* aiScene, aiNode* node, GameObject* parent)
{
	GameObject* tempObject = new GameObject(node->mName.C_Str());

	if (node->mNumMeshes > 0)
	{
		//Importing meshes------------------------------------------------------------
		
		std::vector<ResourceMesh*> meshes = Importer::MeshImporter::LoadMeshes(aiScene, node);

		for (uint i = 0; i < meshes.size(); i++)
		{
			
			ComponentMesh* tempCompMesh = new ComponentMesh(tempObject);
			tempCompMesh->SetMesh(meshes[i]);
			tempObject->AddComponent(tempCompMesh);

			LOG("Imported the mesh of the node: %s", node->mName.C_Str());

			//Importing Materials--------------------------------------------------------

			aiMesh* aiTempMesh = aiScene->mMeshes[node->mMeshes[i]];

			if (aiTempMesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = aiScene->mMaterials[aiTempMesh->mMaterialIndex];

				ResourceMaterial* rMaterial = new ResourceMaterial();

				aiColor4D	color;
				aiString	texPath;
				string		texName;
				string		texExtension;

				if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)										// Could also get specular and ambient occlusion colours.
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
			}
		}
	}

	//Importing Transforms-----------------------------------------------------------
	aiVector3D		aiPosition;
	aiVector3D		aiScale;
	aiQuaternion	aiRotation;

	node->mTransformation.Decompose(aiScale, aiRotation, aiPosition);

	float3	position(aiPosition.x, aiPosition.y, aiPosition.z);
	float3	scale(aiScale.x, aiScale.y, aiScale.z);
	Quat	rotation(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);


	while (strstr(node->mName.C_Str(), "_$AssimpFbx$_") != nullptr && node->mNumChildren == 1)
	{
		node = node->mChildren[0];
		node->mTransformation.Decompose(aiScale, aiRotation, aiPosition);

		position.x += aiPosition.x;
		position.y += aiPosition.y;
		position.z += aiPosition.z;

		scale.x *= aiScale.x;
		scale.y *= aiScale.y;
		scale.z *= aiScale.z;

		rotation.x *= aiRotation.x;
		rotation.y *= aiRotation.y;
		rotation.z *= aiRotation.z;
		rotation.w *= aiRotation.w;
	}
	
	//---------------------------------------------------------------------------------

	parent->AddChildren(tempObject);
	tempObject->transform->UpdateTransform(position, scale, rotation);
	App->scene_intro->game_objects.push_back(tempObject);
	


	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		SceneImporter::IterateNodes(scenePath, aiScene, node->mChildren[i], tempObject);
	}
}