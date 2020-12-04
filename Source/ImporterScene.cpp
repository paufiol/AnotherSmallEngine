#include "ImporterScene.h"
#include "ImporterMesh.h"
#include "ImporterMaterials.h"

#include "Application.h"
#include "GameObject.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "JsonConfig.h"

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

		Importer::SceneImporter::IterateNodes(scenePath, aiScene, aiScene->mRootNode, App->scene->root_object);
	}

}

void Importer::SceneImporter::IterateNodes(const char* scenePath, const aiScene* aiScene, aiNode* node, GameObject* parent)
{
	GameObject* tempObject = new GameObject();

	//Importing Transforms-----------------------------------------------------------
	aiVector3D		aiPosition;
	aiVector3D		aiScale;
	aiQuaternion	aiRotation;

	node->mTransformation.Decompose(aiScale, aiRotation, aiPosition);

	float3	position(aiPosition.x, aiPosition.y, aiPosition.z);
	float3	scale(aiScale.x, aiScale.y, aiScale.z);
	Quat	rotation(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);
	Quat	tempRotation;

	while (strstr(node->mName.C_Str(), "_$AssimpFbx$_") != nullptr && node->mNumChildren == 1)
	{
		node = node->mChildren[0];
		node->mTransformation.Decompose(aiScale, aiRotation, aiPosition);

		tempRotation = Quat(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);

		position.x += aiPosition.x;
		position.y += aiPosition.y;
		position.z += aiPosition.z;

		scale.x *= aiScale.x;
		scale.y *= aiScale.y;
		scale.z *= aiScale.z;

		rotation = rotation * tempRotation;

	}



	if (node->mNumMeshes > 0)
	{
		//Importing meshes------------------------------------------------------------
		
		std::vector<ResourceMesh*> meshes = Importer::MeshImporter::LoadMeshes(aiScene, node);

		for (uint i = 0; i < meshes.size(); i++)
		{
			
			ComponentMesh* tempCompMesh = new ComponentMesh(tempObject);
			tempCompMesh->SetMesh(meshes[i]);
			tempCompMesh->SetPath(scenePath);
			tempObject->AddComponent(tempCompMesh);

			LOG("Imported the mesh of the node: %s", node->mName.C_Str());

			//Importing Materials--------------------------------------------------------

			aiMesh* aiTempMesh = aiScene->mMeshes[node->mMeshes[i]];

			if (aiTempMesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = aiScene->mMaterials[aiTempMesh->mMaterialIndex];

				Importer::MaterialsImporter::ImportMaterial(material,tempObject);
				
			}
		}
	}

	//---------------------------------------------------------------------------------
	tempObject->SetName(node->mName.C_Str());
	
	parent->AddChildren(tempObject);

	tempObject->transform->UpdateTransform(position, scale, rotation);

	App->scene->game_objects.push_back(tempObject);

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		SceneImporter::IterateNodes(scenePath, aiScene, node->mChildren[i], tempObject);
	}
}

void Importer::SceneImporter::Save(GameObject* gameObject, std::string scene)
{
	JsonConfig jsonFile;
	ArrayConfig jsonGOArray = jsonFile.SetArray("GameObjects");

	std::vector<GameObject*> GOarray;
	
	gameObject->FillGameObjectArray(gameObject, GOarray);

	for (uint i = 0; i < GOarray.size(); i++)
	{
		JsonConfig& sceneConfig = jsonGOArray.AddNode();

		sceneConfig.SetString("Name", GOarray[i]->name.c_str());
		sceneConfig.SetNumber("UID", GOarray[i]->UID);
		sceneConfig.SetNumber("Parent UID", GOarray[i]->IsRootObject() ? GOarray[i]->parent->UID : 0);
		sceneConfig.SetBool("IsSelected", GOarray[i]->IsSelected());
		//config.SetBool("IsOpenHeriarchy")

		ArrayConfig jsonCompArray = sceneConfig.SetArray("Components");

		for (int j = 0; j < GOarray[i]->components.size(); ++j)
		{
			Component* component = GOarray[i]->components[j];
			JsonConfig& compConfig = jsonCompArray.AddNode();
		}
	}


}
