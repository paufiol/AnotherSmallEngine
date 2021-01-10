#include "ImporterScene.h"
#include "ImporterMesh.h"
#include "ImporterMaterials.h"

#include "Application.h"
#include "GameObject.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "JsonConfig.h"

#include "ModuleResource.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceScene.h"
#include "ResourceModel.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ModuleScene.h"
#include "ResourceScene.h"
#include "ModuleCamera3D.h"

#include <unordered_map>

#include "Dependencies/Assimp/include/cimport.h"
#include "Dependencies/Assimp/include/scene.h"
#include "Dependencies/Assimp/include/postprocess.h"
#pragma comment (lib, "Dependencies/Assimp/libx86/assimp.lib")

//#include "Dependencies/MathGeoLib/include/MathGeoLib.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/Quat.h"


const aiScene* Importer::ModelImporter::ImportAssimpScene(const char* buffer, uint size)
{
	const aiScene* aiScene;
	aiScene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	
	return aiScene;
}

void Importer::ModelImporter::ImportScene(const aiScene* aiScene, ResourceModel* resourceScene)
{
	//LOG("Importing Scene: %s", scenePath);
	//char* buffer = nullptr;
	//uint size = App->fileSystem->Load(scenePath, &buffer);
		
	Importer::ModelImporter::IterateNodes( aiScene, aiScene->mRootNode, resourceScene, 0);

	//(*resourceScene->models.begin()).name = resourceScene->name;

}

void Importer::ModelImporter::IterateNodes(const aiScene* aiScene, aiNode* node, ResourceModel* resourceScene, uint32 ID)
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

	//float4x4 transform = float4x4::FromTRS(position, rotation, scale);
	ModelContainer model(App->resources->GenerateNewUID(), node->mName.C_Str(), position, scale, rotation, ID);
	
	LOG("Importing Model: %d", model.ID);


	for (uint i = 0; i < node->mNumMeshes && i < 1; i++)
	{
		model.meshID = node->mMeshes[i];
		model.materialID = aiScene->mMeshes[node->mMeshes[i]]->mMaterialIndex;
	}

	resourceScene->models.push_back(model);
	

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		ModelImporter::IterateNodes( aiScene, node->mChildren[i], resourceScene, model.ID);
	}
}

uint32 Importer::ModelImporter::Save(const ResourceModel* resourceScene, char** buffer)
{
	JsonConfig jsonFile;
	ArrayConfig jsonArrray = jsonFile.SetArray("Models");

	for (uint i = 0; i < resourceScene->models.size(); i++)
	{
		JsonConfig& node = jsonArrray.AddNode();

		node.SetString("Name", resourceScene->models[i].name.c_str());
		node.SetNumber("UID", resourceScene->models[i].ID);
		node.SetNumber("Parent UID", resourceScene->models[i].parentID);

		node.SetNumber("Mesh UID", resourceScene->models[i].meshID);
		node.SetNumber("Material UID", resourceScene->models[i].materialID);
		//Here we need to store the Transform!!! HOW?

		node.SetFloat3("Position", resourceScene->models[i].position);
		node.SetFloat3("Scale", resourceScene->models[i].scale);
		node.SetQuat("Rotation", resourceScene->models[i].rotation);


	}

	uint size = jsonFile.SerializeConfig(buffer);


	return size;
}

void Importer::ModelImporter::Load(ResourceModel* resourceScene, char* buffer)
{

	JsonConfig jsonFile(buffer); //needs buffer??
	ArrayConfig jsonArrray = jsonFile.GetArray("Models");

	std::map<uint32, GameObject*> GameObjects;

	for (uint i = 0; i < jsonArrray.GetSize(); i++)
	{

		JsonConfig model = jsonArrray.GetNode(i);

		string name = model.GetString("Name");
		GameObject* tempGameObject = new GameObject(name);

		GameObject* parent = nullptr;
		std::map<uint32, GameObject*>::iterator it = GameObjects.find(model.GetNumber("Parent UID"));
		if (it != GameObjects.end())
		{
			parent = it->second;
		}
		if (parent != nullptr)
		{
			tempGameObject->SetParent(parent);
			parent->AddChildren(tempGameObject);
		}
		else
		{
			tempGameObject->SetParent(App->scene->root_object);
			App->scene->root_object->AddChildren(tempGameObject);
		}

		tempGameObject->SetUID(model.GetNumber("UID"));

		GameObjects[model.GetNumber("UID")] = tempGameObject;

	
		uint meshUID = model.GetNumber("Mesh UID");
		uint materialUID = model.GetNumber("Material UID");
		if (meshUID != 0)
		{
			ComponentMesh* tempCompMesh = new ComponentMesh(tempGameObject);
			ResourceMesh* tempResourceMesh = (ResourceMesh*)App->resources->LoadResource(meshUID);
			tempCompMesh->SetMesh(tempResourceMesh);
			tempGameObject->AddComponent(tempCompMesh);


		}
		if (materialUID != 0)
		{
			ComponentMaterial* tempCompMaterial = new ComponentMaterial(tempGameObject);

			Resource* resource = App->resources->GetResourceInMemory(materialUID);

 			ResourceMaterial* tempResourceMaterial = (ResourceMaterial*)App->resources->LoadResource(resource->UID);

			tempCompMaterial->SetMaterial(tempResourceMaterial);
			tempGameObject->AddComponent(tempCompMaterial);
		}



		//Get Transform
		float3 position, scale;
		Quat rotation;

		position = model.GetFloat3("Position");
		scale = model.GetFloat3("Scale");
		rotation = model.GetQuat("Rotation");
		//Set Transform 
		tempGameObject->transform->SetPosition(position);
		tempGameObject->transform->SetScale(scale);
		tempGameObject->transform->SetRotation(rotation);


		App->scene->game_objects.push_back(tempGameObject);
	}
}


uint32 Importer::SceneImporter::Save(const ResourceScene* scene, char**buffer )
{
	JsonConfig jsonFile;
	ArrayConfig jsonGOArray = jsonFile.SetArray("GameObjects");

	for (uint i = 0; i < App->scene->game_objects.size(); i++)
	{
		JsonConfig& sceneConfig = jsonGOArray.AddNode();

		sceneConfig.SetString("Name", App->scene->game_objects[i]->name.c_str());
		sceneConfig.SetNumber("UID", App->scene->game_objects[i]->GetUID());

		sceneConfig.SetNumber("Parent UID", App->scene->game_objects[i]->parent == nullptr ? 0 : App->scene->game_objects[i]->parent->GetUID());
		sceneConfig.SetBool("IsSelected", App->scene->game_objects[i]->IsSelected());

		ArrayConfig jsonCompArray = sceneConfig.SetArray("Components");

		for (int j = 0; j < App->scene->game_objects[i]->components.size(); ++j)
		{
			Component* component = App->scene->game_objects[i]->components[j];
			JsonConfig& compConfig = jsonCompArray.AddNode();
			
			ComponentMesh* componentMesh = (ComponentMesh*)App->scene->game_objects[i]->GetComponent(ComponentType::Mesh);
			ComponentMaterial* componentMat = (ComponentMaterial*)App->scene->game_objects[i]->GetComponent(ComponentType::Material);
			ComponentTransform* componentTransform = (ComponentTransform*)App->scene->game_objects[i]->GetComponent(ComponentType::Transform);
			ComponentCamera* componentCamera = (ComponentCamera*)App->scene->game_objects[i]->GetComponent(ComponentType::Camera);

			switch (component->type)
			{
			case (ComponentType::Mesh):
				compConfig.SetString("Type", "Mesh");
				compConfig.SetNumber("Resource UID", componentMesh->GetMesh()->GetUID());
				break;
			case (ComponentType::Material):
				
				compConfig.SetString("Type", "Material");
				compConfig.SetNumber("Resource UID", componentMat->GetMaterial()->GetUID());
				compConfig.SetNumber("Shader UID", componentMat->GetMaterial()->GetShader()->GetUID());
				if(componentMat->GetMaterial()->GetShader()->name != "DefaultShader") App->resources->SaveResource(componentMat->GetMaterial()->GetShader());
				componentMat->GetMaterial()->GetTexture()->id != 0 ? compConfig.SetNumber("Texture UID", componentMat->GetMaterial()->GetTexture()->GetUID()) : 0 ;
				
				break;
			case (ComponentType::Transform):
				
				compConfig.SetString("Type", "Transform");
				compConfig.SetFloat3("Position", componentTransform->GetPosition());
				compConfig.SetFloat3("Scale", componentTransform->GetScale());
				compConfig.SetQuat("Rotation", componentTransform->GetRotation());


				compConfig.SetFloat3("GlobalPosition", componentTransform->GetGlobalPosition());
				compConfig.SetFloat3("GlobalScale", componentTransform->GetGlobalScale());
				compConfig.SetQuat("GlobalRotation", componentTransform->GetGlobalRotation());

				break;
			case(ComponentType::Camera):
				compConfig.SetString("Type", "Camera");
				compConfig.SetNumber("NearPlane", componentCamera->GetNearPlane());
				compConfig.SetNumber("FarPlane", componentCamera->GetFarPlane());
				compConfig.SetNumber("FOV", componentCamera->GetFOV());
				compConfig.SetBool("Culling", componentCamera->frustum_culling);
				compConfig.SetBool("BoundingBoxes", componentCamera->draw_boundingboxes);
				compConfig.SetBool("isActive", componentCamera->active_camera);
				break;
			}

		}
	}

	uint size = jsonFile.SerializeConfig(buffer);
	return size;
}

void Importer::SceneImporter::Load(ResourceScene* resourceScene, char* buffer)
{
	JsonConfig jsonFile(buffer);
	std::vector<GameObject*> importedGameObjects;
	App->scene->DeleteAllGameObjects();
	ArrayConfig arrayGameObjects = jsonFile.GetArray("GameObjects");

	for (uint i = 0; i < arrayGameObjects.GetSize(); i++)
	{
		JsonConfig node = arrayGameObjects.GetNode(i);
		//Set Game Object Name
		string name = node.GetString("Name");
		GameObject* gameObject = new GameObject(name);
		//Set Game Object UID & selected bool
		gameObject->SetUID(node.GetNumber("UID"));
		if (node.GetBool("IsSelected")) App->scene->selected_object = gameObject;

		//Set Game Object Parent if it has one
		GameObject* parent = nullptr;
		uint32 parentUID = node.GetNumber("Parent UID");

		for (uint i = 0; i < importedGameObjects.size(); i++)
		{
			if (importedGameObjects[i]->GetUID() == node.GetNumber("Parent UID"))
			{
				parent = importedGameObjects[i];
			}
		}
		if (parent != nullptr)
		{
			gameObject->SetParent(parent);
			parent->AddChildren(gameObject);
		}
		else
		{
			App->scene->root_object = gameObject;
		}
		//Set all Game Objects components 
		ArrayConfig componentsArray = node.GetArray("Components");
		for (uint i = 0; i < componentsArray.GetSize(); i++)
		{
			JsonConfig componentNode = componentsArray.GetNode(i);
			string type = componentNode.GetString("Type");
			if(type == "Mesh") 
			{
				ComponentMesh* tempCompMesh = new ComponentMesh(gameObject);
				ResourceMesh* tempResourceMesh = (ResourceMesh*)App->resources->LoadResource(componentNode.GetNumber("Resource UID"));
				tempCompMesh->SetMesh(tempResourceMesh);
				gameObject->AddComponent(tempCompMesh);
			}
			else if (type == "Material") 
			{
				ComponentMaterial* tempCompMaterial = new ComponentMaterial(gameObject);
				ResourceMaterial* material = (ResourceMaterial*)App->resources->LoadResource(componentNode.GetNumber("Resource UID"));

				ResourceShader* shader = (ResourceShader*)App->resources->LoadResource(componentNode.GetNumber("Shader UID"));

				material->SetShader(shader);

				uint32 textureUID = componentNode.GetNumber("Texture UID");
				if (textureUID != 0)
				{
					ResourceTexture* texture = (ResourceTexture*)App->resources->LoadResource(componentNode.GetNumber("Texture UID"));
					material->SetTexture(texture);
				}

				tempCompMaterial->SetMaterial(material);
				gameObject->AddComponent(tempCompMaterial);
			}
			else if (type == "Transform") 
			{
				gameObject->transform->UpdateTransform(componentNode.GetFloat3("Position"), componentNode.GetFloat3("Scale"), componentNode.GetQuat("Rotation"));

			}
			else if (type == "Camera")
			{
				ComponentCamera* camera = new ComponentCamera(gameObject);
				App->camera->gameCamera = camera;
				camera->draw_boundingboxes = componentNode.GetBool("BoundingBoxes");
				camera->frustum_culling = componentNode.GetBool("Culling");
				camera->active_camera = componentNode.GetBool("isActive");
				camera->SetFOV(componentNode.GetNumber("FOV"));
				camera->SetNearPlane(componentNode.GetNumber("NearPlane"));
				camera->SetFarPlane(componentNode.GetNumber("FarPlane"));
				gameObject->AddComponent(camera);
			}
		}
		importedGameObjects.push_back(gameObject);
	}
	App->scene->game_objects = importedGameObjects;
	//App->scene->FillGameObjectsVector(importedGameObjects);
	
}

