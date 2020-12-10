#include "Application.h"
#include "ModuleScene.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ImporterScene.h"
#include "ImporterMesh.h"
#include "ResourceMesh.h"
#include "ModuleInput.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include <map>

#include "Dependencies/MathGeoLib/include/Geometry/Triangle.h"

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

	CreateGameCamera();

	//Importer::SceneImporter::ImportScene("Assets/Models/BakerHouse.FBX");

	Importer::SceneImporter::ImportScene("Assets/Models/Street_environment.FBX");
	//Importer::SceneImporter::Save(root_object, "Assets/Models/Street_environment.FBX");

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
	for (uint n = 0; n < game_objects.size(); n++)
	{
		game_objects[n]->Update();
	}

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Update();
	}

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
		App->scene->DeleteGameObject(App->scene->selected_object);

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

GameObject* ModuleScene::CreateGameCamera() {

	GameObject* ret = new GameObject("Game Camera");

	AddGameObject(ret);

	ret->SetParent(root_object);

	//(ComponentTransform*)ret->GetComponent(ComponentType::Camera)->
	ret->AddComponent((Component*)new ComponentCamera(ret));

	App->camera->gameCamera = (ComponentCamera*)ret->GetComponent(ComponentType::Camera);

	return ret;
}

void ModuleScene::TestGameObjectSelection(const LineSegment& ray)
{
	std::vector<GameObject*> game_object_candidates;

	for (uint i = 0; i < game_objects.size(); i++)
	{
		//We don't test against meshless components. 
		if (game_objects[i]->GetComponent(ComponentType::Mesh) == nullptr) 
		{
			continue;
		}
		
		if (ray.Intersects(game_objects[i]->aabb)) 
		{
			if(ray.Intersects(game_objects[i]->obb))
			{
				game_object_candidates.push_back(game_objects[i]);
			}
		}
	}
	
	for (uint i = 0; i < game_object_candidates.size(); i++)
	{
		ComponentMesh* mesh_to_test = (ComponentMesh*)game_object_candidates[i]->GetComponent(ComponentType::Mesh);
		ResourceMesh* mesh = mesh_to_test->GetMesh();
		if (mesh != nullptr)
		{
			LineSegment local_ray = ray;
			
			ComponentTransform* c_transform = (ComponentTransform*)game_object_candidates[i]->GetComponent(ComponentType::Transform);
			
			local_ray.Transform(c_transform->GetGlobalTransform().Inverted());
			
			//Iterate points in a mesh jump 3 by 3 because triangles
			for(uint j = 0; j < mesh->size[mesh->index]; j += 3)
			{
				uint triangle_indices[3] = {mesh->indices[j] * 3, mesh->indices[j + 1] * 3, mesh->indices[j + 2] * 3 };
				

				vec point_a(&mesh->vertices[triangle_indices[0]]);

				vec point_b(&mesh->vertices[triangle_indices[1]]);

				vec point_c(&mesh->vertices[triangle_indices[2]]);
				
				Triangle triangle(point_a, point_b, point_c);

				if(local_ray.Intersects(triangle, nullptr, nullptr))//Great constructor GeoLib
				{
					selected_object = game_object_candidates[i];
					break;
				}
			}
		}
	}
}

bool ModuleScene::PerTriangleTest(const ResourceMesh* mesh)
{
	float3 closest_triangle;


	return false;
}

void ModuleScene::SelectObject(GameObject* object)
{
	if (object != nullptr)
	{
		selected_object = object;
	}
}

