#include "Application.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include "ImporterScene.h"
#include "ImporterMesh.h"
#include "ResourceMesh.h"
#include "ModuleInput.h"
#include "ResourceMaterial.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	root_object = new GameObject("Scene");
	game_objects.push_back(root_object);
	root_object->parent = nullptr;
	selected_object = root_object;
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//CreateGameObject("House", "Assets/Models/BakerHouse.FBX","Assets/Textures/BakerHouse.png");
	////CreateGameObject("Street Environment", "Assets/Models/Street_environment.FBX", "Assets/Textures/Street_environment1.png");

	//Importer::SceneImporter::ImportScene("Assets/Models/BakerHouse.FBX");
	Importer::SceneImporter::ImportScene("Assets/Models/Street_environment.FBX");
	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleScene::Update(float dt)
{

	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	for (uint n = 0; n < game_objects.size(); n++)
	{
		game_objects[n]->Update();
	}

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Update();
	}
	

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
		App->scene_intro->DeleteGameObject(App->scene_intro->selected_object);

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}


GameObject* ModuleScene::CreateGameObject(string name, GameObject* parent)
{
	std::string fullName = name;

	fullName += std::to_string(game_objects.size());

	GameObject* tempObject = new GameObject(fullName);

	if (tempObject != nullptr)
	{
		if (game_objects.empty())
		{
			root_object = tempObject;
		}

		game_objects.push_back(tempObject);

		if (parent != nullptr)
		{
			parent->AddChildren(tempObject);
			tempObject->SetParent(parent);
		}
	}

	game_objects.push_back(tempObject);
	
	return tempObject;
	
	//GameObject* tempGO = new GameObject(name);
	//int meshNum = 1;
	//if (meshPath != nullptr)
	//{
	//	//vector<ResourceMesh*> meshes = Importer::MeshImporter::LoadMeshes(meshPath);
	//	tempGO->SetParent(root_object);
	//	root_object->AddChildren(tempGO);
	//	game_objects.push_back(tempGO);
	//	if (meshes.size() == 1)
	//	{
	//		ComponentMesh* compMesh = new ComponentMesh(tempGO, meshPath, meshes.front());
	//		if (texturePath != nullptr)
	//		{
	//			ComponentTexture* compTex = new ComponentTexture(tempGO, texturePath);
	//			compTex->SetTexture(Importer::TextureImporter::Import(texturePath), texturePath);
	//			tempGO->AddComponent(compTex);
	//		}
	//		tempGO->AddComponent(compMesh);

	//	}
	//	else if (meshes.size() > 1)
	//	{
	//		vector<Mesh*>::iterator iterator = meshes.begin();
	//		for (; iterator != meshes.end(); iterator++)
	//		{
	//			string tempName = "Mesh ";
	//			string stringSize = to_string(meshNum);
	//			tempName += stringSize;

	//			GameObject* childGO = new GameObject(tempName);
	//			ComponentMesh* newComp = new ComponentMesh(childGO, meshPath, (*iterator));
	//			if (texturePath != nullptr)
	//			{
	//				ComponentTexture* compTex = new ComponentTexture(childGO, texturePath);
	//				compTex->SetTexture(Importer::TextureImporter::Import(texturePath), texturePath);
	//				childGO->AddComponent(compTex);
	//			}
	//			childGO->AddComponent(newComp);

	//			childGO->SetParent(tempGO);
	//			tempGO->AddChildren(childGO);
	//			game_objects.push_back(childGO);
	//			meshNum++;
	//		}
	//	}

	//	
	//}
}

void ModuleScene::AddGameObject(GameObject* object)
{
	selected_object = object;
	object->SetParent(root_object);
	root_object->AddChildren(object);
	game_objects.push_back(object); 

	//root_object->AppendChildren(object);
}

void ModuleScene::DeleteGameObject(GameObject* object) 
{
	if (object != nullptr && object != root_object) //Avoid deleting root
	{		
		for (uint i = 0; i < game_objects.size(); i++)
		{
			if (object == game_objects[i]) 
			{
				LOG("Game Object %s deleted", object->name.c_str());
				selected_object = nullptr;
				
				object->parent->EraseChild(object);

				game_objects.erase(game_objects.begin() + i);
				
				object->CleanUp();
			
				
				//delete object;
			}
		}
	}
}

void ModuleScene::SelectObject(GameObject* object)
{
	if (object != nullptr)
	{
		selected_object = object;
	}
}

