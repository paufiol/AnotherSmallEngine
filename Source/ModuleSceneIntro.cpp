#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ModuleImporter.h" //TODO: NEEDED ONLY BECAUSE OF MESH CLASS-> Turn Mesh Class into stand alone file.
#include "ComponentTexture.h"
#include "ComponentMesh.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	root_object = new GameObject("Scene");
	game_objects.push_back(root_object);
	selected_object = root_object;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateGameObject("House", "Assets/Models/BakerHouse.FBX","Assets/Textures/BakerHouse.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
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
	
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::CreateGameObject(string name, const char* meshPath, const char* texturePath)
{
	GameObject* tempGO = new GameObject(name);
	int meshNum = 1;
	if (meshPath != nullptr)
	{
		Importer::MeshImporter::Import(meshPath);
		vector<Mesh*> meshes = Importer::MeshImporter::meshes;
		tempGO->SetParent(root_object);
		root_object->AddChildren(tempGO);
		game_objects.push_back(tempGO);
		if (meshes.size() == 1)
		{
			ComponentMesh* compMesh = new ComponentMesh(tempGO, meshPath, meshes.front());
			if (texturePath != nullptr)
			{
				ComponentTexture* compTex = new ComponentTexture(tempGO, texturePath);
				compTex->SetTexture(Importer::TextureImporter::Import(texturePath), texturePath);
				tempGO->AddComponent(compTex);
			}
			tempGO->AddComponent(compMesh);
		}
		else if (meshes.size() > 1)
		{
			vector<Mesh*>::iterator iterator = meshes.begin();
			for (; iterator != meshes.end(); iterator++)
			{
				string tempName = "Mesh ";
				string stringSize = to_string(meshNum);
				tempName += stringSize;

				GameObject* childGO = new GameObject(tempName);
				ComponentMesh* newComp = new ComponentMesh(childGO, meshPath, (*iterator));
				if (texturePath != nullptr)
				{
					ComponentTexture* compTex = new ComponentTexture(childGO, texturePath);
					compTex->SetTexture(Importer::TextureImporter::Import(texturePath), texturePath);
					childGO->AddComponent(compTex);
				}
				childGO->AddComponent(newComp);

				childGO->SetParent(tempGO);
				tempGO->AddChildren(childGO);
				game_objects.push_back(childGO);
				meshNum++;
			}
		}
	}


}

void ModuleSceneIntro::AddGameObject(GameObject* object)
{
	selected_object = object;
	object->SetParent(root_object);
	root_object->AddChildren(object);
	game_objects.push_back(object); 


	//root_object->AppendChildren(object);
}

void ModuleSceneIntro::SelectObject(GameObject* object)
{
	if (object != nullptr)
	{
		selected_object = object;
	}
}

